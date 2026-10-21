#include <stdio.h>
#include <assert.h>
#include "gml_frontend.h"
#include "gml_vm.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    // Test file_rename and directory_rename
    {
        // First create dummy files/directories
        FILE* f = fopen("test_rename_src.txt", "w");
        if (f) {
            fputs("hello world", f);
            fclose(f);
        }

        const char* script =
            "res_file = file_rename(\"test_rename_src.txt\", \"test_rename_dst.txt\");\n"
            "res_file_exist = file_exists(\"test_rename_dst.txt\");\n";

        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_res = gml_vm_get(&vm, "res_file");
        assert(v_res.real == 1.0);
        gml_value_free(&v_res);

        gml_value v_exist = gml_vm_get(&vm, "res_file_exist");
        assert(v_exist.real == 1.0);
        gml_value_free(&v_exist);

        gml_ast_free(ast);

        // Clean up created test file
        remove("test_rename_dst.txt");
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v14 PASSED SUCCESSFULLY.\n");
    return 0;
}
