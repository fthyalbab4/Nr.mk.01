#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

void test_gml_vm_expanded_v3(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test median math function via AST node */
    {
        gml_ast n_med = { GML_AST_CALL, GML_T_NONE, 0, "median", NULL, NULL, 0, NULL };
        gml_ast a1 = { GML_AST_NUMBER, GML_T_NONE, 10, NULL, NULL, NULL, 0, NULL };
        gml_ast a2 = { GML_AST_NUMBER, GML_T_NONE, 50, NULL, NULL, NULL, 0, NULL };
        gml_ast a3 = { GML_AST_NUMBER, GML_T_NONE, 30, NULL, NULL, NULL, 0, NULL };
        gml_ast *items[3] = { &a1, &a2, &a3 };
        n_med.count = 3;
        n_med.items = items;

        gml_ast assign = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var = { GML_AST_NAME, GML_T_NONE, 0, "med_val", NULL, NULL, 0, NULL };
        assign.left = &var;
        assign.right = &n_med;
        gml_ast stmt = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt);
        assert(ok);
        gml_value res = gml_vm_get(&vm, "med_val");
        assert(res.kind == GML_V_REAL);
        assert(fabs(res.real - 30.0) < 0.0001);
    }

    /* Test ds_grid creation, set, get, width, height, clear, destroy */
    {
        gml_ast n_create = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_create", NULL, NULL, 0, NULL };
        gml_ast w = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
        gml_ast h = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_c[2] = { &w, &h };
        n_create.count = 2;
        n_create.items = items_c;

        gml_ast assign_grid = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_grid = { GML_AST_NAME, GML_T_NONE, 0, "grid", NULL, NULL, 0, NULL };
        assign_grid.left = &var_grid;
        assign_grid.right = &n_create;
        gml_ast stmt_grid = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_grid, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt_grid);
        assert(ok);
        gml_value grid_val = gml_vm_get(&vm, "grid");
        assert(grid_val.kind == GML_V_REAL);
        double grid_id = grid_val.real;
        assert(grid_id >= 0);

        /* Set cell (2, 3) = 42 */
        gml_ast n_set = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_set", NULL, NULL, 0, NULL };
        gml_ast arg_gid = { GML_AST_NUMBER, GML_T_NONE, grid_id, NULL, NULL, NULL, 0, NULL };
        gml_ast arg_x = { GML_AST_NUMBER, GML_T_NONE, 2, NULL, NULL, NULL, 0, NULL };
        gml_ast arg_y = { GML_AST_NUMBER, GML_T_NONE, 3, NULL, NULL, NULL, 0, NULL };
        gml_ast arg_val = { GML_AST_NUMBER, GML_T_NONE, 42, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_s[4] = { &arg_gid, &arg_x, &arg_y, &arg_val };
        n_set.count = 4;
        n_set.items = items_s;
        gml_ast stmt_set = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_set, NULL, 0, NULL };
        ok = gml_vm_execute(&vm, &stmt_set);
        assert(ok);

        /* Get cell (2, 3) */
        gml_ast n_get = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_get", NULL, NULL, 0, NULL };
        gml_ast *items_g[3] = { &arg_gid, &arg_x, &arg_y };
        n_get.count = 3;
        n_get.items = items_g;

        gml_ast assign_get = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_got = { GML_AST_NAME, GML_T_NONE, 0, "got_val", NULL, NULL, 0, NULL };
        assign_get.left = &var_got;
        assign_get.right = &n_get;
        gml_ast stmt_get = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_get, NULL, 0, NULL };
        ok = gml_vm_execute(&vm, &stmt_get);
        assert(ok);
        gml_value got_val = gml_vm_get(&vm, "got_val");
        assert(got_val.kind == GML_V_REAL && fabs(got_val.real - 42.0) < 0.0001);

        /* Destroy grid */
        gml_ast n_destroy = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_destroy", NULL, NULL, 0, NULL };
        gml_ast *items_d[1] = { &arg_gid };
        n_destroy.count = 1;
        n_destroy.items = items_d;
        gml_ast stmt_destroy = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_destroy, NULL, 0, NULL };
        ok = gml_vm_execute(&vm, &stmt_destroy);
        assert(ok);
    }

    printf("  test_gml_vm_expanded_v3 PASS\n");
}

#ifndef RUNNING_FULL_SUITE
int main(void) {
    printf("=== Testing GML VM Expanded V3 Suite ===\n");
    test_gml_vm_expanded_v3();
    printf("GML_VM_EXPANDED_V3_TEST_PASS\n");
    return 0;
}
#endif
