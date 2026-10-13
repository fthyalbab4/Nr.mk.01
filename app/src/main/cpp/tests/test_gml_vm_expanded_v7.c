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

void test_gml_vm_expanded_v7(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test ds_grid_set_disk C builtin */
    double grid_id = gml_ds_grid_create(7, 7);
    assert(grid_id >= 0);
    gml_ds_grid_clear(grid_id, 0);

    /* Center (3,3), radius 1 disk -> 5 cells (center + 4 cardinal) set to 10 */
    gml_ds_grid_set_disk(grid_id, 3, 3, 1, 10);
    double sum1 = gml_ds_grid_get_sum(grid_id, 0, 0, 6, 6);
    assert(fabs(sum1 - 50.0) < 0.0001);

    /* Add 5 to disk -> sum should increase by 5 * 5 = 25 -> 75 */
    gml_ds_grid_add_disk(grid_id, 3, 3, 1, 5);
    double sum2 = gml_ds_grid_get_sum(grid_id, 0, 0, 6, 6);
    assert(fabs(sum2 - 75.0) < 0.0001);

    /* Test via AST VM execution dispatch */
    gml_ast n_create = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_create", NULL, NULL, 0, NULL };
    gml_ast c_w = { GML_AST_NUMBER, GML_T_NONE, 5, NULL, NULL, NULL, 0, NULL };
    gml_ast c_h = { GML_AST_NUMBER, GML_T_NONE, 5, NULL, NULL, NULL, 0, NULL };
    gml_ast *c_args[2] = { &c_w, &c_h };
    n_create.count = 2;
    n_create.items = c_args;

    gml_ast assign_g = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_g = { GML_AST_NAME, GML_T_NONE, 0, "gd", NULL, NULL, 0, NULL };
    assign_g.left = &var_g;
    assign_g.right = &n_create;
    gml_ast stmt1 = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_g, NULL, 0, NULL };
    gml_vm_execute(&vm, &stmt1);

    /* ds_grid_set_disk(gd, 2, 2, 1, 100) -> 5 cells * 100 = 500 */
    gml_ast n_set_disk = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_set_disk", NULL, NULL, 0, NULL };
    gml_ast a_gd = { GML_AST_NAME, GML_T_NONE, 0, "gd", NULL, NULL, 0, NULL };
    gml_ast a_2a = { GML_AST_NUMBER, GML_T_NONE, 2, NULL, NULL, NULL, 0, NULL };
    gml_ast a_2b = { GML_AST_NUMBER, GML_T_NONE, 2, NULL, NULL, NULL, 0, NULL };
    gml_ast a_1 = { GML_AST_NUMBER, GML_T_NONE, 1, NULL, NULL, NULL, 0, NULL };
    gml_ast a_100 = { GML_AST_NUMBER, GML_T_NONE, 100, NULL, NULL, NULL, 0, NULL };
    gml_ast *sd_args[5] = { &a_gd, &a_2a, &a_2b, &a_1, &a_100 };
    n_set_disk.count = 5;
    n_set_disk.items = sd_args;

    gml_ast stmt2 = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_set_disk, NULL, 0, NULL };
    int ok = gml_vm_execute(&vm, &stmt2);
    assert(ok);

    /* Check sum via AST VM */
    gml_ast n_sum = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_get_sum", NULL, NULL, 0, NULL };
    gml_ast a_0a = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast a_0b = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast a_4a = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
    gml_ast a_4b = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
    gml_ast *sum_args[5] = { &a_gd, &a_0a, &a_0b, &a_4a, &a_4b };
    n_sum.count = 5;
    n_sum.items = sum_args;

    gml_ast assign_res = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_res = { GML_AST_NAME, GML_T_NONE, 0, "disk_sum", NULL, NULL, 0, NULL };
    assign_res.left = &var_res;
    assign_res.right = &n_sum;
    gml_ast stmt3 = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_res, NULL, 0, NULL };
    gml_vm_execute(&vm, &stmt3);

    gml_value res = gml_vm_get(&vm, "disk_sum");
    assert(res.kind == GML_V_REAL && fabs(res.real - 500.0) < 0.0001);

    gml_ds_grid_destroy(grid_id);
    printf("  test_gml_vm_expanded_v7 PASS\n");
}

#ifndef RUNNING_FULL_SUITE
int main(void) {
    printf("=== Testing GML VM Expanded V7 Suite ===\n");
    test_gml_vm_expanded_v7();
    printf("GML_VM_EXPANDED_V7_TEST_PASS\n");
    return 0;
}
#endif
