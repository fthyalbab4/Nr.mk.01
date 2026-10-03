#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "gml_vm.h"
#include "gml_frontend.h"
#include "gm82_gml_builtins.h"

static void test_hsv_and_merge_colors(void) {
    gml_vm vm;
    gml_vm_init(&vm);
    char err[256];
    gml_ast *ast1 = NULL;

    /* Test make_color_hsv and hue/sat/val retrieval */
    assert(gml_parse_program("c = make_color_hsv(0, 255, 255); r = color_get_red(c); g = color_get_green(c); b = color_get_blue(c);", &ast1, err, sizeof(err)));
    assert(ast1 != NULL);
    assert(gml_vm_execute(&vm, ast1));
    gml_value c_val = gml_vm_get(&vm, "c");
    gml_value r_val = gml_vm_get(&vm, "r");
    assert(r_val.real == 255.0);
    gml_value_free(&c_val);
    gml_value_free(&r_val);
    gml_ast_free(ast1);

    /* Test merge_color */
    gml_ast *ast2 = NULL;
    assert(gml_parse_program("c1 = make_color_rgb(0, 0, 0); c2 = make_color_rgb(100, 200, 50); cm = merge_color(c1, c2, 0.5); mr = color_get_red(cm); mg = color_get_green(cm);", &ast2, err, sizeof(err)));
    assert(ast2 != NULL);
    assert(gml_vm_execute(&vm, ast2));
    gml_value mr_val = gml_vm_get(&vm, "mr");
    gml_value mg_val = gml_vm_get(&vm, "mg");
    assert(fabs(mr_val.real - 50.0) < 1.0);
    assert(fabs(mg_val.real - 100.0) < 1.0);
    gml_value_free(&mr_val);
    gml_value_free(&mg_val);
    gml_ast_free(ast2);

    printf("test_hsv_and_merge_colors PASS\n");
}

static void test_string_replace_and_bytes(void) {
    gml_vm vm;
    gml_vm_init(&vm);
    char err[256];
    gml_ast *ast = NULL;

    assert(gml_parse_program("s = string_replace(\"Hello World\", \"World\", \"NOR Maker\"); blen = string_byte_length(s); b1 = string_byte_at(s, 1);", &ast, err, sizeof(err)));
    assert(ast != NULL);
    assert(gml_vm_execute(&vm, ast));

    gml_value s_val = gml_vm_get(&vm, "s");
    assert(s_val.kind == GML_V_STRING);
    assert(strcmp(s_val.string, "Hello NOR Maker") == 0);

    gml_value blen_val = gml_vm_get(&vm, "blen");
    assert(blen_val.real == 15.0);

    gml_value b1_val = gml_vm_get(&vm, "b1");
    assert(b1_val.real == (double)'H');

    gml_value_free(&s_val);
    gml_value_free(&blen_val);
    gml_value_free(&b1_val);
    gml_ast_free(ast);

    printf("test_string_replace_and_bytes PASS\n");
}

static void test_gml_vm_ds_structures(void) {
    gml_vm vm;
    gml_vm_init(&vm);
    char err[256];
    gml_ast *ast = NULL;

    assert(gml_parse_program("lst = ds_list_create(); ds_list_add(lst, 42); ds_list_add(lst, 99); sz = ds_list_size(lst); v0 = ds_list_find_value(lst, 0); ds_list_destroy(lst);", &ast, err, sizeof(err)));
    assert(ast != NULL);
    assert(gml_vm_execute(&vm, ast));

    gml_value sz_val = gml_vm_get(&vm, "sz");
    assert(sz_val.real == 2.0);

    gml_value v0_val = gml_vm_get(&vm, "v0");
    assert(v0_val.real == 42.0);

    gml_value_free(&sz_val);
    gml_value_free(&v0_val);
    gml_ast_free(ast);

    printf("test_gml_vm_ds_structures PASS\n");
}

static void test_gml_vm_ds_grid(void) {
    gml_vm vm;
    gml_vm_init(&vm);
    char err[256];
    gml_ast *ast = NULL;

    assert(gml_parse_program(
        "grid = ds_grid_create(4, 4);"
        "w = ds_grid_width(grid);"
        "h = ds_grid_height(grid);"
        "ds_grid_clear(grid, 5);"
        "ds_grid_set(grid, 2, 3, 42);"
        "ds_grid_add(grid, 2, 3, 8);"
        "v_clear = ds_grid_get(grid, 0, 0);"
        "v_cell = ds_grid_get(grid, 2, 3);"
        "ds_grid_destroy(grid);",
        &ast, err, sizeof(err)));
    assert(ast != NULL);
    assert(gml_vm_execute(&vm, ast));

    gml_value w_val = gml_vm_get(&vm, "w");
    assert(w_val.real == 4.0);

    gml_value h_val = gml_vm_get(&vm, "h");
    assert(h_val.real == 4.0);

    gml_value vc_val = gml_vm_get(&vm, "v_clear");
    assert(vc_val.real == 5.0);

    gml_value vcell_val = gml_vm_get(&vm, "v_cell");
    assert(vcell_val.real == 50.0);

    gml_value_free(&w_val);
    gml_value_free(&h_val);
    gml_value_free(&vc_val);
    gml_value_free(&vcell_val);
    gml_ast_free(ast);

    printf("test_gml_vm_ds_grid PASS\n");
}

static void test_gml_vm_ini_io(void) {
    gml_vm vm;
    gml_vm_init(&vm);
    char err[256];
    gml_ast *ast = NULL;

    assert(gml_parse_program("ini_open(\"/tmp/test_full_support.ini\"); ini_write_real(\"player\", \"score\", 1500); val = ini_read_real(\"player\", \"score\", 0); ex = ini_key_exists(\"player\", \"score\"); ini_close();", &ast, err, sizeof(err)));
    assert(ast != NULL);
    assert(gml_vm_execute(&vm, ast));

    gml_value val_res = gml_vm_get(&vm, "val");
    assert(val_res.real == 1500.0);

    gml_value ex_res = gml_vm_get(&vm, "ex");
    assert(ex_res.real == 1.0);

    gml_value_free(&val_res);
    gml_value_free(&ex_res);
    gml_ast_free(ast);

    remove("/tmp/test_full_support.ini");
    printf("test_gml_vm_ini_io PASS\n");
}

int main(void) {
    printf("=== Testing GML Full Support VM Suite ===\n");
    test_hsv_and_merge_colors();
    test_string_replace_and_bytes();
    test_gml_vm_ds_structures();
    test_gml_vm_ds_grid();
    test_gml_vm_ini_io();
    printf("GML_FULL_SUPPORT_TEST_PASS\n");
    return 0;
}
