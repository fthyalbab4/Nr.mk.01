#include "gml_vm.h"
#include "gm82_runtime.h"
#include "gm82_gml_builtins.h"
#include "gm82_gml_eval.h"
#include "gml_frontend.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main(void) {
    gm82_runtime rt;
    gm82_runtime_init(&rt);

    gm82_decoded_frame frame0;
    memset(&frame0, 0, sizeof(frame0));
    frame0.width = 16;
    frame0.height = 16;
    frame0.xoffset = 8;
    frame0.yoffset = 8;
    uint8_t dummy_rgba[16 * 16 * 4];
    memset(dummy_rgba, 255, sizeof(dummy_rgba));
    frame0.rgba = dummy_rgba;

    gm82_decoded_sprite_list sprites;
    memset(&sprites, 0, sizeof(sprites));
    sprites.count = 1;
    sprites.frames = &frame0;

    gm82_decoded_object_list objects;
    memset(&objects, 0, sizeof(objects));
    gm82_decoded_object obj0;
    memset(&obj0, 0, sizeof(obj0));
    obj0.sprite_index = 0;
    obj0.solid = 1;
    obj0.visible = 1;
    strcpy(obj0.name, "obj_player");
    objects.count = 1;
    objects.items = &obj0;

    gm82_runtime_bind_assets(&rt, &objects, &sprites, NULL, NULL, NULL);

    gm82_instance *inst1 = gm82_runtime_instance_create(&rt, 0, 100.0, 100.0);
    gm82_instance *inst2 = gm82_runtime_instance_create(&rt, 0, 105.0, 105.0);
    assert(inst1 != NULL);
    assert(inst2 != NULL);

    gm82_gml_set_runtime(&rt);
    gm82_gml_set_self(inst1);

    /* Test 1: Origin-aware Bounding Box calculations */
    double l = gml_get_bbox_left();
    double t = gml_get_bbox_top();
    double r = gml_get_bbox_right();
    double b = gml_get_bbox_bottom();

    assert(l == 92.0);
    assert(t == 92.0);
    assert(r == 107.0);
    assert(b == 107.0);

    /* Test 2: Origin-aware place_meeting and instance_place */
    double meet = gml_place_meeting(100.0, 100.0, 0);
    assert(meet == 1.0);

    double found_id = gml_instance_place(100.0, 100.0, 0);
    assert(found_id == (double)inst2->id);

    /* Test 3: AST VM Execution for window caption & draw_sprite_part */
    gml_vm vm;
    gml_vm_init(&vm);
    char errbuf[256];

    gml_ast *ast1 = NULL;
    int parse_ok1 = gml_parse_program("window_set_caption('GM82 Parity Game');", &ast1, errbuf, sizeof(errbuf));
    assert(parse_ok1 && ast1 != NULL);
    int ok1 = gml_vm_execute(&vm, ast1);
    assert(ok1);
    assert(strcmp(gml_window_get_caption(), "GM82 Parity Game") == 0);
    gml_ast_free(ast1);

    gml_ast *ast2 = NULL;
    int parse_ok2 = gml_parse_program("draw_sprite_part(0, 0, 0, 0, 8, 8, 10, 10);", &ast2, errbuf, sizeof(errbuf));
    assert(parse_ok2 && ast2 != NULL);
    int ok2 = gml_vm_execute(&vm, ast2);
    assert(ok2);
    gml_ast_free(ast2);

    /* Test 4: GML evaluator execution */
    int eval_ok = gm82_gml_eval_stmt(&rt, inst1, "window_set_caption('Evaluator Title')");
    assert(eval_ok);
    assert(strcmp(gml_window_get_caption(), "Evaluator Title") == 0);

    printf("TEST_GML_VM_EXPANDED_V17: PASS\n");
    return 0;
}
