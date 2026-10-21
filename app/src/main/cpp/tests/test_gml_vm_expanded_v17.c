#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

static int dummy_script_callback(void *user, const char *name, const gml_value *args, size_t argc, gml_value *out) {
    (void)user; (void)name;
    if (argc >= 2) {
        double sum = args[0].real + args[1].real;
        if (out) *out = gml_value_real(sum);
        return 1;
    }
    return 0;
}

int main(void) {
    printf("[test_gml_vm_expanded_v17] Starting...\n");

    /* 1. Test ds_list_sort, ds_list_shuffle, ds_list_copy */
    double list1 = gml_ds_list_create();
    assert(list1 >= 0);
    gml_ds_list_add(list1, 42.0);
    gml_ds_list_add(list1, 10.0);
    gml_ds_list_add(list1, 99.0);
    gml_ds_list_add(list1, 5.0);

    /* Sort ascending */
    gml_ds_list_sort(list1, 1.0);
    assert(gml_ds_list_find_value(list1, 0) == 5.0);
    assert(gml_ds_list_find_value(list1, 1) == 10.0);
    assert(gml_ds_list_find_value(list1, 2) == 42.0);
    assert(gml_ds_list_find_value(list1, 3) == 99.0);

    /* Sort descending */
    gml_ds_list_sort(list1, 0.0);
    assert(gml_ds_list_find_value(list1, 0) == 99.0);
    assert(gml_ds_list_find_value(list1, 3) == 5.0);

    /* Copy list */
    double list2 = gml_ds_list_create();
    gml_ds_list_copy(list2, list1);
    assert(gml_ds_list_size(list2) == 4.0);
    assert(gml_ds_list_find_value(list2, 0) == 99.0);

    gml_ds_list_destroy(list1);
    gml_ds_list_destroy(list2);

    /* 2. Test audio compatibility builtins */
    double snd_id = 1.0;
    double p = gml_audio_play_sound(snd_id, 0.0, 0.0);
    assert(p == 1.0 || p == 0.0);
    double is_playing = gml_audio_is_playing(snd_id);
    (void)is_playing;
    gml_audio_stop_sound(snd_id);

    /* 3. Test AST VM invoke with argument0..15 and argument_count */
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast arg_ref0 = { GML_AST_NAME, .text = "argument0" };
    gml_ast arg_ref1 = { GML_AST_NAME, .text = "argument1" };
    gml_ast add_op = {
        GML_AST_BINARY,
        .op = GML_T_PLUS,
        .left = &arg_ref0,
        .right = &arg_ref1
    };
    gml_ast ret_stmt = {
        GML_AST_RETURN,
        .left = &add_op
    };

    gml_value args[2] = { gml_value_real(15.0), gml_value_real(27.0) };
    gml_value out_val;
    int ok = gml_vm_invoke(&vm, &ret_stmt, args, 2, &out_val);
    assert(ok == 1);
    assert(out_val.kind == GML_V_REAL);
    assert(out_val.real == 42.0);

    /* Verify argument_count variable inside VM scope */
    gml_ast cnt_ref = { GML_AST_NAME, .text = "argument_count" };
    gml_ast ret_cnt = {
        GML_AST_RETURN,
        .left = &cnt_ref
    };
    ok = gml_vm_invoke(&vm, &ret_cnt, args, 2, &out_val);
    assert(ok == 1);
    assert(out_val.kind == GML_V_REAL);
    assert(out_val.real == 2.0);

    /* 4. Test script_execute AST dispatch */
    gml_vm_set_script_call(&vm, dummy_script_callback, NULL);

    gml_ast script_id = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast param1 = { GML_AST_NUMBER, .number = 100.0 };
    gml_ast param2 = { GML_AST_NUMBER, .number = 250.0 };
    const gml_ast *se_params[3] = { &script_id, &param1, &param2 };

    gml_ast call_se = {
        GML_AST_CALL,
        .text = "script_execute",
        .items = (gml_ast**)se_params,
        .count = 3
    };

    gml_ast ret_se = {
        GML_AST_RETURN,
        .left = &call_se
    };

    ok = gml_vm_execute(&vm, &ret_se);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 350.0);

    printf("[test_gml_vm_expanded_v17] PASS\n");
    return 0;
}
