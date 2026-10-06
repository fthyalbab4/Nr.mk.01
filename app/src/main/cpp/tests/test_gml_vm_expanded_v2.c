#include "gml_vm.h"
#include "gm82_gml_builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

static void test_vm_ds_structures_v2(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test ds_stack */
    gml_ast st_create = { GML_AST_CALL, GML_T_NONE, 0, "ds_stack_create", NULL, NULL, 0, NULL };
    gml_ast assign_st = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_st = { GML_AST_NAME, GML_T_NONE, 0, "st", NULL, NULL, 0, NULL };
    assign_st.left = &var_st;
    assign_st.right = &st_create;
    gml_ast stmt_st = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_st, NULL, 0, NULL };
    int ok = gml_vm_execute(&vm, &stmt_st);
    assert(ok);
    gml_value st_val = gml_vm_get(&vm, "st");
    assert(st_val.kind == GML_V_REAL);
    double st_id = st_val.real;

    /* Push 100 to stack */
    gml_ast st_push = { GML_AST_CALL, GML_T_NONE, 0, "ds_stack_push", NULL, NULL, 0, NULL };
    gml_ast arg_stid = { GML_AST_NUMBER, GML_T_NONE, st_id, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_val100 = { GML_AST_NUMBER, GML_T_NONE, 100, NULL, NULL, NULL, 0, NULL };
    gml_ast *items_push[2] = { &arg_stid, &arg_val100 };
    st_push.count = 2;
    st_push.items = items_push;
    gml_ast stmt_push = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &st_push, NULL, 0, NULL };
    ok = gml_vm_execute(&vm, &stmt_push);
    assert(ok);

    /* Check top */
    gml_ast st_top = { GML_AST_CALL, GML_T_NONE, 0, "ds_stack_top", NULL, NULL, 0, NULL };
    gml_ast *items_top[1] = { &arg_stid };
    st_top.count = 1;
    st_top.items = items_top;
    gml_ast assign_top = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_top = { GML_AST_NAME, GML_T_NONE, 0, "top_val", NULL, NULL, 0, NULL };
    assign_top.left = &var_top;
    assign_top.right = &st_top;
    gml_ast stmt_top = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_top, NULL, 0, NULL };
    ok = gml_vm_execute(&vm, &stmt_top);
    assert(ok);
    gml_value top_val = gml_vm_get(&vm, "top_val");
    assert(top_val.kind == GML_V_REAL && top_val.real == 100.0);

    /* Pop top */
    gml_ast st_pop = { GML_AST_CALL, GML_T_NONE, 0, "ds_stack_pop", NULL, NULL, 0, NULL };
    st_pop.count = 1;
    st_pop.items = items_top;
    gml_ast assign_pop = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_pop = { GML_AST_NAME, GML_T_NONE, 0, "pop_val", NULL, NULL, 0, NULL };
    assign_pop.left = &var_pop;
    assign_pop.right = &st_pop;
    gml_ast stmt_pop = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_pop, NULL, 0, NULL };
    ok = gml_vm_execute(&vm, &stmt_pop);
    assert(ok);
    gml_value pop_val = gml_vm_get(&vm, "pop_val");
    assert(pop_val.kind == GML_V_REAL && pop_val.real == 100.0);

    /* Destroy stack */
    gml_ast st_del = { GML_AST_CALL, GML_T_NONE, 0, "ds_stack_destroy", NULL, NULL, 0, NULL };
    st_del.count = 1;
    st_del.items = items_top;
    gml_ast stmt_del = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &st_del, NULL, 0, NULL };
    gml_vm_execute(&vm, &stmt_del);

    printf("  test_vm_ds_structures_v2 PASS\n");
}

