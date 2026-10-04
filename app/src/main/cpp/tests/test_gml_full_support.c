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

    assert(gml_parse_program("s = string_replace_all(\"Hello World World\", \"World\", \"NOR\"); letters = string_letters(\"123ABC456!\"); blen = string_byte_length(s); b1 = string_byte_at(s, 1);", &ast, err, sizeof(err)));
    assert(ast != NULL);
    assert(gml_vm_execute(&vm, ast));

    gml_value s_val = gml_vm_get(&vm, "s");
    assert(s_val.kind == GML_V_STRING);
    assert(strcmp(s_val.string, "Hello NOR NOR") == 0);

    gml_value let_val = gml_vm_get(&vm, "letters");
    assert(let_val.kind == GML_V_STRING);
    assert(strcmp(let_val.string, "ABC") == 0);

    gml_value blen_val = gml_vm_get(&vm, "blen");
    assert(blen_val.real == 13.0);

    gml_value b1_val = gml_vm_get(&vm, "b1");
    assert(b1_val.real == (double)'H');

    gml_value_free(&s_val);
    gml_value_free(&let_val);
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

    assert(gml_parse_program("m = ds_map_create(); ds_map_add(m, 10, 500); ex1 = ds_map_exists(m, 10); ex2 = ds_map_exists(m, 99); ds_map_delete(m, 10); ex3 = ds_map_exists(m, 10); ds_map_destroy(m);", &ast, err, sizeof(err)));
    assert(ast != NULL);
    assert(gml_vm_execute(&vm, ast));

    gml_value ex1 = gml_vm_get(&vm, "ex1");
    gml_value ex2 = gml_vm_get(&vm, "ex2");
    gml_value ex3 = gml_vm_get(&vm, "ex3");
    assert(ex1.real == 1.0);
    assert(ex2.real == 0.0);
    assert(ex3.real == 0.0);

    gml_value_free(&ex1);
    gml_value_free(&ex2);
    gml_value_free(&ex3);
    gml_ast_free(ast);

    printf("test_gml_vm_ds_structures PASS\n");
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
    test_gml_vm_ini_io();
    printf("GML_FULL_SUPPORT_TEST_PASS\n");
    return 0;
}
