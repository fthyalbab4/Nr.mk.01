#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "gml_frontend.h"
#include "gml_vm.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    // 1. Test degtorad and radtodeg
    {
        const char* script =
            "rad = degtorad(180);\n"
            "deg = radtodeg(3.14159265358979323846);\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_rad = gml_vm_get(&vm, "rad");
        assert(fabs(v_rad.real - 3.14159265) < 0.001);
        gml_value_free(&v_rad);

        gml_value v_deg = gml_vm_get(&vm, "deg");
        assert(fabs(v_deg.real - 180.0) < 0.001);
        gml_value_free(&v_deg);

        gml_ast_free(ast);
    }

    // 2. Test point_distance, point_direction, lengthdir_x, lengthdir_y
    {
        const char* script =
            "dist = point_distance(0, 0, 3, 4);\n"
            "dir = point_direction(0, 0, 10, 0);\n"
            "lx = lengthdir_x(10, 0);\n"
            "ly = lengthdir_y(10, 90);\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_dist = gml_vm_get(&vm, "dist");
        assert(fabs(v_dist.real - 5.0) < 0.001);
        gml_value_free(&v_dist);

        gml_value v_dir = gml_vm_get(&vm, "dir");
        assert(fabs(v_dir.real - 0.0) < 0.001);
        gml_value_free(&v_dir);

        gml_value v_lx = gml_vm_get(&vm, "lx");
        assert(fabs(v_lx.real - 10.0) < 0.001);
        gml_value_free(&v_lx);

        gml_value v_ly = gml_vm_get(&vm, "ly");
        assert(fabs(v_ly.real - (-10.0)) < 0.001 || fabs(v_ly.real - 10.0) < 0.001);
        gml_value_free(&v_ly);

        gml_ast_free(ast);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v13 PASSED SUCCESSFULLY.\n");
    return 0;
}
