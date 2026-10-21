#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v17] Starting...\n");

    /* 1. Test ds_grid_add_region */
    double grid = gml_ds_grid_create(4, 4);
    assert(grid >= 0);
    gml_ds_grid_clear(grid, 5.0);
    gml_ds_grid_add_region(grid, 1, 1, 2, 2, 10.0);
    assert(gml_ds_grid_get(grid, 0, 0) == 5.0);
    assert(gml_ds_grid_get(grid, 1, 1) == 15.0);
    assert(gml_ds_grid_get(grid, 2, 2) == 15.0);
    assert(gml_ds_grid_get(grid, 3, 3) == 5.0);
    gml_ds_grid_destroy(grid);

    /* 2. Test ds_list_sort and ds_list_shuffle */
    double list = gml_ds_list_create();
    assert(list >= 0);
    gml_ds_list_add(list, 30.0);
    gml_ds_list_add(list, 10.0);
    gml_ds_list_add(list, 20.0);
    gml_ds_list_sort(list, 1.0); /* ascending */
    assert(gml_ds_list_find_value(list, 0) == 10.0);
    assert(gml_ds_list_find_value(list, 1) == 20.0);
    assert(gml_ds_list_find_value(list, 2) == 30.0);

    gml_ds_list_sort(list, 0.0); /* descending */
    assert(gml_ds_list_find_value(list, 0) == 30.0);
    assert(gml_ds_list_find_value(list, 1) == 20.0);
    assert(gml_ds_list_find_value(list, 2) == 10.0);

    gml_ds_list_shuffle(list);
    assert(gml_ds_list_size(list) == 3.0);
    gml_ds_list_destroy(list);

    /* 3. Test ds_map_find_first and ds_map_find_next */
    double map = gml_ds_map_create();
    assert(map >= 0);
    gml_ds_map_add(map, 100.0, 1.0);
    gml_ds_map_add(map, 200.0, 2.0);
    double k1 = gml_ds_map_find_first(map);
    assert(k1 == 100.0);
    double k2 = gml_ds_map_find_next(map, k1);
    assert(k2 == 200.0);
    gml_ds_map_destroy(map);

    /* 4. Test draw_ellipse & draw_ellipse_color */
    uint8_t buf[64 * 64 * 4] = {0};
    gm82_draw_set_target(buf, 64, 64);
    gml_draw_ellipse(10, 10, 30, 30, 0);
    gml_draw_ellipse_color(32, 32, 48, 48, 0xFF0000, 0x00FF00, 1);

    /* 5. Test AST VM execution for new built-ins */
    gml_vm vm;
    gml_vm_init(&vm);

    /* VM call to ds_list_create and ds_list_add */
    gml_ast call_list_create = { GML_AST_CALL, .text = "ds_list_create", .items = NULL, .count = 0 };
    gml_ast ret_create = { GML_AST_RETURN, .left = &call_list_create };
    int ok = gml_vm_execute(&vm, &ret_create);
    assert(ok == 1);
    double vm_list = vm.return_value.real;
    assert(vm_list >= 0);

    gml_ast arg_l_id = { GML_AST_NUMBER, .number = vm_list };
    gml_ast arg_l_val = { GML_AST_NUMBER, .number = 42.0 };
    const gml_ast *add_args[2] = { &arg_l_id, &arg_l_val };
    gml_ast call_list_add = { GML_AST_CALL, .text = "ds_list_add", .items = (gml_ast**)add_args, .count = 2 };
    gml_ast stmt_add = { GML_AST_EXPR_STMT, .left = &call_list_add };
    ok = gml_vm_execute(&vm, &stmt_add);
    assert(ok == 1);

    assert(gml_ds_list_find_value(vm_list, 0) == 42.0);
    gml_ds_list_destroy(vm_list);

    printf("[test_gml_vm_expanded_v17] PASS\n");
    return 0;
}
