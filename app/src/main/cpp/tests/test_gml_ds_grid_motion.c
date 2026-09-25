#include "gm82_runtime.h"
#include "gm82_gml_builtins.h"
#include "gm82_gml_eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

static void test_ds_grid(void) {
    double grid = gml_ds_grid_create(4, 4);
    assert(grid >= 0);
    assert(gml_ds_grid_width(grid) == 4);
    assert(gml_ds_grid_height(grid) == 4);

    assert(gml_ds_grid_set(grid, 2, 2, 100.0) == 1.0);
    assert(gml_ds_grid_get(grid, 2, 2) == 100.0);

    assert(gml_ds_grid_multiply(grid, 2, 2, 2.5) == 1.0);
    assert(gml_ds_grid_get(grid, 2, 2) == 250.0);

    assert(gml_ds_grid_clear(grid, 5.0) == 1.0);
    assert(gml_ds_grid_get(grid, 0, 0) == 5.0);
    assert(gml_ds_grid_get(grid, 2, 2) == 5.0);

    assert(gml_ds_grid_destroy(grid) == 1.0);
    puts("test_ds_grid PASS");
}

static void test_ds_grid_ast_eval(void) {
    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_gml_set_runtime(&rt);

    gm82_instance *inst = gm82_runtime_instance_create(&rt, 0, 0, 0);
    gm82_gml_set_self(inst);

    double g = 0;
    assert(gm82_gml_eval_expr(&rt, inst, "ds_grid_create(4, 4)", &g) == true);
    assert(g >= 0);

    double res = 0;
    assert(gm82_gml_eval_expr(&rt, inst, "ds_grid_set(0, 1, 1, 42)", &res) == true);
    assert(gm82_gml_eval_expr(&rt, inst, "ds_grid_get(0, 1, 1)", &res) == true);
    assert(res == 42.0);

    assert(gm82_gml_eval_expr(&rt, inst, "ds_grid_multiply(0, 1, 1, 2)", &res) == true);
    assert(gm82_gml_eval_expr(&rt, inst, "ds_grid_get(0, 1, 1)", &res) == true);
    assert(res == 84.0);

    assert(gm82_gml_eval_expr(&rt, inst, "ds_grid_destroy(0)", &res) == true);
    puts("test_ds_grid_ast_eval PASS");
}

static void test_motion_functions(void) {
    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_gml_set_runtime(&rt);

    gm82_instance *inst = gm82_runtime_instance_create(&rt, 0, 0, 0);
    gm82_gml_set_self(inst);

    gml_motion_set(0, 10.0); /* 0 degrees = moving right */
    assert(inst->speed == 10.0);
    assert(fabs(inst->hspeed - 10.0) < 0.001);
    assert(fabs(inst->vspeed) < 0.001);

    gml_motion_add(90, 10.0); /* 90 degrees = moving up (-y in GM) */
    assert(fabs(inst->hspeed - 10.0) < 0.001);
    assert(fabs(inst->vspeed - (-10.0)) < 0.001);

    gml_move_towards_point(100, 0, 5.0);
    assert(inst->speed == 5.0);

    puts("test_motion_functions PASS");
}

int main(void) {
    puts("=== Testing GML 2D Grid Data Structure & Motion Functions ===");
    test_ds_grid();
    test_ds_grid_ast_eval();
    test_motion_functions();
    puts("GML_DS_GRID_MOTION_TEST_PASS");
    return 0;
}
