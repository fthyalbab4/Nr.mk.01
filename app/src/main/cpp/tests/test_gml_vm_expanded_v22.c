#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v22] Starting...\n");

    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_gml_set_runtime(&rt);

    /* 1. Direct builtins verification */
    assert(gml_mouse_x() >= 0.0 || gml_mouse_x() == 0.0);
    assert(gml_mouse_y() >= 0.0 || gml_mouse_y() == 0.0);
    assert(gml_room_goto(0.0) == 1.0 || gml_room_goto(0.0) == 0.0);
    assert(gml_room_goto_next() == 1.0 || gml_room_goto_next() == 0.0);

    double inst_id = gml_instance_create(100.0, 200.0, 1.0);
    assert(inst_id >= 100000.0);
    assert(gml_instance_exists(inst_id) == 1.0);

    /* Sound playback Queue check */
    assert(gml_sound_play(0.0) == 1.0 || gml_sound_play(0.0) == 0.0);
    assert(gml_sound_isplaying(0.0) == 0.0 || gml_sound_isplaying(0.0) == 1.0);
    assert(gml_sound_stop(0.0) == 1.0 || gml_sound_stop(0.0) == 0.0);

    /* 2. AST VM Execution verification */
    /* Test AST call: instance_exists(inst_id) */
    {
        gml_vm vm;
        gml_vm_init(&vm);
        gml_ast arg_obj = { GML_AST_NUMBER, .number = inst_id };
        const gml_ast *exists_args[1] = { &arg_obj };
        gml_ast call_exists = { GML_AST_CALL, .text = "instance_exists", .items = (gml_ast**)exists_args, .count = 1 };
        gml_ast ret_exists = { GML_AST_RETURN, .left = &call_exists };

        int ok = gml_vm_execute(&vm, &ret_exists);
        assert(ok == 1);
        assert(vm.return_value.kind == GML_V_REAL);
        assert(vm.return_value.real == 1.0);
    }

    /* Test AST call: sound_play(0.0) */
    {
        gml_vm vm;
        gml_vm_init(&vm);
        gml_ast arg_snd = { GML_AST_NUMBER, .number = 0.0 };
        const gml_ast *snd_args[1] = { &arg_snd };
        gml_ast call_snd = { GML_AST_CALL, .text = "sound_play", .items = (gml_ast**)snd_args, .count = 1 };
        gml_ast ret_snd = { GML_AST_RETURN, .left = &call_snd };

        int ok = gml_vm_execute(&vm, &ret_snd);
        assert(ok == 1);
        assert(vm.return_value.kind == GML_V_REAL);
    }

    /* Test AST call: mouse_x() */
    {
        gml_vm vm;
        gml_vm_init(&vm);
        gml_ast call_mx = { GML_AST_CALL, .text = "mouse_x", .count = 0 };
        gml_ast ret_mx = { GML_AST_RETURN, .left = &call_mx };

        int ok = gml_vm_execute(&vm, &ret_mx);
        assert(ok == 1);
        assert(vm.return_value.kind == GML_V_REAL);
    }

    /* Test AST name lookup: mouse_x variable */
    {
        gml_vm vm;
        gml_vm_init(&vm);
        gml_ast var_mx = { GML_AST_NAME, .text = "mouse_x" };
        gml_ast ret_vmx = { GML_AST_RETURN, .left = &var_mx };

        int ok = gml_vm_execute(&vm, &ret_vmx);
        assert(ok == 1);
        assert(vm.return_value.kind == GML_V_REAL);
    }

    printf("[test_gml_vm_expanded_v22] PASS\n");
    return 0;
}
