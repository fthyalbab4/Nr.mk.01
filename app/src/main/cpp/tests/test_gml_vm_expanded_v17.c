#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v17] Starting...\n");

    /* 1. Setup soft render target */
    uint8_t buffer[64 * 64 * 4];
    memset(buffer, 0, sizeof(buffer));
    gm82_draw_set_target(buffer, 64, 64);

    /* 2. Test draw_set_halign and draw_set_valign */
    gml_draw_set_halign(1.0);
    assert(gml_draw_get_halign() == 1.0);

    gml_draw_set_valign(2.0);
    assert(gml_draw_get_valign() == 2.0);

    /* 3. Test gml_draw_triangle and gml_draw_ellipse direct calls */
    gml_draw_set_color(0x0000FF); /* Blue */
    gml_draw_set_alpha(1.0);
    gml_draw_triangle(10.0, 10.0, 20.0, 10.0, 15.0, 20.0, 0.0);

    gml_draw_set_color(0x00FF00); /* Green */
    gml_draw_ellipse(30.0, 30.0, 50.0, 50.0, 0.0);

    /* 4. Test AST VM invocation for draw_triangle and alignment functions */
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast arg_halign = { GML_AST_NUMBER, .number = 0.0 };
    const gml_ast *halign_args[1] = { &arg_halign };
    gml_ast call_halign = {
        GML_AST_CALL,
        .text = "draw_set_halign",
        .items = (gml_ast**)halign_args,
        .count = 1
    };
    gml_ast stmt_halign = { GML_AST_EXPR_STMT, .left = &call_halign };
    int ok = gml_vm_execute(&vm, &stmt_halign);
    assert(ok == 1);
    assert(gml_draw_get_halign() == 0.0);

    /* Call draw_get_halign via AST VM */
    gml_ast call_get_halign = {
        GML_AST_CALL,
        .text = "draw_get_halign",
        .items = NULL,
        .count = 0
    };
    gml_ast ret_get_halign = { GML_AST_RETURN, .left = &call_get_halign };
    ok = gml_vm_execute(&vm, &ret_get_halign);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 0.0);

    printf("[test_gml_vm_expanded_v17] PASS\n");
    return 0;
}
