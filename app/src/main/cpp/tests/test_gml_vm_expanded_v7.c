#include "gm82_runtime.h"
#include "gm82_sound_runtime.h"
#include "gm82_gml_builtins.h"
#include "gm82_gml_eval.h"
#include "gml_vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

void test_gml_vm_expanded_v7(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* 1. Test ds_grid_multiply_region builtin and AST execution */
    {
        double g_id = gml_ds_grid_create(4, 4);
        assert(g_id >= 0);

        gml_ds_grid_set_region(g_id, 0, 0, 3, 3, 5.0);
        assert(fabs(gml_ds_grid_get(g_id, 1, 1) - 5.0) < 0.0001);

        /* Call ds_grid_multiply_region via VM AST */
        gml_ast n_mult = { GML_AST_CALL, GML_T_NONE, 0, "ds_grid_multiply_region", NULL, NULL, 0, NULL };
        gml_ast p_gid = { GML_AST_NUMBER, GML_T_NONE, g_id, NULL, NULL, NULL, 0, NULL };
        gml_ast p_x1 = { GML_AST_NUMBER, GML_T_NONE, 1.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_y1 = { GML_AST_NUMBER, GML_T_NONE, 1.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_x2 = { GML_AST_NUMBER, GML_T_NONE, 2.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_y2 = { GML_AST_NUMBER, GML_T_NONE, 2.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_val = { GML_AST_NUMBER, GML_T_NONE, 3.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_m[6] = { &p_gid, &p_x1, &p_y1, &p_x2, &p_y2, &p_val };
        n_mult.count = 6;
        n_mult.items = items_m;

        gml_ast assign_m = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_m = { GML_AST_NAME, GML_T_NONE, 0, "m_res", NULL, NULL, 0, NULL };
        assign_m.left = &var_m;
        assign_m.right = &n_mult;
        gml_ast stmt_m = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_m, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt_m);
        assert(ok);

        /* Cell (0,0) should remain 5.0, cell (1,1) should be 5.0 * 3.0 = 15.0 */
        assert(fabs(gml_ds_grid_get(g_id, 0, 0) - 5.0) < 0.0001);
        assert(fabs(gml_ds_grid_get(g_id, 1, 1) - 15.0) < 0.0001);

        gml_ds_grid_destroy(g_id);
    }

    /* 2. Test string_insert and string_delete builtins & VM dispatches */
    {
        char buf1[128], buf2[128];
        gml_string_insert("World", "Hello ", 7, buf1, sizeof buf1);
        assert(strcmp(buf1, "Hello World") == 0);

        gml_string_delete("Hello World", 6, 6, buf2, sizeof buf2);
        assert(strcmp(buf2, "Hello") == 0);

        /* AST VM call for string_insert */
        gml_ast n_ins = { GML_AST_CALL, GML_T_NONE, 0, "string_insert", NULL, NULL, 0, NULL };
        gml_ast p_sub = { GML_AST_STRING, GML_T_NONE, 0, "GM82", NULL, NULL, 0, NULL };
        gml_ast p_str = { GML_AST_STRING, GML_T_NONE, 0, "Hello ", NULL, NULL, 0, NULL };
        gml_ast p_pos = { GML_AST_NUMBER, GML_T_NONE, 7.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_i[3] = { &p_sub, &p_str, &p_pos };
        n_ins.count = 3;
        n_ins.items = items_i;

        gml_ast assign_s = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_s = { GML_AST_NAME, GML_T_NONE, 0, "s_res", NULL, NULL, 0, NULL };
        assign_s.left = &var_s;
        assign_s.right = &n_ins;
        gml_ast stmt_s = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_s, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt_s);
        assert(ok);

        gml_value s_val = gml_vm_get(&vm, "s_res");
        assert(s_val.kind == GML_V_STRING);
        assert(s_val.string != NULL);
        assert(strcmp(s_val.string, "Hello GM82") == 0);
        gml_value_free(&s_val);
    }

    /* 3. Test random and choose AST dispatches */
    {
        gml_ast n_ch = { GML_AST_CALL, GML_T_NONE, 0, "choose", NULL, NULL, 0, NULL };
        gml_ast p_c0 = { GML_AST_NUMBER, GML_T_NONE, 10.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_c1 = { GML_AST_NUMBER, GML_T_NONE, 20.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_c[2] = { &p_c0, &p_c1 };
        n_ch.count = 2;
        n_ch.items = items_c;

        gml_ast assign_c = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_c = { GML_AST_NAME, GML_T_NONE, 0, "ch_res", NULL, NULL, 0, NULL };
        assign_c.left = &var_c;
        assign_c.right = &n_ch;
        gml_ast stmt_c = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_c, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt_c);
        assert(ok);

        gml_value ch_val = gml_vm_get(&vm, "ch_res");
        assert(ch_val.kind == GML_V_REAL);
        assert(ch_val.real == 10.0 || ch_val.real == 20.0);
    }

    /* 4. Test motion builtins and dispatches */
    {
        gml_motion_set(90.0, 5.0);
        gml_motion_add(0.0, 3.0);
        gml_move_towards_point(100.0, 100.0, 4.0);
    }

    printf("  test_gml_vm_expanded_v7 PASS\n");
}

#ifndef RUNNING_FULL_SUITE
int main(void) {
    printf("=== Testing GML VM Expanded V7 Suite ===\n");
    test_gml_vm_expanded_v7();
    printf("GML_VM_EXPANDED_V7_TEST_PASS\n");
    return 0;
}
#endif
