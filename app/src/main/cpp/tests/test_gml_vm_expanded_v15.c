#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "gml_frontend.h"
#include "gml_vm.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    // Test filename_name, filename_path, filename_dir, filename_ext, and filename_change_ext
    {
        const char* script =
            "fn1 = filename_name(\"C:/games/mario/game.gmk\");\n"
            "fp1 = filename_path(\"C:/games/mario/game.gmk\");\n"
            "fd1 = filename_dir(\"C:/games/mario/game.gmk\");\n"
            "fe1 = filename_ext(\"C:/games/mario/game.gmk\");\n"
            "fc1 = filename_change_ext(\"C:/games/mario/game.gmk\", \".exe\");\n";

        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_fn1 = gml_vm_get(&vm, "fn1");
        assert(v_fn1.kind == GML_V_STRING && strcmp(v_fn1.string, "game.gmk") == 0);
        gml_value_free(&v_fn1);

        gml_value v_fp1 = gml_vm_get(&vm, "fp1");
        assert(v_fp1.kind == GML_V_STRING && strcmp(v_fp1.string, "C:/games/mario/") == 0);
        gml_value_free(&v_fp1);

        gml_value v_fd1 = gml_vm_get(&vm, "fd1");
        assert(v_fd1.kind == GML_V_STRING && strcmp(v_fd1.string, "C:/games/mario") == 0);
        gml_value_free(&v_fd1);

        gml_value v_fe1 = gml_vm_get(&vm, "fe1");
        assert(v_fe1.kind == GML_V_STRING && strcmp(v_fe1.string, ".gmk") == 0);
        gml_value_free(&v_fe1);

        gml_value v_fc1 = gml_vm_get(&vm, "fc1");
        assert(v_fc1.kind == GML_V_STRING && strcmp(v_fc1.string, "C:/games/mario/game.exe") == 0);
        gml_value_free(&v_fc1);

        gml_ast_free(ast);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v15 PASSED SUCCESSFULLY.\n");
    return 0;
}
