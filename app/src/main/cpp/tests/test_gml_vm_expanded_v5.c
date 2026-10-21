#include "gm82_runtime.h"
#include "gm82_sound_runtime.h"
#include "gm82_gml_builtins.h"
#include "gm82_gml_eval.h"
#include "gm82_input.h"
#include "gml_frontend.h"
#include "gml_vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

void test_gml_vm_expanded_v5(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test ds_grid_set_region */
    {
        double grid_id = gml_ds_grid_create(4, 4);
        assert(grid_id >= 0);

        gml_ds_grid_set_region(grid_id, 1, 1, 3, 3, 42.0);
        assert(fabs(gml_ds_grid_get(grid_id, 0, 0) - 0.0) < 0.0001);
        assert(fabs(gml_ds_grid_get(grid_id, 1, 1) - 42.0) < 0.0001);
        assert(fabs(gml_ds_grid_get(grid_id, 3, 3) - 42.0) < 0.0001);

        gml_ds_grid_destroy(grid_id);
    }

    /* Test string_repeat_ext */
    {
        char out[256];
        gml_string_repeat_ext("abc", 3, out, sizeof(out));
        assert(strcmp(out, "abcabcabc") == 0);
    }

    /* Test hypot */
    {
        double h = gml_math_hypot(3.0, 4.0);
        assert(fabs(h - 5.0) < 0.0001);
    }

    /* Test AST VM evaluation for hypot and string_repeat_ext */
    {
        /* AST VM evaluation for hypot */
        gml_ast n_hypot = { GML_AST_CALL, GML_T_NONE, 0, "hypot", NULL, NULL, 0, NULL };
        gml_ast a0 = { GML_AST_NUMBER, GML_T_NONE, 6.0, NULL, NULL, NULL, 0, NULL };
        gml_ast a1 = { GML_AST_NUMBER, GML_T_NONE, 8.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items[2] = { &a0, &a1 };
        n_hypot.count = 2;
        n_hypot.items = items;

        gml_ast assign = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var = { GML_AST_NAME, GML_T_NONE, 0, "h_res", NULL, NULL, 0, NULL };
        assign.left = &var;
        assign.right = &n_hypot;
        gml_ast stmt = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt);
        assert(ok);
        gml_value res = gml_vm_get(&vm, "h_res");
        assert(res.kind == GML_V_REAL && fabs(res.real - 10.0) < 0.0001);
    }

    printf("  test_gml_vm_expanded_v5 PASS\n");
}

#ifndef RUNNING_FULL_SUITE
int main(void) {
    printf("=== Testing GML VM Expanded V5 Suite ===\n");
    test_gml_vm_expanded_v5();
    printf("GML_VM_EXPANDED_V5_TEST_PASS\n");
    return 0;
}
#endif
