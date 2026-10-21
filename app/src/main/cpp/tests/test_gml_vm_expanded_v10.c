#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "gml_frontend.h"
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    // 1. Test point_in_rectangle
    {
        const char* script = "res = point_in_rectangle(5, 5, 0, 0, 10, 10);";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);
        gml_value v = gml_vm_get(&vm, "res");
        assert(v.kind == GML_V_REAL || v.kind == GML_V_BOOL);
        assert(v.real == 1.0);
        gml_value_free(&v);
        gml_ast_free(ast);
    }

    // 2. Test string_replace and string_replace_all
    {
        const char* script = "s1 = string_replace(\"hello world\", \"world\", \"gml\"); s2 = string_replace_all(\"banana\", \"a\", \"o\");";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);
        gml_value v1 = gml_vm_get(&vm, "s1");
        assert(v1.kind == GML_V_STRING);
        assert(strcmp(v1.string, "hello gml") == 0);
        gml_value_free(&v1);

        gml_value v2 = gml_vm_get(&vm, "s2");
        assert(v2.kind == GML_V_STRING);
        assert(strcmp(v2.string, "bonono") == 0);
        gml_value_free(&v2);
        gml_ast_free(ast);
    }

    // 3. Test File Text Operations
    {
        const char* test_file = "/tmp/test_gml_vm_v10.txt";
        unlink(test_file);

        const char* script_write =
            "f = file_text_open_append(\"/tmp/test_gml_vm_v10.txt\");\n"
            "file_text_write_string(f, \"line1\");\n"
            "file_text_writeln(f);\n"
            "file_text_write_real(f, 42.5);\n"
            "file_text_close(f);\n";

        gml_ast* ast_write = NULL;
        int ok_write = gml_parse_program(script_write, &ast_write, NULL, 0);
        assert(ok_write && ast_write != NULL);
        int exec_write = gml_vm_execute(&vm, ast_write);
        assert(exec_write);
        gml_ast_free(ast_write);

        const char* script_read =
            "f = file_text_open_read(\"/tmp/test_gml_vm_v10.txt\");\n"
            "str = file_text_read_string(f);\n"
            "file_text_readln(f);\n"
            "val = file_text_read_real(f);\n"
            "file_text_close(f);\n";

        gml_ast* ast_read = NULL;
        int ok_read = gml_parse_program(script_read, &ast_read, NULL, 0);
        assert(ok_read && ast_read != NULL);
        int exec_read = gml_vm_execute(&vm, ast_read);
        assert(exec_read);

        gml_value str_val = gml_vm_get(&vm, "str");
        assert(str_val.kind == GML_V_STRING);
        assert(strcmp(str_val.string, "line1") == 0);
        gml_value_free(&str_val);

        gml_value real_val = gml_vm_get(&vm, "val");
        assert(real_val.kind == GML_V_REAL);
        assert(real_val.real == 42.5);
        gml_value_free(&real_val);

        gml_ast_free(ast_read);
        unlink(test_file);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v10 PASSED SUCCESSFULLY.\n");
    return 0;
}
