#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v18] Starting...\n");

    /* 1. Direct function checks */
    assert(gml_clamp(15.0, 0.0, 10.0) == 10.0);
    assert(gml_clamp(-5.0, 0.0, 10.0) == 0.0);
    assert(gml_clamp(5.0, 0.0, 10.0) == 5.0);

    assert(gml_lerp(10.0, 20.0, 0.5) == 15.0);
    assert(gml_sign(-12.5) == -1.0);
    assert(gml_sign(0.0) == 0.0);
    assert(gml_sign(42.0) == 1.0);

    assert(fabs(gml_frac(3.14159) - 0.14159) < 0.00001);

    char outbuf[256];
    gml_string_copy("Hello World", 1, 5, outbuf, sizeof(outbuf));
    assert(strcmp(outbuf, "Hello") == 0);

    assert(gml_string_pos("World", "Hello World") == 7.0);
    assert(gml_string_pos("XYZ", "Hello World") == 0.0);

    gml_string_delete("Hello World", 6, 6, outbuf, sizeof(outbuf));
    assert(strcmp(outbuf, "Hello") == 0);

    /* 2. AST VM Execution checks */
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test AST clamp(15, 0, 10) */
    gml_ast c_val = { GML_AST_NUMBER, .number = 15.0 };
    gml_ast c_min = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast c_max = { GML_AST_NUMBER, .number = 10.0 };
    const gml_ast *c_args[3] = { &c_val, &c_min, &c_max };
    gml_ast call_c = { GML_AST_CALL, .text = "clamp", .items = (gml_ast**)c_args, .count = 3 };
    gml_ast ret_c = { GML_AST_RETURN, .left = &call_c };

    int ok = gml_vm_execute(&vm, &ret_c);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 10.0);

    /* Test AST lerp(0, 100, 0.25) */
    gml_ast l_a = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast l_b = { GML_AST_NUMBER, .number = 100.0 };
    gml_ast l_amt = { GML_AST_NUMBER, .number = 0.25 };
    const gml_ast *l_args[3] = { &l_a, &l_b, &l_amt };
    gml_ast call_l = { GML_AST_CALL, .text = "lerp", .items = (gml_ast**)l_args, .count = 3 };
    gml_ast ret_l = { GML_AST_RETURN, .left = &call_l };

    ok = gml_vm_execute(&vm, &ret_l);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 25.0);

    /* Test AST string_copy("GameMaker", 1, 4) -> "Game" */
    gml_ast sc_s = { GML_AST_STRING, .text = "GameMaker" };
    gml_ast sc_i = { GML_AST_NUMBER, .number = 1.0 };
    gml_ast sc_c = { GML_AST_NUMBER, .number = 4.0 };
    const gml_ast *sc_args[3] = { &sc_s, &sc_i, &sc_c };
    gml_ast call_sc = { GML_AST_CALL, .text = "string_copy", .items = (gml_ast**)sc_args, .count = 3 };
    gml_ast ret_sc = { GML_AST_RETURN, .left = &call_sc };

    ok = gml_vm_execute(&vm, &ret_sc);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_STRING);
    assert(strcmp(vm.return_value.string, "Game") == 0);

    /* Test AST string_pos("Maker", "GameMaker") -> 5 */
    gml_ast sp_sub = { GML_AST_STRING, .text = "Maker" };
    gml_ast sp_str = { GML_AST_STRING, .text = "GameMaker" };
    const gml_ast *sp_args[2] = { &sp_sub, &sp_str };
    gml_ast call_sp = { GML_AST_CALL, .text = "string_pos", .items = (gml_ast**)sp_args, .count = 2 };
    gml_ast ret_sp = { GML_AST_RETURN, .left = &call_sp };

    ok = gml_vm_execute(&vm, &ret_sp);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 5.0);

    /* Test AST string_delete("GameMaker", 1, 4) -> "Maker" */
    gml_ast sd_s = { GML_AST_STRING, .text = "GameMaker" };
    gml_ast sd_i = { GML_AST_NUMBER, .number = 1.0 };
    gml_ast sd_c = { GML_AST_NUMBER, .number = 4.0 };
    const gml_ast *sd_args[3] = { &sd_s, &sd_i, &sd_c };
    gml_ast call_sd = { GML_AST_CALL, .text = "string_delete", .items = (gml_ast**)sd_args, .count = 3 };
    gml_ast ret_sd = { GML_AST_RETURN, .left = &call_sd };

    ok = gml_vm_execute(&vm, &ret_sd);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_STRING);
    assert(strcmp(vm.return_value.string, "Maker") == 0);

    printf("[test_gml_vm_expanded_v18] PASS\n");
    return 0;
}
