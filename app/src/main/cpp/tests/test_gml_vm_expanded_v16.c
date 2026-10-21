#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v16] Starting...\n");

    /* 1. Test point_in_triangle */
    double in_tri = gml_point_in_triangle(2.0, 2.0, 0.0, 0.0, 5.0, 0.0, 0.0, 5.0);
    assert(in_tri == 1.0);

    double out_tri = gml_point_in_triangle(10.0, 10.0, 0.0, 0.0, 5.0, 0.0, 0.0, 5.0);
    assert(out_tri == 0.0);

    /* 2. Test binary file I/O functions */
    const char *test_bin_path = "/tmp/test_gm82_v16.bin";
    double handle = gml_file_bin_open(test_bin_path, 1); /* write mode */
    assert(handle >= 0);

    double w1 = gml_file_bin_write_byte(handle, 0x41); /* 'A' */
    double w2 = gml_file_bin_write_byte(handle, 0x42); /* 'B' */
    double w3 = gml_file_bin_write_byte(handle, 0x43); /* 'C' */
    assert(w1 == 1.0 && w2 == 1.0 && w3 == 1.0);

    double bin_sz = gml_file_bin_size(handle);
    assert(bin_sz == 3.0);

    gml_file_bin_close(handle);

    /* Open for read */
    handle = gml_file_bin_open(test_bin_path, 0); /* read mode */
    assert(handle >= 0);

    double b1 = gml_file_bin_read_byte(handle);
    assert(b1 == 0x41);

    double pos1 = gml_file_bin_pos(handle);
    assert(pos1 == 1.0);

    gml_file_bin_seek(handle, 2.0);
    double b3 = gml_file_bin_read_byte(handle);
    assert(b3 == 0x43);

    gml_file_bin_close(handle);

    /* 3. Test string_hash_to_newline */
    char str_out[256];
    gml_string_hash_to_newline("Hello#World\\#Tag", str_out, sizeof(str_out));
    assert(strcmp(str_out, "Hello\nWorld#Tag") == 0);

    /* 4. Test AST VM invocation for new built-ins */
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast arg_px = { GML_AST_NUMBER, .number = 1.0 };
    gml_ast arg_py = { GML_AST_NUMBER, .number = 1.0 };
    gml_ast arg_x1 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast arg_y1 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast arg_x2 = { GML_AST_NUMBER, .number = 4.0 };
    gml_ast arg_y2 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast arg_x3 = { GML_AST_NUMBER, .number = 0.0 };
    gml_ast arg_y3 = { GML_AST_NUMBER, .number = 4.0 };

    const gml_ast *call_args[8] = {
        &arg_px, &arg_py, &arg_x1, &arg_y1, &arg_x2, &arg_y2, &arg_x3, &arg_y3
    };

    gml_ast call_tri = {
        GML_AST_CALL,
        .text = "point_in_triangle",
        .items = (gml_ast**)call_args,
        .count = 8
    };

    gml_ast ret_stmt = {
        GML_AST_RETURN,
        .left = &call_tri
    };

    int ok = gml_vm_execute(&vm, &ret_stmt);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 1.0);

    printf("[test_gml_vm_expanded_v16] PASS\n");
    return 0;
}
