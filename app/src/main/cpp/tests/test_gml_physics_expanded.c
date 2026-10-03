#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "gm82_runtime.h"
#include "gm82_gml_builtins.h"
#include "gm82_gml_eval.h"
#include "gm82_sprite_decode.h"

int main(void) {
    printf("=== Testing GML Physics & Builtins Expanded Suite ===\n");

    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_gml_set_runtime(&rt);

    /* Create dummy sprite list */
    gm82_decoded_sprite_list sprites;
    gm82_decoded_sprite_list_init(&sprites);
    sprites.count = 1;
    sprites.frames = calloc(1, sizeof(gm82_decoded_frame));
    sprites.frames[0].width = 32;
    sprites.frames[0].height = 32;
    sprites.frames[0].xoffset = 16;
    sprites.frames[0].yoffset = 16;
    rt.sprites = &sprites;

    gm82_instance *inst = gm82_runtime_instance_create(&rt, 0, 100, 100);
    assert(inst != NULL);
    gm82_gml_set_self(inst);

    /* Test sprite offsets */
    double xoff = 0, yoff = 0;
    gm82_gml_eval_expr(&rt, inst, "sprite_get_xoffset(0)", &xoff);
    gm82_gml_eval_expr(&rt, inst, "sprite_get_yoffset(0)", &yoff);
    assert((int)xoff == 16);
    assert((int)yoff == 16);
    printf("test_sprite_offsets PASS\n");

    /* Test instance furthest search */
    gm82_instance *inst2 = gm82_runtime_instance_create(&rt, 0, 500, 500);
    (void)inst2;
    double furthest_id = -1;
    gm82_gml_eval_expr(&rt, inst, "instance_furthest(100, 100, -1)", &furthest_id);
    assert((int)furthest_id == inst2->id);
    printf("test_instance_furthest PASS\n");

    /* Test move_bounce_solid logic */
    inst->hspeed = 4;
    inst->vspeed = -4;
    gm82_instance *solid_obstacle = gm82_runtime_instance_create(&rt, 1, 104, 100);
    solid_obstacle->solid = 1;

    double bounce_res = 0;
    gm82_gml_eval_expr(&rt, inst, "move_bounce_solid(0)", &bounce_res);
    assert(bounce_res == 1.0);
    assert(inst->hspeed == -4.0);
    printf("test_move_bounce_solid PASS\n");

    /* Test position_destroy */
    double destroyed = 0;
    gm82_gml_eval_expr(&rt, inst, "position_destroy(500, 500)", &destroyed);
    assert(destroyed == 1.0);
    assert(inst2->alive == 0);
    printf("test_position_destroy PASS\n");

    gm82_decoded_sprite_list_free(&sprites);

    printf("GML_PHYSICS_EXPANDED_TEST_PASS\n");
    return 0;
}
