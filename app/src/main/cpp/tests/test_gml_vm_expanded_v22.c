#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v22] Starting...\n");

    /* 1. Direct function tests for ds_grid disk operations:
       multiply_disk, get_disk_min, get_disk_max, get_disk_sum, get_disk_mean */
    double grid = gml_ds_grid_create(5, 5);
    assert(grid >= 0.0);
    gml_ds_grid_clear(grid, 10.0);

    /* Center (2, 2), radius 1: cells in disk = (2,2), (1,2), (3,2), (2,1), (2,3) -> 5 cells */
    /* Multiply disk by 2.0 -> these 5 cells become 20.0, rest stay 10.0 */
    double res_mult = gml_ds_grid_multiply_disk(grid, 2, 2, 1, 2.0);
    assert(res_mult == 1.0);

    assert(gml_ds_grid_get(grid, 2, 2) == 20.0);
    assert(gml_ds_grid_get(grid, 1, 2) == 20.0);
    assert(gml_ds_grid_get(grid, 0, 0) == 10.0);

    /* Disk stats */
    double disk_min = gml_ds_grid_get_disk_min(grid, 2, 2, 1);
    assert(disk_min == 20.0);

    double disk_max = gml_ds_grid_get_disk_max(grid, 2, 2, 1);
    assert(disk_max == 20.0);

    double disk_sum = gml_ds_grid_get_disk_sum(grid, 2, 2, 1);
    assert(disk_sum == 100.0); /* 5 * 20.0 */

    double disk_mean = gml_ds_grid_get_disk_mean(grid, 2, 2, 1);
    assert(disk_mean == 20.0);

    /* Larger disk (radius 2 around center) including 10.0 and 20.0 cells */
    double global_min = gml_ds_grid_get_disk_min(grid, 2, 2, 2);
    assert(global_min == 10.0);

    double global_max = gml_ds_grid_get_disk_max(grid, 2, 2, 2);
    assert(global_max == 20.0);

    gml_ds_grid_destroy(grid);

    /* 2. AST VM Execution tests */
    gml_vm vm;
    gml_vm_init(&vm);

    double grid_vm = gml_ds_grid_create(5, 5);
    gml_ds_grid_clear(grid_vm, 5.0);

    /* AST Call: ds_grid_multiply_disk(grid_vm, 2, 2, 1, 3.0) */
    gml_ast arg_id = { GML_AST_NUMBER, .number = grid_vm };
    gml_ast arg_x = { GML_AST_NUMBER, .number = 2.0 };
    gml_ast arg_y = { GML_AST_NUMBER, .number = 2.0 };
    gml_ast arg_r = { GML_AST_NUMBER, .number = 1.0 };
    gml_ast arg_v = { GML_AST_NUMBER, .number = 3.0 };
    const gml_ast *mult_args[5] = { &arg_id, &arg_x, &arg_y, &arg_r, &arg_v };
    gml_ast call_mult = { GML_AST_CALL, .text = "ds_grid_multiply_disk", .items = (gml_ast**)mult_args, .count = 5 };
    gml_ast ret_mult = { GML_AST_RETURN, .left = &call_mult };

    int ok = gml_vm_execute(&vm, &ret_mult);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 1.0);
    assert(gml_ds_grid_get(grid_vm, 2, 2) == 15.0);

    /* AST Call: ds_grid_get_disk_sum(grid_vm, 2, 2, 1) -> 5 * 15.0 = 75.0 */
    const gml_ast *sum_args[4] = { &arg_id, &arg_x, &arg_y, &arg_r };
    gml_ast call_sum = { GML_AST_CALL, .text = "ds_grid_get_disk_sum", .items = (gml_ast**)sum_args, .count = 4 };
    gml_ast ret_sum = { GML_AST_RETURN, .left = &call_sum };

    ok = gml_vm_execute(&vm, &ret_sum);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 75.0);

    gml_ds_grid_destroy(grid_vm);

    printf("[test_gml_vm_expanded_v22] PASS\n");
    return 0;
}
