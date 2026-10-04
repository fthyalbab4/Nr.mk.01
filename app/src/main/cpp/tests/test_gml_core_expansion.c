#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gm82_gml_eval.h"
#include "gm82_runtime.h"

static void test_degree_trig_functions(void) {
    printf("Testing degree trig functions...\n");
    double sin_90 = gml_dsin(90.0);
    assert(fabs(sin_90 - 1.0) < 1e-5);

    double cos_0 = gml_dcos(0.0);
    assert(fabs(cos_0 - 1.0) < 1e-5);

    double tan_45 = gml_dtan(45.0);
    assert(fabs(tan_45 - 1.0) < 1e-5);

    double asin_1 = gml_darcsin(1.0);
    assert(fabs(asin_1 - 90.0) < 1e-5);

    double acos_1 = gml_darccos(1.0);
    assert(fabs(acos_1 - 0.0) < 1e-5);

    double atan_1 = gml_darctan(1.0);
    assert(fabs(atan_1 - 45.0) < 1e-5);

    double atan2_11 = gml_darctan2(1.0, 1.0);
    assert(fabs(atan2_11 - 45.0) < 1e-5);

    printf("  test_degree_trig_functions PASS\n");
}

static void test_string_letters_width_height(void) {
    printf("Testing string letters, lettersdigits, width & height...\n");
    char buf[64];

    gml_string_letters("Hello123World!#$", buf, sizeof(buf));
    assert(strcmp(buf, "HelloWorld") == 0);

    gml_string_lettersdigits("Hello123World!#$", buf, sizeof(buf));
    assert(strcmp(buf, "Hello123World") == 0);

    double w = gml_string_width("Hello");
    assert(w == 40.0);

    double h = gml_string_height("Hello\nWorld\n!");
    assert(h == 48.0);

    printf("  test_string_letters_width_height PASS\n");
}

static void test_eval_expansion(void) {
    printf("Testing GML expression evaluation dispatches...\n");
    gm82_runtime rt;
    memset(&rt, 0, sizeof(rt));
    gm82_gml_set_runtime(&rt);

    gm82_instance self;
    memset(&self, 0, sizeof(self));
    self.alive = true;
    self.x = 10.0;
    self.y = 20.0;
    gm82_gml_set_self(&self);

    double val = 0;
    gm82_gml_eval_expr(&rt, &self, "dsin(90)", &val);
    assert(fabs(val - 1.0) < 1e-5);

    gm82_gml_eval_expr(&rt, &self, "dcos(0)", &val);
    assert(fabs(val - 1.0) < 1e-5);

    gm82_gml_eval_expr(&rt, &self, "distance_to_point(10, 50)", &val);
    assert(fabs(val - 30.0) < 1e-5);

    gm82_gml_eval_expr(&rt, &self, "ds_list_create()", &val);
    assert(val >= 0);

    gm82_gml_eval_expr(&rt, &self, "ds_list_add(0, 42)", &val);
    gm82_gml_eval_expr(&rt, &self, "ds_list_find_value(0, 0)", &val);
    assert(val == 42.0);

    printf("  test_eval_expansion PASS\n");
}

int main(void) {
    printf("=== Testing GML Core Expansion Suite ===\n");
    test_degree_trig_functions();
    test_string_letters_width_height();
    test_eval_expansion();
    printf("GML_CORE_EXPANSION_TEST_PASS\n");
    return 0;
}
