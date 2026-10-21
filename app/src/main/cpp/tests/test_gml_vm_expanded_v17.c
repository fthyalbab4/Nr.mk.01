#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"
#include "gm82_runtime.h"
#include "gm82_sprite_decode.h"

int main(void) {
    printf("Running test_gml_vm_expanded_v17...\n");

    /* 1. Test game_save and game_load builtins */
    gm82_runtime rt;
    memset(&rt, 0, sizeof(rt));

    gm82_decoded_room_list rooms;
    memset(&rooms, 0, sizeof(rooms));
    rooms.count = 3;
    gm82_decoded_room room_items[3];
    memset(room_items, 0, sizeof(room_items));
    for (int i = 0; i < 3; i++) {
        room_items[i].width = 640;
        room_items[i].height = 480;
        room_items[i].speed = 30;
    }
    rooms.items = room_items;

    rt.rooms = &rooms;
    gm82_runtime_goto_room(&rt, 2);
    rt.instance_count = 5;
    gm82_gml_set_runtime(&rt);

    gml_set_score(1500);
    gml_set_lives(5);
    gml_set_health(80);

    const char *save_file = "test_save_v17.dat";
    double save_res = gml_game_save(save_file);
    assert(save_res == 1.0);

    /* Modify state */
    gml_set_score(0);
    gml_set_lives(1);
    gml_set_health(10);
    gm82_runtime_goto_room(&rt, 0);

    double load_res = gml_game_load(save_file);
    assert(load_res == 1.0);
    assert(rt.current_room == 2);
    assert(gml_get_score() == 1500);
    assert(gml_get_lives() == 5);
    assert(gml_get_health() == 80);

    remove(save_file);

    /* 2. Test draw_sprite_part, draw_sprite_tiled, and text builtins via framebuffer */
    uint8_t buf[64 * 64 * 4];
    memset(buf, 0, sizeof(buf));
    gm82_draw_set_target(buf, 64, 64);

    /* Create dummy sprite list */
    gm82_decoded_sprite_list sprites;
    memset(&sprites, 0, sizeof(sprites));
    sprites.count = 1;
    sprites.frames = calloc(1, sizeof(gm82_decoded_frame));
    sprites.frames[0].width = 16;
    sprites.frames[0].height = 16;
    sprites.frames[0].rgba = calloc(16 * 16 * 4, 1);
    for (int i = 0; i < 16 * 16; i++) {
        sprites.frames[0].rgba[i * 4 + 0] = 255; /* Red */
        sprites.frames[0].rgba[i * 4 + 3] = 255;
    }
    rt.sprites = &sprites;

    gml_draw_sprite_part(0, 0, 0, 0, 8, 8, 10, 10);
    /* Check pixel drawn at (10,10) */
    uint8_t *p1 = buf + (10 * 64 + 10) * 4;
    assert(p1[0] == 255 && p1[3] == 255);

    gml_draw_sprite_tiled(0, 0, 0, 0);
    uint8_t *p2 = buf + (0 * 64 + 0) * 4;
    assert(p2[0] == 255 && p2[3] == 255);

    gml_draw_text_transformed(0, 0, "TEST", 1.0, 1.0, 0);

    free(sprites.frames[0].rgba);
    free(sprites.frames);

    /* 3. Test GML AST VM execution of new builtins */
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast arg_save = { GML_AST_STRING, .text = "vm_save.dat" };
    const gml_ast *save_args[1] = { &arg_save };
    gml_ast call_save = {
        GML_AST_CALL,
        .text = "game_save",
        .items = (gml_ast**)save_args,
        .count = 1
    };

    int ok = gml_vm_execute(&vm, &call_save);
    assert(ok == 1);
    remove("vm_save.dat");

    printf("test_gml_vm_expanded_v17 passed successfully!\n");
    return 0;
}
