#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v17] Starting...\n");

    /* 1. Direct call tests for rectangle_in_rectangle */
    /* sx1, sy1, sx2, sy2 inside dx1, dy1, dx2, dy2 (completely inside -> 1.0) */
    double in_rect = gml_rectangle_in_rectangle(10.0, 10.0, 20.0, 20.0, 0.0, 0.0, 30.0, 30.0);
    assert(in_rect == 1.0);

    /* overlapping -> 2.0 */
    double overlap_rect = gml_rectangle_in_rectangle(10.0, 10.0, 40.0, 40.0, 0.0, 0.0, 30.0, 30.0);
    assert(overlap_rect == 2.0);

    /* outside / no intersection -> 0.0 */
    double out_rect = gml_rectangle_in_rectangle(40.0, 40.0, 50.0, 50.0, 0.0, 0.0, 30.0, 30.0);
    assert(out_rect == 0.0);

    /* 2. Direct call tests for rectangle_in_circle */
    /* completely inside circle -> 1.0 */
    double in_circ = gml_rectangle_in_circle(-5.0, -5.0, 5.0, 5.0, 0.0, 0.0, 10.0);
    assert(in_circ == 1.0);

    /* overlapping circle -> 2.0 */
    double overlap_circ = gml_rectangle_in_circle(5.0, 5.0, 15.0, 15.0, 0.0, 0.0, 10.0);
    assert(overlap_circ == 2.0);

    /* outside circle -> 0.0 */
    double out_circ = gml_rectangle_in_circle(20.0, 20.0, 30.0, 30.0, 0.0, 0.0, 10.0);
    assert(out_circ == 0.0);

    /* 3. AST VM execution tests */
    gml_vm vm;
    gml_vm_init(&vm);

    /* test rectangle_in_rectangle AST execution */
    gml_ast a_sx1 = { GML_AST_NUMBER, .number = 10.0 };
    gml_ast a_sy1 = { GML_AST_NUMBER, .number = 10.0 };
    gml_ast a_sx2 = { GML_AST_NUMBER, .number = 20.0 };
    gml_ast a_sy2 = { GML_AST_NUMBER, .number = 20.0 };
    gml_ast a_dx1 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast a_dy1 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast a_dx2 = { GML_AST_NUMBER, .number = 30.0 };
    gml_ast a_dy2 = { GML_AST_NUMBER, .number = 30.0 };

    const gml_ast *r_args[8] = {
        &a_sx1, &a_sy1, &a_sx2, &a_sy2, &a_dx1, &a_dy1, &a_dx2, &a_dy2
    };

    gml_ast call_r_in_r = {
        GML_AST_CALL,
        .text = "rectangle_in_rectangle",
        .items = (gml_ast**)r_args,
        .count = 8
    };

    gml_ast ret_stmt1 = {
        GML_AST_RETURN,
        .left = &call_r_in_r
    };

    int ok1 = gml_vm_execute(&vm, &ret_stmt1);
    assert(ok1 == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 1.0);

    /* test rectangle_in_circle AST execution */
    gml_ast c_x1 = { GML_AST_NUMBER, .number = 5.0 };
    gml_ast c_y1 = { GML_AST_NUMBER, .number = 5.0 };
    gml_ast c_x2 = { GML_AST_NUMBER, .number = 15.0 };
    gml_ast c_y2 = { GML_AST_NUMBER, .number = 15.0 };
    gml_ast c_cx = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast c_cy = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast c_rad = { GML_AST_NUMBER, .number = 10.0 };

    const gml_ast *c_args[7] = {
        &c_x1, &c_y1, &c_x2, &c_y2, &c_cx, &c_cy, &c_rad
    };

    gml_ast call_r_in_c = {
        GML_AST_CALL,
        .text = "rectangle_in_circle",
        .items = (gml_ast**)c_args,
        .count = 7
    };

    gml_ast ret_stmt2 = {
        GML_AST_RETURN,
        .left = &call_r_in_c
    };

    int ok2 = gml_vm_execute(&vm, &ret_stmt2);
    assert(ok2 == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 2.0);

    printf("[test_gml_vm_expanded_v17] PASS\n");
    return 0;
}
