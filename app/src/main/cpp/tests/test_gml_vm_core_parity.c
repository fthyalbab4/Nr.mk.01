#include "gml_vm.h"
#include "gm82_gml_builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

static void test_vm_buffer_and_file_ops(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast b_create = { GML_AST_CALL, GML_T_NONE, 0, "buffer_create", NULL, NULL, 0, NULL };
    gml_ast arg_sz = { GML_AST_NUMBER, GML_T_NONE, 256, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_tp = { GML_AST_NUMBER, GML_T_NONE, 1, NULL, NULL, NULL, 0, NULL };
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
    assert(buf_val.kind == GML_V_REAL);
    double buf_id = buf_val.real;
    assert(buf_id >= 0);

    /* Write value 42 to buffer */
    gml_ast b_write = { GML_AST_CALL, GML_T_NONE, 0, "buffer_write", NULL, NULL, 0, NULL };
    gml_ast arg_bid = { GML_AST_NUMBER, GML_T_NONE, buf_id, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_type3 = { GML_AST_NUMBER, GML_T_NONE, 3, NULL, NULL, NULL, 0, NULL }; /* s32 */
    gml_ast arg_val = { GML_AST_NUMBER, GML_T_NONE, 42, NULL, NULL, NULL, 0, NULL };
    gml_ast *items_w[3] = { &arg_bid, &arg_type3, &arg_val };
    b_write.count = 3;
    b_write.items = items_w;

    gml_ast stmt_w = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &b_write, NULL, 0, NULL };
    ok = gml_vm_execute(&vm, &stmt_w);
    assert(ok);

    /* Check buffer size */
    gml_ast b_size = { GML_AST_CALL, GML_T_NONE, 0, "buffer_get_size", NULL, NULL, 0, NULL };
    gml_ast *items_s[1] = { &arg_bid };
    b_size.count = 1;
    b_size.items = items_s;

    gml_ast assign_sz = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_sz = { GML_AST_NAME, GML_T_NONE, 0, "sz", NULL, NULL, 0, NULL };
    assign_sz.left = &var_sz;
    assign_sz.right = &b_size;

    gml_ast stmt_sz = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_sz, NULL, 0, NULL };

    ok = gml_vm_execute(&vm, &stmt_sz);
    assert(ok);
    gml_value sz_val = gml_vm_get(&vm, "sz");
    assert(sz_val.kind == GML_V_REAL && sz_val.real == 4);

    /* Seek back to 0 */
    gml_ast b_seek = { GML_AST_CALL, GML_T_NONE, 0, "buffer_seek", NULL, NULL, 0, NULL };
    gml_ast arg_base0 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_off0 = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast *items_sk[3] = { &arg_bid, &arg_base0, &arg_off0 };
    b_seek.count = 3;
    b_seek.items = items_sk;
    gml_ast stmt_sk = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &b_seek, NULL, 0, NULL };
    gml_vm_execute(&vm, &stmt_sk);

    /* Read value back */
    gml_ast b_read = { GML_AST_CALL, GML_T_NONE, 0, "buffer_read", NULL, NULL, 0, NULL };
    gml_ast *items_r[2] = { &arg_bid, &arg_type3 };
    b_read.count = 2;
    b_read.items = items_r;

    gml_ast assign_read = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_val = { GML_AST_NAME, GML_T_NONE, 0, "read_val", NULL, NULL, 0, NULL };
    assign_read.left = &var_val;
    assign_read.right = &b_read;

    gml_ast stmt_read = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_read, NULL, 0, NULL };

    ok = gml_vm_execute(&vm, &stmt_read);
    assert(ok);
    gml_value read_val = gml_vm_get(&vm, "read_val");
    assert(read_val.kind == GML_V_REAL && read_val.real == 42);

    /* Clean up buffer */
    gml_ast b_del = { GML_AST_CALL, GML_T_NONE, 0, "buffer_delete", NULL, NULL, 0, NULL };
    b_del.count = 1;
    b_del.items = items_s;
    gml_ast stmt_del = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &b_del, NULL, 0, NULL };
    gml_vm_execute(&vm, &stmt_del);

    printf("  test_vm_buffer_and_file_ops PASS\n");
}

