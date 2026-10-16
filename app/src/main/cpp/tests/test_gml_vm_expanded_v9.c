#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

int main(void) {
    printf("=== Testing GML VM Expanded V9 Suite ===\n");

    /* Test point_in_circle */
    assert(gml_point_in_circle(5, 5, 5, 5, 10) == 1.0);
    assert(gml_point_in_circle(20, 20, 0, 0, 5) == 0.0);

    /* Test ds_grid_value_* functions */
    double grid = gml_ds_grid_create(5, 5);
    gml_ds_grid_set(grid, 2, 3, 42.0);
    assert(gml_ds_grid_value_exists(grid, 0, 0, 4, 4, 42.0) == 1.0);
    assert(gml_ds_grid_value_exists(grid, 0, 0, 4, 4, 99.0) == 0.0);
    assert(gml_ds_grid_value_x(grid, 0, 0, 4, 4, 42.0) == 2.0);
    assert(gml_ds_grid_value_y(grid, 0, 0, 4, 4, 42.0) == 3.0);
    assert(gml_ds_grid_value_x(grid, 0, 0, 4, 4, 99.0) == -1.0);
    gml_ds_grid_destroy(grid);

    /* Test text file read string / readln */
    double wf = gml_file_text_open_write("/tmp/test_v9.txt");
    assert(wf >= 0);
    gml_file_text_write_string(wf, "Line1");
    gml_file_text_writeln(wf);
    gml_file_text_write_string(wf, "Line2");
    gml_file_text_close(wf);

    double rf = gml_file_text_open_read("/tmp/test_v9.txt");
    assert(rf >= 0);
    char buf[128];
    double res1 = gml_file_text_read_string(rf, buf, sizeof(buf));
    assert(res1 > 0);
    assert(strcmp(buf, "Line1") == 0);
    assert(gml_file_text_readln(rf) == 1.0);
    assert(gml_file_text_read_string(rf, buf, sizeof(buf)) > 0);
    assert(strcmp(buf, "Line2") == 0);
    gml_file_text_close(rf);
    gml_file_delete("/tmp/test_v9.txt");

    /* Test VM AST execution of point_in_circle and ds_grid_value_exists */
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast a1 = { .kind = GML_AST_NUMBER, .number = 0.0 };
    gml_ast a2 = { .kind = GML_AST_NUMBER, .number = 0.0 };
    gml_ast a3 = { .kind = GML_AST_NUMBER, .number = 0.0 };
    gml_ast a4 = { .kind = GML_AST_NUMBER, .number = 0.0 };
    gml_ast a5 = { .kind = GML_AST_NUMBER, .number = 10.0 };
    gml_ast *pic_args[5] = { &a1, &a2, &a3, &a4, &a5 };
    gml_ast fn_pic = { .kind = GML_AST_CALL, .text = "point_in_circle", .count = 5, .items = pic_args };
    assert(gml_vm_execute(&vm, &fn_pic));

    printf("  test_gml_vm_expanded_v9 PASS\n");
    printf("GML_VM_EXPANDED_V9_TEST_PASS\n");
    return 0;
}
