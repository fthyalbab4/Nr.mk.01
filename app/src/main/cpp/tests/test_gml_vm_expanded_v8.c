#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

int main(void) {
    printf("=== Testing GML VM Expanded V8 Suite ===\n");

    /* Test window fullscreen state functions */
    assert(gml_window_get_fullscreen() == 0.0);
    assert(gml_window_set_fullscreen(1.0) == 1.0);
    assert(gml_window_get_fullscreen() == 1.0);
    assert(gml_window_set_fullscreen(0.0) == 1.0);
    assert(gml_window_get_fullscreen() == 0.0);

    /* Test ds_grid_copy */
    double grid1 = gml_ds_grid_create(4, 4);
    double grid2 = gml_ds_grid_create(4, 4);
    gml_ds_grid_set(grid1, 1, 1, 99.0);
    assert(gml_ds_grid_get(grid2, 1, 1) == 0.0);
    assert(gml_ds_grid_copy(grid2, grid1) == 1.0);
    assert(gml_ds_grid_get(grid2, 1, 1) == 99.0);
    gml_ds_grid_destroy(grid1);
    gml_ds_grid_destroy(grid2);

    /* Test string width/height ext builtins */
    assert(gml_string_width_ext("Hello World", 10, 100) > 0);
    assert(gml_string_height_ext("Hello World", 10, 100) > 0);

    /* Test VM execution of new dispatches */
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test window_set_fullscreen and window_get_fullscreen via VM call */
    gml_ast arg_fs_val = { .kind = GML_AST_NUMBER, .number = 1.0 };
    gml_ast *items_fs_set[1] = { &arg_fs_val };
    gml_ast fn_fs_set = { .kind = GML_AST_CALL, .text = "window_set_fullscreen", .count = 1, .items = items_fs_set };
    assert(gml_vm_execute(&vm, &fn_fs_set));

    gml_ast fn_fs_get = { .kind = GML_AST_CALL, .text = "window_get_fullscreen", .count = 0, .items = NULL };
    assert(gml_vm_execute(&vm, &fn_fs_get));

    printf("  test_gml_vm_expanded_v8 PASS\n");
    printf("GML_VM_EXPANDED_V8_TEST_PASS\n");
    return 0;
}
