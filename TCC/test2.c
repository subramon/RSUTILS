#include <stdio.h>
#include <stdlib.h>
#include <libtcc.h>

// 1. Define the host function you want the dynamic code to call
int host_logger(const char *message, int code) {
    printf("[HOST LOG] Received from dynamic code: '%s' (Code: %d)\n", message, code);
    return code * 10; // Return a value back to the dynamic code
}

int main() {
    TCCState *s;
    int (*run_dynamic_script)(void);

    // 2. Initialize TCC state
    s = tcc_new();
    if (!s) {
        fprintf(stderr, "Could not create TCC state\n");
        return 1;
    }

    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    // 3. Register the host function with TCC
    // The first argument is the name the dynamic code will use to call it.
    // The second argument is the actual function pointer cast to (void *).
    if (tcc_add_symbol(s, "host_logger", (void *)host_logger) < 0) {
        fprintf(stderr, "Failed to add host symbol\n");
        tcc_delete(s);
        return 1;
    }

    // 4. Write dynamic C code that declares 'extern' for the host function and calls it
    char *dynamic_code = 
        "// Declare the external host function so the compiler knows its signature\n"
        "extern int host_logger(const char *msg, int val);\n"
        "\n"
        "int execute_script() {\n"
        "    int res = host_logger(\"Hello from inside dynamic TCC code!\", 42);\n"
        "    return res + 8;\n"
        "}\n";

    if (tcc_compile_string(s, dynamic_code) == -1) {
        fprintf(stderr, "Compilation failed\n");
        tcc_delete(s);
        return 1;
    }

    // 5. Relocate memory (this resolves the 'host_logger' symbol address)
    if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0) {
        fprintf(stderr, "Relocation failed\n");
        tcc_delete(s);
        return 1;
    }

    // 6. Look up and execute the entry point of our dynamic script
    run_dynamic_script = (int (*)(void))tcc_get_symbol(s, "execute_script");
    if (!run_dynamic_script) {
        fprintf(stderr, "Symbol 'execute_script' not found\n");
        tcc_delete(s);
        return 1;
    }

    int final_result = run_dynamic_script();
    printf("Result returned from host function chain: %d\n", final_result);

    // 7. Clean up
    tcc_delete(s);
    return 0;
}
