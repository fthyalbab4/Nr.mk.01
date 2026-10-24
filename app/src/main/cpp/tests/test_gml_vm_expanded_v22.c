#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v22] Starting...\n");

    /* 1. Direct function tests for ds_grid_get_disk_* */
    double grid = gml_ds_grid_create(5, 5);
    assert(grid >= 0.0);
    gml_ds_grid_clear(grid, 0.0);
    gml_ds_grid_set(grid, 2, 2, 10.0);
    gml_ds_grid_set(grid, 2, 1, 20.0);
    gml_ds_grid_set(grid, 3, 2, 30.0);

    /* Disk at (2,2) with radius 1 covers (2,2)=10, (2,1)=20, (2,3)=0, (1,2)=0, (3,2)=30 => total 5 cells */
    double disk_sum = gml_ds_grid_get_disk_sum(grid, 2, 2, 1);
    assert(disk_sum == 60.0);

    double disk_max = gml_ds_grid_get_disk_max(grid, 2, 2, 1);
    assert(disk_max == 30.0);

    double disk_min = gml_ds_grid_get_disk_min(grid, 2, 2, 1);
    assert(disk_min == 0.0);

    double disk_mean = gml_ds_grid_get_disk_mean(grid, 2, 2, 1);
    assert(fabs(disk_mean - 12.0) < 0.0001);

    gml_ds_grid_destroy(grid);

    /* 2. Direct function tests for date compare and spans */
    double d1 = 1704067200.0; /* 2024-01-01 00:00:00 */
    double d2 = 1704153600.0; /* 2024-01-02 00:00:00 (1 day = 86400s later) */

    assert(gml_date_compare_datetime(d1, d2) == -1.0);
    assert(gml_date_compare_datetime(d2, d1) == 1.0);
    assert(gml_date_compare_datetime(d1, d1) == 0.0);

    assert(gml_date_day_span(d1, d2) == 1.0);
    assert(gml_date_hour_span(d1, d2) == 24.0);
    assert(gml_date_second_span(d1, d2) == 86400.0);

    /* 3. Direct function tests for vector dot_product_normal and 3d_normal */
    double dp_norm = gml_dot_product_normal(3.0, 0.0, 5.0, 0.0);
    assert(fabs(dp_norm - 1.0) < 0.0001);

    double dp3d_norm = gml_dot_product_3d_normal(0.0, 10.0, 0.0, 0.0, -2.0, 0.0);
    assert(fabs(dp3d_norm - (-1.0)) < 0.0001);

    /* 4. AST VM Execution tests */
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test AST call: date_day_span(d1, d2) */
    gml_ast arg_d1 = { GML_AST_NUMBER, .number = d1 };
    gml_ast arg_d2 = { GML_AST_NUMBER, .number = d2 };
    const gml_ast *span_args[2] = { &arg_d1, &arg_d2 };
    gml_ast call_span = { GML_AST_CALL, .text = "date_day_span", .items = (gml_ast**)span_args, .count = 2 };
    gml_ast ret_span = { GML_AST_RETURN, .left = &call_span };

    int ok = gml_vm_execute(&vm, &ret_span);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 1.0);

    /* Test AST call: dot_product_normal(3, 0, 5, 0) */
    gml_ast arg_x1 = { GML_AST_NUMBER, .number = 3.0 };
    gml_ast arg_y1 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast arg_x2 = { GML_AST_NUMBER, .number = 5.0 };
    gml_ast arg_y2 = { GML_AST_NUMBER, .number = 0.0 };
    const gml_ast *dp_args[4] = { &arg_x1, &arg_y1, &arg_x2, &arg_y2 };
    gml_ast call_dp = { GML_AST_CALL, .text = "dot_product_normal", .items = (gml_ast**)dp_args, .count = 4 };
    gml_ast ret_dp = { GML_AST_RETURN, .left = &call_dp };

    ok = gml_vm_execute(&vm, &ret_dp);
    if (!ok) { printf("VM Error: %s\n", vm.error); fflush(stdout); }
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(fabs(vm.return_value.real - 1.0) < 0.0001);

    printf("[test_gml_vm_expanded_v22] PASS\n");
    return 0;
}
