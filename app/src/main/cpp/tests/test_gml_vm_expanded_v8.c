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

void test_gml_vm_expanded_v8(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* 1. Test point_in_rectangle AST VM dispatch */
    {
        gml_ast n_pir = { GML_AST_CALL, GML_T_NONE, 0, "point_in_rectangle", NULL, NULL, 0, NULL };
        gml_ast p_px = { GML_AST_NUMBER, GML_T_NONE, 50.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_py = { GML_AST_NUMBER, GML_T_NONE, 50.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_x1 = { GML_AST_NUMBER, GML_T_NONE, 10.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_y1 = { GML_AST_NUMBER, GML_T_NONE, 10.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_x2 = { GML_AST_NUMBER, GML_T_NONE, 100.0, NULL, NULL, NULL, 0, NULL };
        gml_ast p_y2 = { GML_AST_NUMBER, GML_T_NONE, 100.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_pir[6] = { &p_px, &p_py, &p_x1, &p_y1, &p_x2, &p_y2 };
        n_pir.count = 6;
        n_pir.items = items_pir;

        gml_ast assign_pir = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_pir = { GML_AST_NAME, GML_T_NONE, 0, "in_rect", NULL, NULL, 0, NULL };
        assign_pir.left = &var_pir;
        assign_pir.right = &n_pir;
        gml_ast stmt_pir = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_pir, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt_pir);
        assert(ok);

        gml_value pir_val = gml_vm_get(&vm, "in_rect");
        assert(pir_val.kind == GML_V_REAL || pir_val.kind == GML_V_BOOL);
        assert((pir_val.kind == GML_V_BOOL ? pir_val.boolean : pir_val.real) == 1.0);
    }

    /* 2. Test window caption AST VM dispatches */
    {
        gml_ast n_sw = { GML_AST_CALL, GML_T_NONE, 0, "window_set_caption", NULL, NULL, 0, NULL };
        gml_ast p_title = { GML_AST_STRING, GML_T_NONE, 0, "GM82 Mobile Engine", NULL, NULL, 0, NULL };
        gml_ast *items_sw[1] = { &p_title };
        n_sw.count = 1;
        n_sw.items = items_sw;

        gml_ast stmt_sw = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_sw, NULL, 0, NULL };
        assert(gml_vm_execute(&vm, &stmt_sw));

        gml_ast n_gw = { GML_AST_CALL, GML_T_NONE, 0, "window_get_caption", NULL, NULL, 0, NULL };
        n_gw.count = 0;

        gml_ast assign_gw = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var_gw = { GML_AST_NAME, GML_T_NONE, 0, "title_res", NULL, NULL, 0, NULL };
        assign_gw.left = &var_gw;
        assign_gw.right = &n_gw;
        gml_ast stmt_gw = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign_gw, NULL, 0, NULL };

        assert(gml_vm_execute(&vm, &stmt_gw));

        gml_value t_val = gml_vm_get(&vm, "title_res");
        assert(t_val.kind == GML_V_STRING);
        assert(t_val.string != NULL);
        assert(strcmp(t_val.string, "GM82 Mobile Engine") == 0);
        gml_value_free(&t_val);
    }

    /* 3. Test sound controls AST VM dispatches */
    {
        gml_ast n_snd = { GML_AST_CALL, GML_T_NONE, 0, "sound_play", NULL, NULL, 0, NULL };
        gml_ast p_sidx = { GML_AST_NUMBER, GML_T_NONE, 1.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items_snd[1] = { &p_sidx };
        n_snd.count = 1;
        n_snd.items = items_snd;

        gml_ast stmt_snd = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_snd, NULL, 0, NULL };
        assert(gml_vm_execute(&vm, &stmt_snd));
    }

    printf("  test_gml_vm_expanded_v8 PASS\n");
}

#ifndef RUNNING_FULL_SUITE
int main(void) {
    printf("=== Testing GML VM Expanded V8 Suite ===\n");
    test_gml_vm_expanded_v8();
    printf("GML_VM_EXPANDED_V8_TEST_PASS\n");
    return 0;
}
#endif
