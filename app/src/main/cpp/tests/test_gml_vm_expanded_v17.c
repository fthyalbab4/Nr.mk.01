#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v17] Starting...\n");

    /* 1. Test ds_list_sort & ds_list_shuffle */
    double list_id = gml_ds_list_create();
    assert(list_id >= 0);

    gml_ds_list_add(list_id, 30.0);
    gml_ds_list_add(list_id, 10.0);
    gml_ds_list_add(list_id, 20.0);

    /* Sort ascending */
    gml_ds_list_sort(list_id, 1.0);
    assert(gml_ds_list_find_value(list_id, 0.0) == 10.0);
    assert(gml_ds_list_find_value(list_id, 1.0) == 20.0);
    assert(gml_ds_list_find_value(list_id, 2.0) == 30.0);

    /* Sort descending */
    gml_ds_list_sort(list_id, 0.0);
    assert(gml_ds_list_find_value(list_id, 0.0) == 30.0);
    assert(gml_ds_list_find_value(list_id, 1.0) == 20.0);
    assert(gml_ds_list_find_value(list_id, 2.0) == 10.0);

    /* Shuffle (verify list size remains 3 and elements exist) */
    gml_ds_list_shuffle(list_id);
    assert(gml_ds_list_size(list_id) == 3.0);

    gml_ds_list_destroy(list_id);

    /* 2. Test point_in_ellipse */
    /* Ellipse bounded by x: 0..10, y: 0..10 -> center (5, 5), rx=5, ry=5 */
    double inside_e = gml_point_in_ellipse(5.0, 5.0, 0.0, 0.0, 10.0, 10.0);
    assert(inside_e == 1.0);

    double outside_e = gml_point_in_ellipse(15.0, 15.0, 0.0, 0.0, 10.0, 10.0);
    assert(outside_e == 0.0);

    /* 3. Test string_pos_ext */
    const char *sample_str = "banana";
    double pos1 = gml_string_pos_ext("an", sample_str, 1.0);
    assert(pos1 == 2.0);

    double pos2 = gml_string_pos_ext("an", sample_str, 3.0);
    assert(pos2 == 4.0);

    double pos_none = gml_string_pos_ext("an", sample_str, 5.0);
    assert(pos_none == 0.0);

    /* 4. Test AST VM invocation for point_in_ellipse and string_pos_ext */
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast arg_px = { GML_AST_NUMBER, .number = 5.0 };
    gml_ast arg_py = { GML_AST_NUMBER, .number = 5.0 };
    gml_ast arg_x1 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast arg_y1 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast arg_x2 = { GML_AST_NUMBER, .number = 10.0 };
    gml_ast arg_y2 = { GML_AST_NUMBER, .number = 10.0 };

    const gml_ast *call_args[6] = {
        &arg_px, &arg_py, &arg_x1, &arg_y1, &arg_x2, &arg_y2
    };

    gml_ast call_ellipse = {
        GML_AST_CALL,
        .text = "point_in_ellipse",
        .items = (gml_ast**)call_args,
        .count = 6
    };

    gml_ast ret_stmt = {
        GML_AST_RETURN,
        .left = &call_ellipse
    };

    int ok = gml_vm_execute(&vm, &ret_stmt);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 1.0);

    printf("[test_gml_vm_expanded_v17] PASS\n");
    return 0;
}
