#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v20] Starting...\n");

    /* 1. Direct function checks for ds_grid max/min/sum */
    double grid = gml_ds_grid_create(3, 3);
    assert(grid >= 0.0);
    gml_ds_grid_set(grid, 0, 0, 5.0);
    gml_ds_grid_set(grid, 1, 0, 15.0);
    gml_ds_grid_set(grid, 2, 0, 25.0);

    assert(gml_ds_grid_get_sum(grid, 0, 0, 2, 0) == 45.0);
    assert(gml_ds_grid_get_max(grid, 0, 0, 2, 0) == 25.0);
    assert(gml_ds_grid_get_min(grid, 0, 0, 2, 0) == 5.0);
    gml_ds_grid_destroy(grid);

    /* 2. String bytes and Window caption checks */
    const char *test_str = "Hello GM82";
    assert(gml_string_byte_length(test_str) == 10.0);
    assert(gml_string_byte_at(test_str, 1) == 'H');
    assert(gml_string_byte_at(test_str, 7) == 'G');

    gml_window_set_caption("GM82 Android Test");
    assert(strcmp(gml_window_get_caption(), "GM82 Android Test") == 0);

    assert(gml_display_get_width() > 0.0);
    assert(gml_display_get_height() > 0.0);

    /* 3. AST VM Execution checks for string_byte_length, ds_grid_get_sum, and window_get_caption */
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test AST string_byte_length("GM82") -> 4.0 */
    gml_ast str_node = { GML_AST_STRING, .text = "GM82" };
    const gml_ast *str_args[1] = { &str_node };
    gml_ast call_byte_len = { GML_AST_CALL, .text = "string_byte_length", .items = (gml_ast**)str_args, .count = 1 };
    gml_ast ret_byte_len = { GML_AST_RETURN, .left = &call_byte_len };

    int ok = gml_vm_execute(&vm, &ret_byte_len);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 4.0);

    /* Test AST window_get_caption() -> "GM82 Android Test" */
    gml_ast call_get_cap = { GML_AST_CALL, .text = "window_get_caption", .items = NULL, .count = 0 };
    gml_ast ret_get_cap = { GML_AST_RETURN, .left = &call_get_cap };

    ok = gml_vm_execute(&vm, &ret_get_cap);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_STRING);
    assert(vm.return_value.string != NULL);
    assert(strcmp(vm.return_value.string, "GM82 Android Test") == 0);

    printf("[test_gml_vm_expanded_v20] PASS\n");
    return 0;
}
