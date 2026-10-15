#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "gml_frontend.h"
#include "gml_vm.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    // 1. Test string_width and string_height
    {
        const char* script =
            "w = string_width(\"Hello World\");\n"
            "h = string_height(\"Hello World\");\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_w = gml_vm_get(&vm, "w");
        assert(v_w.real > 0.0);
        gml_value_free(&v_w);

        gml_value v_h = gml_vm_get(&vm, "h");
        assert(v_h.real > 0.0);
        gml_value_free(&v_h);

        gml_ast_free(ast);
    }

    // 2. Test math_hypot, deg_to_rad, rad_to_deg, math_min, math_max
    {
        const char* script =
            "hyp = math_hypot(3, 4);\n"
            "rad = deg_to_rad(180);\n"
            "deg = rad_to_deg(3.14159265358979323846);\n"
            "mn = math_min(10, 20);\n"
            "mx = math_max(10, 20);\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_hyp = gml_vm_get(&vm, "hyp");
        assert(fabs(v_hyp.real - 5.0) < 0.001);
        gml_value_free(&v_hyp);

        gml_value v_rad = gml_vm_get(&vm, "rad");
        assert(fabs(v_rad.real - 3.14159265) < 0.001);
        gml_value_free(&v_rad);

        gml_value v_deg = gml_vm_get(&vm, "deg");
        assert(fabs(v_deg.real - 180.0) < 0.001);
        gml_value_free(&v_deg);

        gml_value v_mn = gml_vm_get(&vm, "mn");
        assert(v_mn.real == 10.0);
        gml_value_free(&v_mn);

        gml_value v_mx = gml_vm_get(&vm, "mx");
        assert(v_mx.real == 20.0);
        gml_value_free(&v_mx);

        gml_ast_free(ast);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v12 PASSED SUCCESSFULLY.\n");
    return 0;
}