static void test_vm_string_and_color_math(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test make_color_hsv(128, 255, 255) */
    gml_ast c_hsv = { GML_AST_CALL, GML_T_NONE, 0, "make_color_hsv", NULL, NULL, 0, NULL };
    gml_ast arg_h = { GML_AST_NUMBER, GML_T_NONE, 128, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_s = { GML_AST_NUMBER, GML_T_NONE, 255, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_v = { GML_AST_NUMBER, GML_T_NONE, 255, NULL, NULL, NULL, 0, NULL };
    gml_ast *items_hsv[3] = { &arg_h, &arg_s, &arg_v };
    c_hsv.count = 3;
    c_hsv.items = items_hsv;

    gml_ast assign_col = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_col = { GML_AST_NAME, GML_T_NONE, 0, "col", NULL, NULL, 0, NULL };
    assign_col.left = &var_col;
    assign_col.right = &c_hsv;

    gml_ast stmt_col = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_col, NULL, 0, NULL };

    int ok = gml_vm_execute(&vm, &stmt_col);
    assert(ok);
    gml_value col_val = gml_vm_get(&vm, "col");
    assert(col_val.kind == GML_V_REAL);

    /* Test clamp(150, 0, 100) -> 100 */
    gml_ast m_clamp = { GML_AST_CALL, GML_T_NONE, 0, "clamp", NULL, NULL, 0, NULL };
    gml_ast arg_x = { GML_AST_NUMBER, GML_T_NONE, 150, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_lo = { GML_AST_NUMBER, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_hi = { GML_AST_NUMBER, GML_T_NONE, 100, NULL, NULL, NULL, 0, NULL };
    gml_ast *items_clp[3] = { &arg_x, &arg_lo, &arg_hi };
    m_clamp.count = 3;
    m_clamp.items = items_clp;

    gml_ast assign_clp = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_clp = { GML_AST_NAME, GML_T_NONE, 0, "clp", NULL, NULL, 0, NULL };
    assign_clp.left = &var_clp;
    assign_clp.right = &m_clamp;

    gml_ast stmt_clp = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_clp, NULL, 0, NULL };

    ok = gml_vm_execute(&vm, &stmt_clp);
    assert(ok);
    gml_value clp_val = gml_vm_get(&vm, "clp");
    assert(clp_val.kind == GML_V_REAL && clp_val.real == 100.0);

    /* Test lerp(10, 20, 0.5) -> 15 */
    gml_ast m_lerp = { GML_AST_CALL, GML_T_NONE, 0, "lerp", NULL, NULL, 0, NULL };
    gml_ast arg_a = { GML_AST_NUMBER, GML_T_NONE, 10, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_b = { GML_AST_NUMBER, GML_T_NONE, 20, NULL, NULL, NULL, 0, NULL };
    gml_ast arg_t = { GML_AST_NUMBER, GML_T_NONE, 0.5, NULL, NULL, NULL, 0, NULL };
    gml_ast *items_lrp[3] = { &arg_a, &arg_b, &arg_t };
    m_lerp.count = 3;
    m_lerp.items = items_lrp;

    gml_ast assign_lrp = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
    gml_ast var_lrp = { GML_AST_NAME, GML_T_NONE, 0, "lrp", NULL, NULL, 0, NULL };
    assign_lrp.left = &var_lrp;
    assign_lrp.right = &m_lerp;

    gml_ast stmt_lrp = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_lrp, NULL, 0, NULL };

    ok = gml_vm_execute(&vm, &stmt_lrp);
    assert(ok);
    gml_value lrp_val = gml_vm_get(&vm, "lrp");
    assert(lrp_val.kind == GML_V_REAL && lrp_val.real == 15.0);

    printf("  test_vm_string_and_color_math PASS\n");
}

int main(void) {
    printf("=== Testing GML VM Core Parity Suite ===\n");
    test_vm_buffer_and_file_ops();
    test_vm_string_and_color_math();
    printf("GML_VM_CORE_PARITY_TEST_PASS\n");
    return 0;
}
