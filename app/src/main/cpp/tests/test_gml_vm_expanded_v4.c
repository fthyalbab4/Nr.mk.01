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

static void test_vm_keyboard_and_sound(void) {
    gm82_input_state input;
    gm82_input_init(&input);
    gm82_input_bind_global(&input);

    gm82_input_key_down(&input, 37); /* VK_LEFT */
    assert(gml_keyboard_check(37) == 1.0);
    assert(gml_keyboard_check_direct(37) == 1.0);

    gml_keyboard_clear(37);
    assert(gml_keyboard_check(37) == 0.0);

    gm82_input_key_down(&input, 39); /* VK_RIGHT */
    gml_io_clear();
    assert(gml_keyboard_check(39) == 0.0);

    gm82_sound_runtime sr;
    gm82_sound_runtime_init(&sr);
    gm82_decoded_sound_list sounds;
    memset(&sounds, 0, sizeof(sounds));
    sounds.count = 1;
    sounds.items = (gm82_decoded_sound *)calloc(1, sizeof(gm82_decoded_sound));
    strcpy(sounds.items[0].name, "snd_test");
    sounds.items[0].volume = 1.0;
    gm82_sound_runtime_bind(&sr, &sounds);
    gm82_sound_set_global(&sr);

    assert(gml_sound_volume(0, 0.8) == 1.0);
    assert(gml_sound_pan(0, -0.5) == 1.0);
    assert(gml_sound_pitch(0, 1.2) == 1.0);

    free(sounds.items);

    printf("  test_vm_keyboard_and_sound PASS\n");
}

static void test_vm_instance_management_v4(void) {
    gm82_runtime rt;
    gm82_runtime_init(&rt);

    gm82_decoded_object_list objs;
    memset(&objs, 0, sizeof(objs));
    objs.count = 2;
    objs.items = (gm82_decoded_object *)calloc(2, sizeof(gm82_decoded_object));
    strcpy(objs.items[0].name, "obj_player");
    objs.items[0].sprite_index = 1;
    strcpy(objs.items[1].name, "obj_enemy");
    objs.items[1].sprite_index = 2;

    gm82_runtime_bind_assets(&rt, &objs, NULL, NULL, NULL, NULL);

    gm82_instance *inst1 = gm82_runtime_instance_create(&rt, 0, 100, 100);
    gm82_instance *inst2 = gm82_runtime_instance_create(&rt, 1, 200, 200);
    assert(inst1 && inst2);
    assert(rt.instance_count == 2);

    gm82_gml_set_runtime(&rt);
    gm82_gml_set_self(inst1);

    /* Test instance_copy */
    double new_id = gml_instance_copy(1);
    assert(new_id > 0);
    assert(rt.instance_count == 3);

    /* Test instance_change */
    assert(gml_instance_change(1, 1) == 1.0);
    assert(inst1->object_index == 1);
    assert(inst1->sprite_index == 2);

    /* Test instance_deactivate_all */
    assert(gml_instance_deactivate_all(1) == 1.0);
    /* inst1 (self) should still be alive, inst2 and copy should be soft deactivated */
    assert(inst1->alive == 1);
    assert(inst2->alive == 0);

    free(objs.items);
    printf("  test_vm_instance_management_v4 PASS\n");
}

static void test_gml_vm_expanded_v4_ast(void) {
    const char *script =
        "sound_volume(1, 0.5);\n"
        "sound_pitch(1, 1.1);\n"
        "x = 50;\n"
        "y = 75;\n"
        "res = keyboard_check_direct(37) + instance_activate_all();\n"
        "return res;\n";

    gml_ast *ast = NULL;
    char err[160] = {0};
    int parse_ok = gml_parse_program(script, &ast, err, sizeof(err));
    assert(parse_ok);

    gml_vm vm;
    gml_vm_init(&vm);
    int exec_ok = gml_vm_execute(&vm, ast);
    assert(exec_ok);
    assert(vm.returned);

    gml_ast_free(ast);
    gml_value_free(&vm.return_value);
    printf("  test_gml_vm_expanded_v4_ast PASS\n");
}

void test_gml_vm_expanded_v4(void) {
    printf("=== Testing GML VM Expanded V4 Suite ===\n");
    test_vm_keyboard_and_sound();
    test_vm_instance_management_v4();
    test_gml_vm_expanded_v4_ast();
    printf("GML_VM_EXPANDED_V4_TEST_PASS\n");
}

int main(void) {
    test_gml_vm_expanded_v4();
    return 0;
}
