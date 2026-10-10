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

    /* Motion builtins test */
    gml_motion_set(90.0, 5.0);
    gml_motion_add(0.0, 5.0);

    /* Spatial queries test */
    double empty1 = gml_place_empty(100.0, 100.0);
    double empty2 = gml_position_empty(500.0, 500.0);
    assert(empty1 == 1.0 || empty1 == 0.0);
    assert(empty2 == 1.0 || empty2 == 0.0);

    /* Extended drawing functions */
    double d1 = gml_draw_line_width(0.0, 0.0, 100.0, 100.0, 2.0);
    double d2 = gml_draw_circle_color(50.0, 50.0, 25.0, 255.0, 65535.0, 0.0);
    double d3 = gml_draw_rectangle_color(10.0, 10.0, 80.0, 80.0, 255.0, 65535.0, 16711680.0, 0.0, 1.0);
    double d4 = gml_draw_text_ext(10.0, 10.0, "Testing extended text", 16.0, 200.0);
    assert(d1 == 1.0);
    assert(d2 == 1.0);
    assert(d3 == 1.0);
    assert(d4 == 1.0);

    /* Surface manager test */
    double s_id = gml_surface_create(256.0, 256.0);
    assert(s_id >= 0.0);
    double s_exists = gml_surface_exists(s_id);
    assert(s_exists == 1.0);
    gml_surface_set_target(s_id);
    gml_surface_reset_target();
    gml_surface_free(s_id);
    assert(gml_surface_exists(s_id) == 0.0);

    /* AST VM evaluation test for new builtins */
    {
        gml_ast n_motion = { GML_AST_CALL, GML_T_NONE, 0, "motion_set", NULL, NULL, 0, NULL };
        gml_ast arg_dir = { GML_AST_NUMBER, GML_T_NONE, 90.0, NULL, NULL, NULL, 0, NULL };
        gml_ast arg_spd = { GML_AST_NUMBER, GML_T_NONE, 10.0, NULL, NULL, NULL, 0, NULL };
        gml_ast *items[2] = { &arg_dir, &arg_spd };
        n_motion.count = 2;
        n_motion.items = items;

        gml_ast assign = { GML_AST_ASSIGN, GML_T_NONE, 0, NULL, NULL, NULL, 0, NULL };
        gml_ast var = { GML_AST_NAME, GML_T_NONE, 0, "m_res", NULL, NULL, 0, NULL };
        assign.left = &var;
        assign.right = &n_motion;
        gml_ast stmt = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &assign, NULL, 0, NULL };

        int ok = gml_vm_execute(&vm, &stmt);
        assert(ok);
        gml_value res = gml_vm_get(&vm, "m_res");
        assert(res.kind == GML_V_REAL && res.real == 1.0);
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
