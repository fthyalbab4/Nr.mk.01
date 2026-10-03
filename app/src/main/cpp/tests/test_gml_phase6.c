#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "gml_vm.h"

static void test_type_checking(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    gml_value r = gml_vm_get(&vm, "nonexistent");
    assert(r.kind == GML_V_UNDEFINED);

    gml_ast call_nan = {0};
    call_nan.kind = GML_AST_EXPR_STMT;

    gml_ast inner_call_nan = {0};
    inner_call_nan.kind = GML_AST_CALL;
    inner_call_nan.text = "is_nan";
    inner_call_nan.count = 1;

    gml_ast arg_nan = {0};
    arg_nan.kind = GML_AST_NUMBER;
    arg_nan.number = NAN;

    gml_ast *items_nan[1] = { &arg_nan };
    inner_call_nan.items = items_nan;
    call_nan.left = &inner_call_nan;

    assert(gml_vm_execute(&vm, &call_nan));

    gml_ast call_ret = {0};
    call_ret.kind = GML_AST_RETURN;
    call_ret.left = &inner_call_nan;

    assert(gml_vm_execute(&vm, &call_ret));
    assert(vm.return_value.kind == GML_V_BOOL);
    assert(vm.return_value.boolean == 1);

    printf("test_type_checking PASS\n");
}

static void test_string_pos_ext(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast inner_call_spe = {0};
    inner_call_spe.kind = GML_AST_CALL;
    inner_call_spe.text = "string_pos_ext";
    inner_call_spe.count = 3;

    gml_ast needle = {0};
    needle.kind = GML_AST_STRING;
    needle.text = "world";

    gml_ast haystack = {0};
    haystack.kind = GML_AST_STRING;
    haystack.text = "hello world world";

    gml_ast start_pos = {0};
    start_pos.kind = GML_AST_NUMBER;
    start_pos.number = 8.0;

    gml_ast *items_spe[3] = { &needle, &haystack, &start_pos };
    inner_call_spe.items = items_spe;

    gml_ast call_ret = {0};
    call_ret.kind = GML_AST_RETURN;
    call_ret.left = &inner_call_spe;

    assert(gml_vm_execute(&vm, &call_ret));
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 13.0);

    printf("test_string_pos_ext PASS\n");
}

int main(void) {
    printf("=== Testing Phase 6 GML VM Builtins ===\n");
    test_type_checking();
    test_string_pos_ext();
    printf("GML_PHASE6_TEST_PASS\n");
    return 0;
}