static void test_vm_buffer_poke_peek_and_math(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test buffer_create(128, 0, 1) */
    gml_ast b_create = { GML_AST_CALL, GML_T_NONE, 0, "buffer_create", NULL, NULL, 0, NULL };
    gml_ast arg_sz = { GML_AST_NUMBER, GML_T_NONE, 128, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_tp = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_align = { GML_AST_NUMBER, GML_T_NONE, 1, NULL, NULL, NULL, 0, NULL };
    gml_ast *items_c[3] = { &arg_sz, &arg_tp, &arg_align };
    b_create.count = 3;
    b_create.items = items_c;

    gml_ast assign_buf = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_buf = { GML_AST_NAME, GML_T_NONE, 0, "buf", NULL, NULL, 0, NULL };
    assign_buf.left = &var_buf;
    assign_buf.right = &b_create;
    gml_ast stmt_buf = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_buf, NULL, 0, NULL };
    int ok = gml_vm_execute(&vm, &stmt_buf);
    assert(ok);
    gml_value buf_val = gml_vm_get(&vm, "buf");
    double buf_id = buf_val.real;

    /* Poke 77 at offset 4 */
    gml_ast b_poke = { GML_AST_CALL, GML_T_NONE, 0, "buffer_poke", NULL, NULL, 0, NULL };
    gml_ast arg_bid = { GML_AST_NUMBER, GML_T_NONE, buf_id, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_off4 = { GML_AST_NUMBER, GML_T_NONE, 4, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_type3 = { GML_AST_NUMBER, GML_T_NONE, 3, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_val77 = { GML_AST_NUMBER, GML_T_NONE, 77, NULL, NULL, NULL, 0, NULL };
    gml_ast *items_pk[4] = { &arg_bid, &arg_off4, &arg_type3, &arg_val77 };
    b_poke.count = 4;
    b_poke.items = items_pk;
    gml_ast stmt_pk = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &b_poke, NULL, 0, NULL };
    ok = gml_vm_execute(&vm, &stmt_pk);
    assert(ok);

    /* Peek at offset 4 */
    gml_ast b_peek = { GML_AST_CALL, GML_T_NONE, 0, "buffer_peek", NULL, NULL, 0, NULL };
    gml_ast *items_pkv[3] = { &arg_bid, &arg_off4, &arg_type3 };
    b_peek.count = 3;
    b_peek.items = items_pkv;
    gml_ast assign_peek = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_peek = { GML_AST_NAME, GML_T_NONE, 0, "peeked", NULL, NULL, 0, NULL };
    assign_peek.left = &var_peek;
    assign_peek.right = &b_peek;
    gml_ast stmt_peek = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_peek, NULL, 0, NULL };
    ok = gml_vm_execute(&vm, &stmt_peek);
    assert(ok);
    gml_value peeked = gml_vm_get(&vm, "peeked");
    assert(peeked.kind == GML_V_REAL && peeked.real == 77.0);

    /* Clean buffer */
    gml_ast b_del = { GML_AST_CALL, GML_T_NONE, 0, "buffer_delete", NULL, NULL, 0, NULL };
    gml_ast *items_s[1] = { &arg_bid };
    b_del.count = 1;
    b_del.items = items_s;
    gml_ast stmt_del = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &b_del, NULL, 0, NULL };
    gml_vm_execute(&vm, &stmt_del);

    /* Test angle_difference(90, 45) -> 45 */
    gml_ast a_diff = { GML_AST_CALL, GML_T_NONE, 0, "angle_difference", NULL, NULL, 0, NULL };
    gml_ast arg_d1 = { GML_AST_NUMBER, GML_T_NONE, 90, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_d2 = { GML_AST_NUMBER, GML_T_NONE, 45, NULL, NULL, NULL, 0, NULL };
    gml_ast *items_ad[2] = { &arg_d1, &arg_d2 };
    a_diff.count = 2;
    a_diff.items = items_ad;
    gml_ast assign_ad = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_ad = { GML_AST_NAME, GML_T_NONE, 0, "adiff", NULL, NULL, 0, NULL };
    assign_ad.left = &var_ad;
    assign_ad.right = &a_diff;
    gml_ast stmt_ad = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_ad, NULL, 0, NULL };
    ok = gml_vm_execute(&vm, &stmt_ad);
    assert(ok);
    gml_value ad_val = gml_vm_get(&vm, "adiff");
    assert(ad_val.kind == GML_V_REAL && fabs(ad_val.real - 45.0) < 0.0001);

    printf("  test_vm_buffer_poke_peek_and_math PASS\n");
}

int main(void) {
    printf("=== Testing GML VM Expanded V2 Suite ===\n");
    test_vm_ds_structures_v2();
    test_vm_buffer_poke_peek_and_math();
    printf("GML_VM_EXPANDED_V2_TEST_PASS\n");
    return 0;
}
