#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

void test_gml_vm_expanded_v5(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test ds_grid_set_region, ds_grid_value_exists, ds_grid_value_x, ds_grid_value_y */
    {
        double grid_id = gml_ds_grid_create(5, 5);
        assert(grid_id >= 0);

        gml_ds_grid_set_region(grid_id, 1, 1, 3, 3, 99.0);
        assert(fabs(gml_ds_grid_get(grid_id, 2, 2) - 99.0) < 0.0001);
        assert(fabs(gml_ds_grid_get(grid_id, 0, 0) - 0.0) < 0.0001);

        assert(gml_ds_grid_value_exists(grid_id, 0, 0, 4, 4, 99.0) == 1.0);
        assert(gml_ds_grid_value_exists(grid_id, 0, 0, 4, 4, 123.0) == 0.0);

        double vx = gml_ds_grid_value_x(grid_id, 0, 0, 4, 4, 99.0);
        double vy = gml_ds_grid_value_y(grid_id, 0, 0, 4, 4, 99.0);
        assert(vx == 1.0 && vy == 1.0);

        /* Test VM AST evaluation for ds_grid_value_exists */
        gml_ast n_call = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_value_exists", NULL, NULL, 0, NULL };
        gml_ast a0 = { GML_AST_NUMBER, GML_T_NONE, grid_id, NULL, NULL, NULL, 0, NULL };
        gml_ast a1 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast a2 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast a3 = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
        gml_ast a4 = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
        gml_ast a5 = { GML_AST_NUMBER, GML_T_NONE, 99.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items[6] = { &a0, &a1, &a2, &a3, &a4, &a5 };
        n_call.count = 6;
        n_call.items = items;

        gml_ast assign = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var = { GML_AST_NAME, GML_T_NONE, 0, "found_val", NULL, NULL, 0, NULL };
        assign.left = &var;
        assign.right = &n_call;
        gml_ast stmt = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt);
        assert(ok);
        gml_value res = gml_vm_get(&vm, "found_val");
        assert(res.kind == GML_V_REAL && res.real == 1.0);

        gml_ds_grid_destroy(grid_id);
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
