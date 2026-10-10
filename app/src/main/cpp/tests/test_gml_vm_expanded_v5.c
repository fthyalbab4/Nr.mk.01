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

    /* Test ds_grid_set_region and ds_grid_add_region C builtins */
    double grid_id = gml_ds_grid_create(5, 5);
    assert(grid_id >= 0);

    gml_ds_grid_clear(grid_id, 0);
    gml_ds_grid_set_region(grid_id, 1, 1, 3, 3, 10);
    gml_ds_grid_add_region(grid_id, 2, 2, 4, 4, 5);

    double sum = gml_ds_grid_get_sum(grid_id, 0, 0, 4, 4);
    assert(fabs(sum - 135.0) < 0.0001);

    /* Test AST VM execution dispatch for ds_grid_set_region and ds_grid_add_region */
    gml_ast n_create = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_create", NULL, NULL, 0, NULL };
    gml_ast c_w = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
    gml_ast c_h = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
    gml_ast *c_args[2] = { &c_w, &c_h };
    n_create.count = 2;
    n_create.items = c_args;

    gml_ast assign_g = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_g = { GML_AST_NAME, GML_T_NONE, 0, "g2", NULL, NULL, 0, NULL };
    assign_g.left = &var_g;
    assign_g.right = &n_create;
    gml_ast stmt1 = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_g, NULL, 0, NULL };
    gml_vm_execute(&vm, &stmt1);

    /* ds_grid_set_region(g2, 0, 0, 1, 1, 100) */
    gml_ast n_set_reg = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_set_region", NULL, NULL, 0, NULL };
    gml_ast a_g2 = { GML_AST_NAME, GML_T_NONE, 0, "g2", NULL, NULL, 0, NULL };
    gml_ast a_0 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast a_0b = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast a_1 = { GML_AST_NUMBER, GML_T_NONE, 1, NULL, NULL, NULL, 0, NULL };
    gml_ast a_1b = { GML_AST_NUMBER, GML_T_NONE, 1, NULL, NULL, NULL, 0, NULL };
    gml_ast a_100 = { GML_AST_NUMBER, GML_T_NONE, 100, NULL, NULL, NULL, 0, NULL };
    gml_ast *sr_args[6] = { &a_g2, &a_0, &a_0b, &a_1, &a_1b, &a_100 };
    n_set_reg.count = 6;
    n_set_reg.items = sr_args;

    gml_ast stmt2 = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_set_reg, NULL, 0, NULL };
    int ok = gml_vm_execute(&vm, &stmt2);
    assert(ok);

    /* ds_grid_get_sum(g2, 0, 0, 3, 3) -> 4 * 100 = 400 */
    gml_ast n_sum = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_get_sum", NULL, NULL, 0, NULL };
    gml_ast a_3 = { GML_AST_NUMBER, GML_T_NONE, 3, NULL, NULL, NULL, 0, NULL };
    gml_ast a_3b = { GML_AST_NUMBER, GML_T_NONE, 3, NULL, NULL, NULL, 0, NULL };
    gml_ast *sum_args[5] = { &a_g2, &a_0, &a_0b, &a_3, &a_3b };
    n_sum.count = 5;
    n_sum.items = sum_args;

    gml_ast assign_res = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_res = { GML_AST_NAME, GML_T_NONE, 0, "total_sum", NULL, NULL, 0, NULL };
    assign_res.left = &var_res;
    assign_res.right = &n_sum;
    gml_ast stmt3 = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_res, NULL, 0, NULL };
    gml_vm_execute(&vm, &stmt3);

    gml_value res = gml_vm_get(&vm, "total_sum");
    assert(res.kind == GML_V_REAL && fabs(res.real - 400.0) < 0.0001);

    gml_ds_grid_destroy(grid_id);
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
