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
#include <math.h>

void test_gml_vm_expanded_v5(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test 1: Script invocation with argument0 and argument1 in AST VM */
    {
        /* AST: return argument0 + argument1 * 2; */
        gml_ast arg0_node = { GML_AST_NAME, GML_T_NONE, 0, "argument0", NULL, NULL, 0, NULL };
        gml_ast arg1_node = { GML_AST_NAME, GML_T_NONE, 0, "argument1", NULL, NULL, 0, NULL };
        gml_ast two_node = { GML_AST_NUMBER, GML_T_NONE, 2, NULL, NULL, NULL, 0, NULL };

        gml_ast mul_node = { GML_AST_BINARY, GML_T_STAR, 0, NULL, &arg1_node, &two_node, 0, NULL };
        gml_ast add_node = { GML_AST_BINARY, GML_T_PLUS, 0, NULL, &arg0_node, &mul_node, 0, NULL };
        gml_ast ret_node = { GML_AST_RETURN, GML_T_NONE, 0, NULL, &add_node, NULL, 0, NULL };

        gml_value args[2];
        args[0] = gml_value_real(10.0);
        args[1] = gml_value_real(5.0);

        gml_value out = gml_value_real(0);
        int ok = gml_vm_invoke(&vm, &ret_node, args, 2, &out);
        assert(ok == 1);
        assert(out.kind == GML_V_REAL);
        assert(fabs(out.real - 20.0) < 0.0001);
        gml_value_free(&out);
    }

    /* Test 2: AST VM call to surface creation, existence, and freeing */
    {
        double surf_id = gml_surface_create(64, 64);
        assert(surf_id >= 0);
        assert(gml_surface_exists(surf_id) == 1.0);
        assert(gml_surface_get_width(surf_id) == 64.0);
        assert(gml_surface_get_height(surf_id) == 64.0);

        /* Test surface_exists dispatch in VM */
        gml_ast surf_arg = { GML_AST_NUMBER, GML_T_NONE, surf_id, NULL, NULL, NULL, 0, NULL };
        gml_ast *items[1] = { &surf_arg };
        gml_ast call_surf = { GML_AST_CALL, GML_T_NONE, 1, "surface_exists", NULL, NULL, 1, items };

        gml_ast assign = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var = { GML_AST_NAME, GML_T_NONE, 0, "surf_ok", NULL, NULL, 0, NULL };
        assign.left = &var;
        assign.right = &call_surf;
        gml_ast stmt = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt);
        assert(ok);
        gml_value res = gml_vm_get(&vm, "surf_ok");
        assert(res.kind == GML_V_REAL && res.real == 1.0);

        gml_surface_free(surf_id);
        assert(gml_surface_exists(surf_id) == 0.0);
    }

    /* Test 3: AST VM call to draw_text & motion */
    {
        /* AST: motion_set(90, 5) */
        gml_ast deg_arg = { GML_AST_NUMBER, GML_T_NONE, 90.0, NULL, NULL, NULL, 0, NULL };
        gml_ast spd_arg = { GML_AST_NUMBER, GML_T_NONE, 5.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *mitems[2] = { &deg_arg, &spd_arg };
        gml_ast call_motion = { GML_AST_CALL, GML_T_NONE, 2, "motion_set", NULL, NULL, 2, mitems };
        gml_ast stmt = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &call_motion, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt);
        assert(ok);
    }

    printf("  test_gml_vm_expanded_v5 PASS\n");
}

#ifndef RUNNING_FULL_SUITE
int main(void) {
    printf("=== Testing GML VM Expanded V5 Suite ===\n");
    test_gml_vm_expanded_v5();
    printf("GML_VM_EXPANDED_V5_TEST_PASS\n");
    return 0;
}
#endif
