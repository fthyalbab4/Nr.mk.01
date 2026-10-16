#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "gml_frontend.h"
#include "gml_vm.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    // 1. Test ds_list_sort and ds_list_reverse
    {
        const char* script =
            "l = ds_list_create();\n"
            "ds_list_add(l, 30);\n"
            "ds_list_add(l, 10);\n"
            "ds_list_add(l, 20);\n"
            "ds_list_sort(l, 1);\n" // ascending: 10, 20, 30
            "v0 = ds_list_find_value(l, 0);\n"
            "v1 = ds_list_find_value(l, 1);\n"
            "v2 = ds_list_find_value(l, 2);\n"
            "ds_list_reverse(l);\n" // reverse: 30, 20, 10
            "rv0 = ds_list_find_value(l, 0);\n"
            "rv2 = ds_list_find_value(l, 2);\n"
            "ds_list_destroy(l);\n";

        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v0 = gml_vm_get(&vm, "v0");
        assert(v0.real == 10.0);
        gml_value_free(&v0);

        gml_value v1 = gml_vm_get(&vm, "v1");
        assert(v1.real == 20.0);
        gml_value_free(&v1);

        gml_value v2 = gml_vm_get(&vm, "v2");
        assert(v2.real == 30.0);
        gml_value_free(&v2);

        gml_value rv0 = gml_vm_get(&vm, "rv0");
        assert(rv0.real == 30.0);
        gml_value_free(&rv0);

        gml_value rv2 = gml_vm_get(&vm, "rv2");
        assert(rv2.real == 10.0);
        gml_value_free(&rv2);

        gml_ast_free(ast);
    }

    // 2. Test ds_map_find_first and ds_map_find_next
    {
        const char* script =
            "m = ds_map_create();\n"
            "ds_map_add(m, 101, 55);\n"
            "ds_map_add(m, 102, 66);\n"
            "k1 = ds_map_find_first(m);\n"
            "k2 = ds_map_find_next(m, k1);\n"
            "ds_map_destroy(m);\n";

        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value k1 = gml_vm_get(&vm, "k1");
        assert(k1.real == 101.0);
        gml_value_free(&k1);

        gml_value k2 = gml_vm_get(&vm, "k2");
        assert(k2.real == 102.0);
        gml_value_free(&k2);

        gml_ast_free(ast);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v14 PASSED SUCCESSFULLY.\n");
    return 0;
}
