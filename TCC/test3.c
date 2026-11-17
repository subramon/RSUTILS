#include <stdio.h>
#include <stdlib.h>
#include <libtcc.h>

int main() {
    TCCState *s;
    double (*compute_hypot)(double, double);

    s = tcc_new();
    if (!s) {
        fprintf(stderr, "Could not create TCC state\n");
        return 1;
    }

    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    // Optional: Explicitly add system include paths if your OS layout requires it
    tcc_add_sysinclude_path(s, "/usr/include");

    // Link the math library so math.h symbols (like sqrt) can resolve
    if (tcc_add_library(s, "m") < 0) {
        fprintf(stderr, "Could not link libm\n");
        tcc_delete(s);
        return 1;
    }

    // Write dynamic code utilizing standard headers
    char *code = 
        "#include <stdio.h>\n"
        "#include <math.h>\n"
        "\n"
        "double calculate_hypotenuse(double a, double b) {\n"
        "    printf(\"[TCC Internal] Calculating hypotenuse for %.2f and %.2f\\n\", a, b);\n"
        "    return sqrt((a * a) + (b * b));\n"
        "}\n";

    if (tcc_compile_string(s, code) == -1) {
        fprintf(stderr, "Compilation failed\n");
        tcc_delete(s);
        return 1;
    }

    if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0) {
        fprintf(stderr, "Relocation failed\n");
        tcc_delete(s);
        return 1;
    }

    compute_hypot = (double (*)(double, double))tcc_get_symbol(s, "calculate_hypotenuse");
    if (!compute_hypot) {
        fprintf(stderr, "Symbol not found\n");
        tcc_delete(s);
        return 1;
    }

    double result = compute_hypot(3.0, 4.0);
    printf("Result from dynamic code: %.2f\n", result);

    tcc_delete(s);
    return 0;
}
