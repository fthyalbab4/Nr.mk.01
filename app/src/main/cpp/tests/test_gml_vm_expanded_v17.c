#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gml_frontend.h"
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

static gml_ast* parse_code(const char* code) {
    gml_ast* ast = NULL;
    char err[256] = {0};
    int res = gml_parse_program(code, &ast, err, sizeof(err));
    if (!res) {
        fprintf(stderr, "Parse error: %s\n", err);
        return NULL;
    }
    return ast;
}

int main(void) {
    /* Direct C Builtin Verification */
    char out_buf[256];

    /* 1. string_replace_all, string_delete, string_insert */
    gml_string_replace_all("hello world", "o", "0", out_buf, sizeof(out_buf));
    assert(strcmp(out_buf, "hell0 w0rld") == 0);

    gml_string_delete("abcdef", 3, 2, out_buf, sizeof(out_buf));
    assert(strcmp(out_buf, "abef") == 0);

    gml_string_insert("123", "abcdef", 3, out_buf, sizeof(out_buf));
    assert(strcmp(out_buf, "ab123cdef") == 0);

    /* 2. Color functions */
    double col = gml_make_color_rgb(128, 64, 200);
    assert(gml_color_get_red(col) == 128);
    assert(gml_color_get_green(col) == 64);
    assert(gml_color_get_blue(col) == 200);

    /* 3. Math functions */
    assert(gml_clamp(15, 0, 10) == 10);
    assert(gml_lerp(10, 20, 0.5) == 15);
    assert(gml_point_distance(0, 0, 3, 4) == 5);
    assert(gml_point_direction(0, 0, 10, 0) == 0);

    /* 4. AST VM Execution Verification */
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast* ast1 = parse_code("s = \"hello world\"; r = string_replace_all(s, \"o\", \"0\");");
    assert(ast1 != NULL);
    int ok = gml_vm_execute(&vm, ast1);
    assert(ok);
    gml_value val1 = gml_vm_get(&vm, "r");
    assert(val1.kind == GML_V_STRING);
    assert(val1.string != NULL && strcmp(val1.string, "hell0 w0rld") == 0);
    gml_value_free(&val1);
    gml_ast_free(ast1);

    gml_ast* ast2 = parse_code("c = clamp(15, 0, 10); l = lerp(10, 20, 0.5);");
    assert(ast2 != NULL);
    ok = gml_vm_execute(&vm, ast2);
    assert(ok);
    gml_value val_c = gml_vm_get(&vm, "c");
    gml_value val_l = gml_vm_get(&vm, "l");
    assert(val_c.kind == GML_V_REAL && val_c.real == 10);
    assert(val_l.kind == GML_V_REAL && val_l.real == 15);
    gml_value_free(&val_c);
    gml_value_free(&val_l);
    gml_ast_free(ast2);

    printf("test_gml_vm_expanded_v17 PASSED!\n");
    return 0;
}
