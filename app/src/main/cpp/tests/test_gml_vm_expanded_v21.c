#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v21] Starting...\n");

    /* 1. Direct function tests for ds_grid disk value searches */
    double grid = gml_ds_grid_create(5, 5);
    assert(grid >= 0.0);
    gml_ds_grid_clear(grid, 0.0);
    gml_ds_grid_set(grid, 2, 2, 42.0);

    assert(gml_ds_grid_value_disk_exists(grid, 2, 2, 1, 42.0) == 1.0);
    assert(gml_ds_grid_value_disk_x(grid, 2, 2, 1, 42.0) == 2.0);
    assert(gml_ds_grid_value_disk_y(grid, 2, 2, 1, 42.0) == 2.0);

    assert(gml_ds_grid_value_disk_exists(grid, 0, 0, 1, 42.0) == 0.0);
    assert(gml_ds_grid_value_disk_x(grid, 0, 0, 1, 42.0) == -1.0);
    assert(gml_ds_grid_value_disk_y(grid, 0, 0, 1, 42.0) == -1.0);
    gml_ds_grid_destroy(grid);

    /* 2. Direct function tests for framebuffer pixel reading (draw_getpixel / draw_getpixel_ext) */
    uint8_t buffer[10 * 10 * 4];
    memset(buffer, 0, sizeof(buffer));
    gm82_draw_set_target(buffer, 10, 10);
    gml_draw_set_color(0x00FF88); /* R=0x88, G=0xFF, B=0x00 (RGB integer 0x00FF88) */
    gml_draw_set_alpha(1.0);
    gml_draw_rectangle(1, 1, 3, 3, 0);

    double pix = gml_draw_getpixel(2, 2);
    assert(pix == 0x00FF88);

    double pix_ext = gml_draw_getpixel_ext(2, 2);
    assert(((uint32_t)pix_ext & 0xFFFFFF) == 0x00FF88);

    /* 3. Direct function tests for date utility functions */
    double dt_2024 = 1704067200.0; /* Jan 1 2024 */
    assert(gml_date_is_leap_year(dt_2024) == 1.0);
    assert(gml_date_days_in_year(dt_2024) == 366.0);

    double dt_2023 = 1672531200.0; /* Jan 1 2023 */
    assert(gml_date_is_leap_year(dt_2023) == 0.0);
    assert(gml_date_days_in_year(dt_2023) == 365.0);

    /* 4. AST VM Execution tests */
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test AST call: draw_getpixel(2, 2) */
    gml_ast arg_x = { GML_AST_NUMBER, .number = 2.0 };
    gml_ast arg_y = { GML_AST_NUMBER, .number = 2.0 };
    const gml_ast *pix_args[2] = { &arg_x, &arg_y };
    gml_ast call_getpix = { GML_AST_CALL, .text = "draw_getpixel", .items = (gml_ast**)pix_args, .count = 2 };
    gml_ast ret_getpix = { GML_AST_RETURN, .left = &call_getpix };

    int ok = gml_vm_execute(&vm, &ret_getpix);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 0x00FF88);

    /* Test AST call: date_is_leap_year(dt_2024) */
    gml_ast arg_dt = { GML_AST_NUMBER, .number = dt_2024 };
    const gml_ast *leap_args[1] = { &arg_dt };
    gml_ast call_leap = { GML_AST_CALL, .text = "date_is_leap_year", .items = (gml_ast**)leap_args, .count = 1 };
    gml_ast ret_leap = { GML_AST_RETURN, .left = &call_leap };

    ok = gml_vm_execute(&vm, &ret_leap);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 1.0);

    printf("[test_gml_vm_expanded_v21] PASS\n");
    return 0;
}
