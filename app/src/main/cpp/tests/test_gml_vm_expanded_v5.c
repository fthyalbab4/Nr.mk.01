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

    /* 1. Test ds_grid_create, ds_grid_fill, ds_grid_set_region, ds_grid_get */
    {
        gml_ast n_create = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_create", NULL, NULL, 0, NULL };
        gml_ast w = { GML_AST_NUMBER, GML_T_NONE, 5, NULL, NULL, NULL, 0, NULL };
        gml_ast h = { GML_AST_NUMBER, GML_T_NONE, 5, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_c[2] = { &w, &h };
        n_create.count = 2;
        n_create.items = items_c;

        gml_ast assign_grid = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_grid = { GML_AST_NAME, GML_T_NONE, 0, "g5", NULL, NULL, 0, NULL };
        assign_grid.left = &var_grid;
        assign_grid.right = &n_create;
        gml_ast stmt_grid = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_grid, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt_grid);
        assert(ok);
        gml_value grid_val = gml_vm_get(&vm, "g5");
        assert(grid_val.kind == GML_V_REAL);
        double gid = grid_val.real;

        /* ds_grid_fill(g5, 10) */
        gml_ast n_fill = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_fill", NULL, NULL, 0, NULL };
        gml_ast arg_gid = { GML_AST_NUMBER, GML_T_NONE, gid, NULL, NULL, NULL, 0, NULL };
        gml_ast arg_fval = { GML_AST_NUMBER, GML_T_NONE, 10, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_f[2] = { &arg_gid, &arg_fval };
        n_fill.count = 2;
        n_fill.items = items_f;
        gml_ast stmt_fill = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_fill, NULL, 0, NULL };
        ok = gml_vm_execute(&vm, &stmt_fill);
        assert(ok);

        /* ds_grid_set_region(g5, 1, 1, 3, 3, 99) */
        gml_ast n_sr = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_set_region", NULL, NULL, 0, NULL };
        gml_ast x1 = { GML_AST_NUMBER, GML_T_NONE, 1, NULL, NULL, NULL, 0, NULL };
        gml_ast y1 = { GML_AST_NUMBER, GML_T_NONE, 1, NULL, NULL, NULL, 0, NULL };
        gml_ast x2 = { GML_AST_NUMBER, GML_T_NONE, 3, NULL, NULL, NULL, 0, NULL };
        gml_ast y2 = { GML_AST_NUMBER, GML_T_NONE, 3, NULL, NULL, NULL, 0, NULL };
        gml_ast rval = { GML_AST_NUMBER, GML_T_NONE, 99, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_sr[6] = { &arg_gid, &x1, &y1, &x2, &y2, &rval };
        n_sr.count = 6;
        n_sr.items = items_sr;
        gml_ast stmt_sr = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_sr, NULL, 0, NULL };
        ok = gml_vm_execute(&vm, &stmt_sr);
        assert(ok);

        /* Get cell (2, 2) which should be 99, and (0, 0) which should be 10 */
        assert(gml_ds_grid_get(gid, 2, 2) == 99.0);
        assert(gml_ds_grid_get(gid, 0, 0) == 10.0);

        gml_ds_grid_destroy(gid);
    }

    /* 2. Test ds_list_create, ds_list_add, ds_list_sort */
    {
        double lid = gml_ds_list_create();
        gml_ds_list_add(lid, 45.0);
        gml_ds_list_add(lid, 12.0);
        gml_ds_list_add(lid, 88.0);
        gml_ds_list_add(lid, 3.0);

        /* Call ds_list_sort(lid, 1) -> ascending */
        gml_ast n_sort = { GML_AST_CALL, GML_T_NONE, 0, "ds_list_sort", NULL, NULL, 0, NULL };
        gml_ast arg_lid = { GML_AST_NUMBER, GML_T_NONE, lid, NULL, NULL, NULL, 0, NULL };
        gml_ast arg_asc = { GML_AST_NUMBER, GML_T_NONE, 1, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_s[2] = { &arg_lid, &arg_asc };
        n_sort.count = 2;
        n_sort.items = items_s;
        gml_ast stmt_sort = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_sort, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt_sort);
        assert(ok);

        assert(gml_ds_list_find_value(lid, 0) == 3.0);
        assert(gml_ds_list_find_value(lid, 1) == 12.0);
        assert(gml_ds_list_find_value(lid, 2) == 45.0);
        assert(gml_ds_list_find_value(lid, 3) == 88.0);

        gml_ds_list_destroy(lid);
    }

    /* 3. Test string_pos_ext and string_last_pos */
    {
        gml_ast n_spe = { GML_AST_CALL, GML_T_NONE, 0, "string_pos_ext", NULL, NULL, 0, NULL };
        gml_ast sub = { GML_AST_STRING, GML_T_NONE, 0, "la", NULL, NULL, 0, NULL };
        gml_ast str = { GML_AST_STRING, GML_T_NONE, 0, "lalalalala", NULL, NULL, 0, NULL };
        gml_ast start_p = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_spe[3] = { &sub, &str, &start_p };
        n_spe.count = 3;
        n_spe.items = items_spe;

        gml_ast assign_spe = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_pos = { GML_AST_NAME, GML_T_NONE, 0, "p_ext", NULL, NULL, 0, NULL };
        assign_spe.left = &var_pos;
        assign_spe.right = &n_spe;
        gml_ast stmt_spe = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_spe, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt_spe);
        assert(ok);
        gml_value pval = gml_vm_get(&vm, "p_ext");
        assert(pval.kind == GML_V_REAL && pval.real == 5.0);

        /* Test string_last_pos */
        double lpos = gml_string_last_pos("la", "lalalalala");
        assert(lpos == 9.0);
    }

    /* 4. Test instance_deactivate_region and instance_activate_region */
    {
        gml_ast n_deact = { GML_AST_CALL, GML_T_NONE, 0, "instance_deactivate_region", NULL, NULL, 0, NULL };
        gml_ast a1 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast a2 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast a3 = { GML_AST_NUMBER, GML_T_NONE, 100, NULL, NULL, NULL, 0, NULL };
        gml_ast a4 = { GML_AST_NUMBER, GML_T_NONE, 100, NULL, NULL, NULL, 0, NULL };
        gml_ast a5 = { GML_AST_NUMBER, GML_T_NONE, 1, NULL, NULL, NULL, 0, NULL };
        gml_ast a6 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_d[6] = { &a1, &a2, &a3, &a4, &a5, &a6 };
        n_deact.count = 6;
        n_deact.items = items_d;

        gml_ast stmt_deact = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_deact, NULL, 0, NULL };
        int ok = gml_vm_execute(&vm, &stmt_deact);
        assert(ok);
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
