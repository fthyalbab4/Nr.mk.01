#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "gm82_runtime.h"
#include "gm82_gml_builtins.h"
#include "gm82_gml_eval.h"

static void test_instance_start_previous_vars(void) {
    printf("Testing xstart, ystart, xprevious, yprevious...\n");
    gm82_runtime rt;
    gm82_runtime_init(&rt);

    gm82_instance *inst = gm82_runtime_instance_create(&rt, 0, 100.0, 200.0);
    assert(inst != NULL);
    assert(inst->xstart == 100.0);
    assert(inst->ystart == 200.0);
    assert(inst->xprevious == 100.0);
    assert(inst->yprevious == 200.0);

    /* Simulate step */
    inst->hspeed = 5.0;
    inst->vspeed = 10.0;
    rt.running = 1;
    gm82_runtime_step(&rt);

    assert(inst->xprevious == 100.0);
    assert(inst->yprevious == 200.0);
    assert(inst->x == 105.0);
    assert(inst->y == 210.0);

    /* GML eval getters */
    double val = 0;
    gm82_gml_eval_expr(&rt, inst, "xstart", &val);
    assert(val == 100.0);
    gm82_gml_eval_expr(&rt, inst, "ystart", &val);
    assert(val == 200.0);
    gm82_gml_eval_expr(&rt, inst, "xprevious", &val);
    assert(val == 100.0);
    gm82_gml_eval_expr(&rt, inst, "yprevious", &val);
    assert(val == 200.0);

    printf("  test_instance_start_previous_vars PASS\n");
}

static void test_scaled_collision_meeting(void) {
    printf("Testing scaled sprite bounding box collision...\n");
    gm82_runtime rt;
    gm82_runtime_init(&rt);

    gm82_instance *inst1 = gm82_runtime_instance_create(&rt, 0, 10.0, 10.0);
    gm82_instance *inst2 = gm82_runtime_instance_create(&rt, 1, 30.0, 10.0);
    inst1->image_xscale = 2.0; /* 16*2 = 32 wide: [10, 42] */

    gm82_gml_set_runtime(&rt);
    gm82_gml_set_self(inst1);

    double meeting = gml_place_meeting(10.0, 10.0, 1);
    assert(meeting == 1.0);

    printf("  test_scaled_collision_meeting PASS\n");
}

int main(void) {
    printf("=== Testing GML Physics & Simulation Test Suite ===\n");
    test_instance_start_previous_vars();
    test_scaled_collision_meeting();
    printf("GML_PHYSICS_SIMULATION_TEST_PASS\n");
    return 0;
}
