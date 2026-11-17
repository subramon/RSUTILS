// sudo apt install libtcc-dev
// sudo apt install tcc
#include <stdio.h>
#include <stdlib.h>
#include <libtcc.h>

int main() {
    TCCState *s;
    int (*add_func)(int, int);

    // 1. Initialize the TCC context
    s = tcc_new();
    if (!s) {
        fprintf(stderr, "Could not create TCC state\n");
        return 1;
    }

    // 2. Set output to memory (TCC_OUTPUT_MEMORY allows immediate execution)
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    // 3. Optional: Add library search paths or include directories if needed
    // tcc_add_library_path(s, "/usr/lib");

    // 4. Compile dynamic C code string
    char *my_code = 
        "int dynamic_add(int a, int b) {\n"
        "    return a + b * 2;\n"
        "}\n";

    if (tcc_compile_string(s, my_code) == -1) {
        fprintf(stderr, "Compilation failed\n");
        tcc_delete(s);
        return 1;
    }

    // 5. Relocate the code in memory to resolve symbols and jump targets
    if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0) {
        fprintf(stderr, "Relocation failed\n");
        tcc_delete(s);
        return 1;
    }

    // 6. Look up the symbol (function name) compiled in the code string
    add_func = (int (*)(int, int))tcc_get_symbol(s, "dynamic_add");
    if (!add_func) {
        fprintf(stderr, "Symbol not found\n");
        tcc_delete(s);
        return 1;
    }

    // 7. Execute the dynamically compiled function
    int result = add_func(10, 5); // Evaluates: 10 + 5 * 2 = 20
    printf("Result from dynamically compiled code: %d\n", result);

    // 8. Clean up TCC state
    tcc_delete(s);
    return 0;
}
