#include "gm82_gl_textures.h"
#include "gm82_sprite_decode.h"
#include "gm82_background_decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void) {
    puts("=== Testing GLES Texture Atlas Upload & Draw Functions ===");

    gm82_gl_atlas atlas;
    gm82_gl_atlas_init(&atlas);

    /* Construct dummy sprite list */
    gm82_decoded_frame frame;
    memset(&frame, 0, sizeof(frame));
    strncpy(frame.name, "spr_test", sizeof(frame.name)-1);
    frame.width = 16;
    frame.height = 16;
    frame.rgba = (uint8_t *)calloc(16 * 16 * 4, 1);

    gm82_decoded_sprite_list slist;
    memset(&slist, 0, sizeof(slist));
    slist.count = 1;
    slist.frames = &frame;

    assert(gm82_gl_upload_sprites(&atlas, &slist) == true);
    assert(atlas.sprite_count == 1);
    assert(atlas.sprites[0].tex_id != 0);

    /* Construct dummy background list */
    gm82_decoded_background bg;
    memset(&bg, 0, sizeof(bg));
    strncpy(bg.name, "bg_test", sizeof(bg.name)-1);
    bg.width = 32;
    bg.height = 32;
    bg.rgba = (uint8_t *)calloc(32 * 32 * 4, 1);

    gm82_decoded_background_list bglist;
    memset(&bglist, 0, sizeof(bglist));
    bglist.count = 1;
    bglist.items = &bg;

    assert(gm82_gl_upload_backgrounds(&atlas, &bglist) == true);
    assert(atlas.background_count == 1);

    assert(gm82_gl_draw_texture(atlas.sprites[0].tex_id, 10, 10, 16, 16) == true);

    gm82_gl_atlas_free(&atlas);
    free(frame.rgba);
    free(bg.rgba);

    puts("GL_TEXTURES_UPLOAD_TEST_PASS");
    return 0;
}
