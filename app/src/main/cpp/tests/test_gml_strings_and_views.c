#include "gm82_gml_eval.h"
#include "gm82_gml_builtins.h"
#include "gm82_runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void test_string_literals_and_concat(void) {
    puts("Testing string literals and string concatenation...");
    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_instance *inst = gm82_runtime_instance_create(&rt, 0, 10, 20);
    assert(inst != NULL);

    double val = 0;
    bool ok = gm82_gml_eval_expr(&rt, inst, "\"Hello \" + \"World\"", &val);
    assert(ok == true);

    ok = gm82_gml_eval_expr(&rt, inst, "string_length(\"Hello World\")", &val);
    assert(ok == true);
    assert(val == 11.0);

    ok = gm82_gml_eval_stmt(&rt, inst, "draw_text(10, 20, \"Player 1\")");
    assert(ok == true);

    puts("  test_string_literals_and_concat PASS");
}

static void test_color_constants(void) {
    puts("Testing color constants in evaluator...");
    gm82_runtime rt;
    gm82_runtime_init(&rt);

    double val = 0;
    gm82_gml_eval_expr(&rt, NULL, "c_black", &val);
    assert(val == 0.0);

    gm82_gml_eval_expr(&rt, NULL, "c_white", &val);
    assert(val == 16777215.0);

    gm82_gml_eval_expr(&rt, NULL, "c_red", &val);
    assert(val == 255.0);

    gm82_gml_eval_expr(&rt, NULL, "c_yellow", &val);
    assert(val == 65535.0);

    puts("  test_color_constants PASS");
}

static void test_view_and_alarm_variables(void) {
    puts("Testing view variables and alarm variables...");
    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_instance *inst = gm82_runtime_instance_create(&rt, 0, 100, 200);

    gm82_gml_eval_block(&rt, inst, "view_enabled = 1; view_xview = 50; view_yview = 75;");
    assert(rt.view_enabled == 1);
    assert(rt.view_x == 50.0);
    assert(rt.view_y == 75.0);

    gm82_gml_eval_block(&rt, inst, "alarm0 = 60; alarm1 = 120;");
    assert(inst->alarms[0] == 60);
    assert(inst->alarms[1] == 120);

    double val = 0;
    gm82_gml_eval_expr(&rt, inst, "alarm0", &val);
    assert(val == 60.0);

    gm82_gml_eval_expr(&rt, inst, "alarm1", &val);
    assert(val == 120.0);

    puts("  test_view_and_alarm_variables PASS");
}

int main(void) {
    puts("=== Testing GML Strings, Views & Constants Suite ===");
    test_string_literals_and_concat();
    test_color_constants();
    test_view_and_alarm_variables();
    puts("GML_STRINGS_AND_VIEWS_TEST_PASS");
    return 0;
}
