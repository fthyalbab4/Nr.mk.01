#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v21] Starting...\n");

    /* 1. Direct function checks for window position & size */
    assert(gml_window_set_position(100.0, 200.0) == 1.0);
    assert(gml_window_get_x() == 100.0);
    assert(gml_window_get_y() == 200.0);

    assert(gml_window_set_size(800.0, 600.0) == 1.0);
    assert(gml_window_get_width() == 800.0);
    assert(gml_window_get_height() == 600.0);

    /* 2. Direct clipboard checks */
    assert(gml_clipboard_has_text() == 0.0);
    assert(gml_clipboard_set_text("GM82 Clipboard Content") == 1.0);
    assert(gml_clipboard_has_text() == 1.0);

    char clip_out[256];
    assert(gml_clipboard_get_text(clip_out, sizeof(clip_out)) == 1.0);
    assert(strcmp(clip_out, "GM82 Clipboard Content") == 0);

    /* 3. Parameter and environment variable checks */
    assert(gml_parameter_count() == 0.0);
    char param_out[128];
    assert(gml_parameter_string(0, param_out, sizeof(param_out)) == 0.0);
    assert(param_out[0] == '\0');

    char env_out[256];
    gml_environment_get_variable("PATH", env_out, sizeof(env_out));

    /* 4. AST VM Execution checks */
    gml_vm vm;
    gml_vm_init(&vm);

    /* AST Test: window_get_x() -> 100.0 */
    gml_ast call_win_x = { GML_AST_CALL, .text = "window_get_x", .items = NULL, .count = 0 };
    gml_ast ret_win_x = { GML_AST_RETURN, .left = &call_win_x };

    int ok = gml_vm_execute(&vm, &ret_win_x);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 100.0);

    /* AST Test: clipboard_has_text() -> 1.0 */
    gml_ast call_clip_has = { GML_AST_CALL, .text = "clipboard_has_text", .items = NULL, .count = 0 };
    gml_ast ret_clip_has = { GML_AST_RETURN, .left = &call_clip_has };

    ok = gml_vm_execute(&vm, &ret_clip_has);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 1.0);

    /* AST Test: clipboard_get_text() -> "GM82 Clipboard Content" */
    gml_ast call_clip_get = { GML_AST_CALL, .text = "clipboard_get_text", .items = NULL, .count = 0 };
    gml_ast ret_clip_get = { GML_AST_RETURN, .left = &call_clip_get };

    ok = gml_vm_execute(&vm, &ret_clip_get);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_STRING);
    assert(vm.return_value.string != NULL);
    assert(strcmp(vm.return_value.string, "GM82 Clipboard Content") == 0);

    printf("[test_gml_vm_expanded_v21] PASS\n");
    return 0;
}
