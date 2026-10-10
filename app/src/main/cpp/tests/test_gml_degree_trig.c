#include "gm82_gml_eval.h"
#include "gml_vm.h"
#include "gml_frontend.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("--- Running GML Degree Trig Tests ---\n");

    gm82_runtime rt;
    memset(&rt, 0, sizeof(rt));

    /* Test AST / Evaluator degree trig functions */
    double v_dsin = 0, v_dcos = 0, v_dtan = 0;
    assert(gm82_gml_eval_expr(&rt, NULL, "dsin(90)", &v_dsin));
    assert(fabs(v_dsin - 1.0) < 0.0001);

    assert(gm82_gml_eval_expr(&rt, NULL, "dcos(0)", &v_dcos));
    assert(fabs(v_dcos - 1.0) < 0.0001);

    assert(gm82_gml_eval_expr(&rt, NULL, "dtan(45)", &v_dtan));
    assert(fabs(v_dtan - 1.0) < 0.0001);

    double v_darcsin = 0, v_darccos = 0, v_darctan = 0, v_darctan2 = 0;
    assert(gm82_gml_eval_expr(&rt, NULL, "darcsin(1)", &v_darcsin));
    assert(fabs(v_darcsin - 90.0) < 0.0001);

    assert(gm82_gml_eval_expr(&rt, NULL, "darccos(1)", &v_darccos));
    assert(fabs(v_darccos - 0.0) < 0.0001);

    assert(gm82_gml_eval_expr(&rt, NULL, "darctan(1)", &v_darctan));
    assert(fabs(v_darctan - 45.0) < 0.0001);

    assert(gm82_gml_eval_expr(&rt, NULL, "darctan2(1, 1)", &v_darctan2));
    assert(fabs(v_darctan2 - 45.0) < 0.0001);

    /* Test Bytecode VM degree trig functions */
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast *ast_dsin = NULL;
    assert(gml_parse_program("dsin(90)", &ast_dsin, NULL, 0));
    assert(gml_vm_execute(&vm, ast_dsin));

    gml_ast *ast_darctan2 = NULL;
    assert(gml_parse_program("darctan2(1, 1)", &ast_darctan2, NULL, 0));
    assert(gml_vm_execute(&vm, ast_darctan2));

    gml_ast_free(ast_dsin);
    gml_ast_free(ast_darctan2);

    printf("ALL_DEGREE_TRIG_TESTS_PASS\n");
    return 0;
}
