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
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

void test_gml_vm_expanded_v4(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test 3D math: point_distance_3d and dot_product_3d */
    {
        double dist = gml_point_distance_3d(0, 0, 0, 3, 4, 12);
        assert(fabs(dist - 13.0) < 0.0001);

        double dot = gml_dot_product_3d(1, 2, 3, 4, 5, 6);
        assert(fabs(dot - 32.0) < 0.0001);

        /* AST VM evaluation for point_distance_3d */
        gml_ast n_dist = { GML_AST_CALL, GML_T_NONE, 0, "point_distance_3d", NULL, NULL, 0, NULL };
        gml_ast a0 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast a1 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast a2 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast a3 = { GML_AST_NUMBER, GML_T_NONE, 3, NULL, NULL, NULL, 0, NULL };
        gml_ast a4 = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
        gml_ast a5 = { GML_AST_NUMBER, GML_T_NONE, 12, NULL, NULL, NULL, 0, NULL };
        gml_ast *items[6] = { &a0, &a1, &a2, &a3, &a4, &a5 };
        n_dist.count = 6;
        n_dist.items = items;

        gml_ast assign = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var = { GML_AST_NAME, GML_T_NONE, 0, "d3d", NULL, NULL, 0, NULL };
        assign.left = &var;
        assign.right = &n_dist;
        gml_ast stmt = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt);
        assert(ok);
        gml_value res = gml_vm_get(&vm, "d3d");
        assert(res.kind == GML_V_REAL && fabs(res.real - 13.0) < 0.0001);
    }

    /* Test ds_grid stats and math ops: add, multiply, max, min */
    {
        double grid_id = gml_ds_grid_create(3, 3);
        assert(grid_id >= 0);

        gml_ds_grid_set(grid_id, 0, 0, 10);
        gml_ds_grid_set(grid_id, 1, 1, 50);
        gml_ds_grid_set(grid_id, 2, 2, 25);

        gml_ds_grid_add(grid_id, 0, 0, 5);
        assert(fabs(gml_ds_grid_get(grid_id, 0, 0) - 15.0) < 0.0001);

        gml_ds_grid_multiply(grid_id, 2, 2, 2);
        assert(fabs(gml_ds_grid_get(grid_id, 2, 2) - 50.0) < 0.0001);

        double maxv = gml_ds_grid_get_max(grid_id, 0, 0, 2, 2);
        assert(fabs(maxv - 50.0) < 0.0001);

        double minv = gml_ds_grid_get_min(grid_id, 0, 0, 2, 2);
        assert(fabs(minv - 0.0) < 0.0001);

        gml_ds_grid_destroy(grid_id);
    }

    /* Test ds_list_insert and ds_list_replace */
    {
        double lid = gml_ds_list_create();
        gml_ds_list_add(lid, 10);
        gml_ds_list_add(lid, 30);
        gml_ds_list_insert(lid, 1, 20);
        assert(gml_ds_list_size(lid) == 3);
        assert(gml_ds_list_find_value(lid, 0) == 10);
        assert(gml_ds_list_find_value(lid, 1) == 20);
        assert(gml_ds_list_find_value(lid, 2) == 30);

        gml_ds_list_replace(lid, 1, 25);
        assert(gml_ds_list_find_value(lid, 1) == 25);
        gml_ds_list_destroy(lid);
    }

    /* Test string_trim and instance activation */
    {
        char out[128];
        gml_string_trim("  hello world  \t", out, sizeof(out));
        assert(strcmp(out, "hello world") == 0);

        assert(gml_instance_deactivate_object(0) == 1.0);
        assert(gml_instance_activate_object(0) == 1.0);
    }

    printf("  test_gml_vm_expanded_v4 PASS\n");
}

#ifndef RUNNING_FULL_SUITE
int main(void) {
    printf("=== Testing GML VM Expanded V4 Suite ===\n");
    test_gml_vm_expanded_v4();
    printf("GML_VM_EXPANDED_V4_TEST_PASS\n");
    return 0;
}
#endif
