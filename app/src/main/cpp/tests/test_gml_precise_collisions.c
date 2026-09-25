#include "gm82_runtime.h"
#include "gm82_gml_builtins.h"
#include "gm82_sprite_decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void test_string_builtins(void) {
    char buf[128];
    double len = gml_string_copy("Hello NOR Maker", 1, 5, buf, sizeof(buf));
    assert(len == 5);
    assert(strcmp(buf, "Hello") == 0);

    gml_string_replace("Hello World", "World", "Maker", buf, sizeof(buf));
    assert(strcmp(buf, "Hello Maker") == 0);

    gml_string_replace_all("a_b_c_d", "_", "-", buf, sizeof(buf));
    assert(strcmp(buf, "a-b-c-d") == 0);

    puts("test_string_builtins PASS");
}

static void test_precise_collisions(void) {
    gm82_runtime rt;
    gm82_runtime_init(&rt);

    gm82_decoded_frame frames[2];
    memset(frames, 0, sizeof(frames));

    /* Frame 0: 10x10 sprite with active pixel only at (5,5) */
    frames[0].width = 10;
    frames[0].height = 10;
    frames[0].rgba = (uint8_t *)calloc(10 * 10 * 4, 1);
    frames[0].mask = (uint8_t *)calloc(10 * 10, 1);
    frames[0].mask[5 * 10 + 5] = 1;

    /* Frame 1: 10x10 sprite with active pixel only at (5,5) */
    frames[1].width = 10;
    frames[1].height = 10;
    frames[1].rgba = (uint8_t *)calloc(10 * 10 * 4, 1);
    frames[1].mask = (uint8_t *)calloc(10 * 10, 1);
    frames[1].mask[5 * 10 + 5] = 1;

    gm82_decoded_sprite_list slist;
    slist.count = 2;
    slist.frames = frames;

    rt.sprites = &slist;
    gm82_gml_set_runtime(&rt);

    gm82_instance *inst1 = gm82_runtime_instance_create(&rt, 0, 0, 0);
    inst1->sprite_index = 0;

    gm82_instance *inst2 = gm82_runtime_instance_create(&rt, 0, 0, 0);
    inst2->sprite_index = 1;

    gm82_gml_set_self(inst1);

    /* Bounding box overlaps (both 10x10 at 0,0) */
    assert(gml_place_meeting(0, 0, 0) == 1.0);

    /* Offset inst2 so active pixels do not align (inst2 at (1,0) -> pixel at (6,5) instead of (5,5)) */
    inst2->x = 1.0;
    assert(gml_place_meeting(0, 0, 0) == 0.0);

    /* Precise point collision test at active pixel (5,5) vs inactive pixel (0,0) */
    assert(gml_collision_point(5, 5, 0, 1, 0) == (double)inst1->id);
    assert(gml_collision_point(0, 0, 0, 1, 0) == -4.0);

    free(frames[0].rgba); free(frames[0].mask);
    free(frames[1].rgba); free(frames[1].mask);

    puts("test_precise_collisions PASS");
}

int main(void) {
    puts("=== Testing GML Precise Collisions & String Extensions ===");
    test_string_builtins();
    test_precise_collisions();
    puts("GML_PRECISE_COLLISIONS_TEST_PASS");
    return 0;
}
