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

void test_gml_vm_expanded_v6(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test ds_grid_set_disk & ds_grid_add_disk */
    {
        double grid_id = gml_ds_grid_create(5, 5);
        assert(grid_id >= 0);

        /* Set disk with center (2,2) and radius 1 */
        gml_ds_grid_set_disk(grid_id, 2, 2, 1, 10.0);
        assert(fabs(gml_ds_grid_get(grid_id, 2, 2) - 10.0) < 0.0001);
        assert(fabs(gml_ds_grid_get(grid_id, 2, 1) - 10.0) < 0.0001);
        assert(fabs(gml_ds_grid_get(grid_id, 1, 2) - 10.0) < 0.0001);
        assert(fabs(gml_ds_grid_get(grid_id, 0, 0) - 0.0) < 0.0001);

        /* Add disk */
        gml_ds_grid_add_disk(grid_id, 2, 2, 1, 5.0);
        assert(fabs(gml_ds_grid_get(grid_id, 2, 2) - 15.0) < 0.0001);

        gml_ds_grid_destroy(grid_id);
    }

    /* Test instance_activate_region & instance_deactivate_region */
    {
        assert(gml_instance_activate_region(0, 0, 100, 100, 1) == 1.0);
        assert(gml_instance_deactivate_region(0, 0, 100, 100, 1, 0) == 1.0);
    }

    /* Test display_get_width & window_get_width builtins */
    {
        assert(gml_display_get_width() > 0);
        assert(gml_window_get_width() > 0);
    }

    /* Test AST VM evaluation for ds_grid_set_disk */
    {
        gml_ast n_create = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_create", NULL, NULL, 0, NULL };
        gml_ast a0 = { GML_AST_NUMBER, GML_T_NONE, 5.0, NULL, NULL, NULL, 0, NULL };
        gml_ast a1 = { GML_AST_NUMBER, GML_T_NONE, 5.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_c[2] = { &a0, &a1 };
        n_create.count = 2;
        n_create.items = items_c;

        gml_ast assign_g = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_g = { GML_AST_NAME, GML_T_NONE, 0, "g_id", NULL, NULL, 0, NULL };
        assign_g.left = &var_g;
        assign_g.right = &n_create;
        gml_ast stmt_g = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_g, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt_g);
        assert(ok);

        gml_value g_id_val = gml_vm_get(&vm, "g_id");
        assert(g_id_val.kind == GML_V_REAL);

        /* Call ds_grid_set_disk in VM */
        gml_ast n_set_disk = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_set_disk", NULL, NULL, 0, NULL };
        gml_ast p_gid = { GML_AST_NUMBER, GML_T_NONE, g_id_val.real, NULL, NULL, NULL, 0, NULL };
        gml_ast p_cx = { GML_AST_NUMBER, GML_T_NONE, 2.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_cy = { GML_AST_NUMBER, GML_T_NONE, 2.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_r = { GML_AST_NUMBER, GML_T_NONE, 1.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_v = { GML_AST_NUMBER, GML_T_NONE, 99.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_sd[5] = { &p_gid, &p_cx, &p_cy, &p_r, &p_v };
        n_set_disk.count = 5;
        n_set_disk.items = items_sd;

        gml_ast assign_sd = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_sd = { GML_AST_NAME, GML_T_NONE, 0, "sd_res", NULL, NULL, 0, NULL };
        assign_sd.left = &var_sd;
        assign_sd.right = &n_set_disk;
        gml_ast stmt_sd = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_sd, NULL, 0, NULL };

        ok = gml_vm_execute(&vm, &stmt_sd);
        assert(ok);

        gml_value sd_res = gml_vm_get(&vm, "sd_res");
        assert(sd_res.kind == GML_V_REAL && sd_res.real == 1.0);
        assert(fabs(gml_ds_grid_get(g_id_val.real, 2, 2) - 99.0) < 0.0001);

        gml_ds_grid_destroy(g_id_val.real);
    }

    printf("  test_gml_vm_expanded_v6 PASS\n");
}

#ifndef RUNNING_FULL_SUITE
int main(void) {
    printf("=== Testing GML VM Expanded V6 Suite ===\n");
    test_gml_vm_expanded_v6();
    printf("GML_VM_EXPANDED_V6_TEST_PASS\n");
    return 0;
}
#endif
