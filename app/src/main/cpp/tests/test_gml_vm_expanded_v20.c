#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

int main(void) {
    printf("=== Test GML VM Expanded v20 (Newly Wired Builtins) ===\n");

    /* Initialize runtime and instance */
    gm82_runtime rt;
    memset(&rt, 0, sizeof(rt));
    rt.room_speed = 30;
    gm82_gml_set_runtime(&rt);

    gm82_instance inst;
    memset(&inst, 0, sizeof(inst));
    inst.x = 100.0;
    inst.y = 200.0;
    inst.hspeed = 4.0;
    inst.vspeed = 3.0;
    inst.direction = 45.0;
    inst.speed = 5.0;
    inst.sprite_index = 2.0;
    inst.image_index = 1.0;
    inst.visible = 1.0;
    inst.solid = 1.0;
    gm82_gml_set_self(&inst);

    /* Test 1: Direct builtin calls for newly wired functions */
    assert(gml_get_x() == 100.0);
    assert(gml_get_y() == 200.0);
    gml_set_x(150.0);
    gml_set_y(250.0);
    assert(gml_get_x() == 150.0);
    assert(gml_get_y() == 250.0);

    gml_set_score(500.0);
    assert(gml_get_score() == 500.0);
    gml_set_lives(5.0);
    assert(gml_get_lives() == 5.0);
    gml_set_health(80.0);
    assert(gml_get_health() == 80.0);

    gml_window_set_caption("NOR Maker GM82 Test");
    assert(strcmp(gml_window_get_caption(), "NOR Maker GM82 Test") == 0);

    /* Test 2: AST VM execution of newly wired builtins */
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test AST return window_get_caption() */
    gml_ast ast_caption = { .kind = GML_AST_CALL, .text = "window_get_caption", .items = NULL, .count = 0 };
    gml_ast ret_caption = { .kind = GML_AST_RETURN, .left = &ast_caption };
    assert(gml_vm_execute(&vm, &ret_caption));
    assert(vm.return_value.kind == GML_V_STRING);
    assert(strcmp(vm.return_value.string, "NOR Maker GM82 Test") == 0);

    /* Test AST return room_speed() */
    gml_ast ast_rs = { .kind = GML_AST_CALL, .text = "room_speed", .items = NULL, .count = 0 };
    gml_ast ret_rs = { .kind = GML_AST_RETURN, .left = &ast_rs };
    assert(gml_vm_execute(&vm, &ret_rs));
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 30.0);

    printf("PASS: test_gml_vm_expanded_v20 verified successfully!\n");
    return 0;
}
