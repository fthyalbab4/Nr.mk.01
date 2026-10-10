#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "gm82_runtime.h"
#include "gm82_gml_eval.h"
#include "gm82_gml_builtins.h"

int main(void) {
    printf("=== Testing GML Eval Built-ins Dispatched Suite ===\n");

    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_gml_set_runtime(&rt);

    gm82_instance inst;
    memset(&inst, 0, sizeof(inst));
    inst.id = 100001;
    inst.object_index = 0;
    inst.x = 50;
    inst.y = 100;
    inst.sprite_index = 0;
    inst.image_index = 0;
    inst.image_speed = 1.0;
    inst.image_xscale = 1.0;
    inst.image_yscale = 1.0;

    gm82_gml_set_self(&inst);

    /* Test motion builtins via GML eval */
    double res = 0;
    bool ok = gm82_gml_eval_stmt(&rt, &inst, "motion_set(90, 4);");
    assert(ok);
    assert(inst.direction == 90);
    assert(inst.speed == 4);

    ok = gm82_gml_eval_stmt(&rt, &inst, "motion_add(0, 3);");
    assert(ok);

    /* Test draw builtins via GML eval */
    ok = gm82_gml_eval_stmt(&rt, &inst, "draw_set_color(16711680);");
    assert(ok);
    assert(gml_draw_get_color() == 16711680);

    ok = gm82_gml_eval_stmt(&rt, &inst, "draw_set_alpha(0.5);");
    assert(ok);
    assert(gml_draw_get_alpha() == 0.5);

    /* Test ds_list builtins via GML eval */
    ok = gm82_gml_eval_expr(&rt, &inst, "ds_list_create()", &res);
    assert(ok);
    double list_id = res;

    char stmt_buf[128];
    snprintf(stmt_buf, sizeof(stmt_buf), "ds_list_add(%.0f, 42);", list_id);
    ok = gm82_gml_eval_stmt(&rt, &inst, stmt_buf);
    assert(ok);

    snprintf(stmt_buf, sizeof(stmt_buf), "ds_list_find_value(%.0f, 0)", list_id);
    ok = gm82_gml_eval_expr(&rt, &inst, stmt_buf, &res);
    assert(ok);
    assert(res == 42);

    /* Test instance_number / exists via GML eval */
    ok = gm82_gml_eval_expr(&rt, &inst, "instance_exists(100001)", &res);
    assert(ok);

    /* Test room goto via GML eval */
    ok = gm82_gml_eval_stmt(&rt, &inst, "room_goto(1);");
    assert(ok);

    printf("GML_EVAL_BUILTINS_TEST_PASS\n");
    return 0;
}
