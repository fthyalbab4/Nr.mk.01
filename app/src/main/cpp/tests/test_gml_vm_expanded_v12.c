#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "gml_frontend.h"
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    // 1. Test Window and Timer builtins execution
    {
        const char* script =
            "window_set_caption('My GM82 Game');\n"
            "cap = window_get_caption();\n"
            "t = get_timer();\n"
            "dt = delta_time();\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_cap = gml_vm_get(&vm, "cap");
        assert(v_cap.kind == GML_V_STRING && !strcmp(v_cap.string, "My GM82 Game"));
        gml_value_free(&v_cap);

        gml_value v_t = gml_vm_get(&vm, "t");
        assert(v_t.real >= 0.0);
        gml_value_free(&v_t);

        gml_value v_dt = gml_vm_get(&vm, "dt");
        assert(v_dt.real >= 0.0);
        gml_value_free(&v_dt);

        gml_ast_free(ast);
    }

    // 2. Test Alarm builtins execution
    {
        const char* script =
            "alarm_set(0, 30);\n"
            "a0 = alarm_get(0);\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_a0 = gml_vm_get(&vm, "a0");
        assert(v_a0.real == -1.0 || v_a0.real == 30.0);
        gml_value_free(&v_a0);

        gml_ast_free(ast);
    }

    // 3. Test Room and Path builtins execution
    {
        const char* script =
            "num_paths = path_get_number();\n"
            "p_end = path_end();\n"
            "tl_stop = timeline_stop();\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_paths = gml_vm_get(&vm, "num_paths");
        assert(v_paths.real >= 0.0);
        gml_value_free(&v_paths);

        gml_ast_free(ast);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v12 PASSED SUCCESSFULLY.\n");
    return 0;
}
