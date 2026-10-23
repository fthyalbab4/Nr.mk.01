#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v17] Starting...\n");

    /* 1. Test point_in_ellipse C function directly */
    double in_ellipse = gml_point_in_ellipse(10.0, 10.0, 0.0, 0.0, 20.0, 20.0);
    assert(in_ellipse == 1.0);

    double out_ellipse = gml_point_in_ellipse(0.0, 0.0, 10.0, 10.0, 20.0, 20.0);
    assert(out_ellipse == 0.0);

    /* 2. Test AST VM invocation for point_in_ellipse */
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast arg_px = { GML_AST_NUMBER, .number = 10.0 };
    gml_ast arg_py = { GML_AST_NUMBER, .number = 10.0 };
    gml_ast arg_x1 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast arg_y1 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast arg_x2 = { GML_AST_NUMBER, .number = 20.0 };
    gml_ast arg_y2 = { GML_AST_NUMBER, .number = 20.0 };

    const gml_ast *call_args[6] = {
        &arg_px, &arg_py, &arg_x1, &arg_y1, &arg_x2, &arg_y2
    };

    gml_ast call_pie = {
        GML_AST_CALL,
        .text = "point_in_ellipse",
        .items = (gml_ast**)call_args,
        .count = 6
    };

    gml_ast ret_stmt = {
        GML_AST_RETURN,
        .left = &call_pie
    };

    int ok = gml_vm_execute(&vm, &ret_stmt);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 1.0);

    /* 3. Test string_count via AST VM */
    gml_ast sc_sub = { GML_AST_STRING, .text = "la" };
    gml_ast sc_str = { GML_AST_STRING, .text = "lalala" };
    const gml_ast *sc_args[2] = { &sc_sub, &sc_str };
    gml_ast call_sc = {
        GML_AST_CALL,
        .text = "string_count",
        .items = (gml_ast**)sc_args,
        .count = 2
    };
    gml_ast ret_sc = { GML_AST_RETURN, .left = &call_sc };
    ok = gml_vm_execute(&vm, &ret_sc);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 3.0);

    /* 4. Test string_replace_all via AST VM */
    gml_ast sra_src = { GML_AST_STRING, .text = "hello world world" };
    gml_ast sra_old = { GML_AST_STRING, .text = "world" };
    gml_ast sra_new = { GML_AST_STRING, .text = "gml" };
    const gml_ast *sra_args[3] = { &sra_src, &sra_old, &sra_new };
    gml_ast call_sra = {
        GML_AST_CALL,
        .text = "string_replace_all",
        .items = (gml_ast**)sra_args,
        .count = 3
    };
    gml_ast ret_sra = { GML_AST_RETURN, .left = &call_sra };
    ok = gml_vm_execute(&vm, &ret_sra);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_STRING);
    assert(strcmp(vm.return_value.string, "hello gml gml") == 0);

    /* 5. Test file_text_eof via AST VM */
    gml_ast fte_id = { GML_AST_NUMBER, .number = -1.0 };
    const gml_ast *fte_args[1] = { &fte_id };
    gml_ast call_fte = {
        GML_AST_CALL,
        .text = "file_text_eof",
        .items = (gml_ast**)fte_args,
        .count = 1
    };
    gml_ast ret_fte = { GML_AST_RETURN, .left = &call_fte };
    ok = gml_vm_execute(&vm, &ret_fte);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 1.0);

    printf("[test_gml_vm_expanded_v17] PASS\n");
    return 0;
}
