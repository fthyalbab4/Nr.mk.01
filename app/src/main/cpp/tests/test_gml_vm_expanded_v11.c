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

    /* Test file_exists via GML VM */
    {
        gml_ast *ast = NULL;
        int ok = gml_parse_program("res = file_exists('non_existent_file_xyz123.tmp');", &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);
        gml_value res = gml_vm_get(&vm, "res");
        assert(res.kind == GML_V_REAL);
        assert(res.real == 0.0);
        gml_value_free(&res);
        gml_ast_free(ast);
    }

    /* Test file_delete & file_exists via GML VM */
    {
        FILE *f = fopen("test_dummy_v11.txt", "w");
        if (f) {
            fputs("hello", f);
            fclose(f);
        }

        gml_ast *ast1 = NULL;
        assert(gml_parse_program("e1 = file_exists('test_dummy_v11.txt');", &ast1, NULL, 0) && ast1 != NULL);
        assert(gml_vm_execute(&vm, ast1));
        gml_value e1 = gml_vm_get(&vm, "e1");
        assert(e1.real == 1.0);
        gml_value_free(&e1);
        gml_ast_free(ast1);

        gml_ast *ast2 = NULL;
        assert(gml_parse_program("d1 = file_delete('test_dummy_v11.txt');", &ast2, NULL, 0) && ast2 != NULL);
        assert(gml_vm_execute(&vm, ast2));
        gml_value d1 = gml_vm_get(&vm, "d1");
        assert(d1.real == 1.0);
        gml_value_free(&d1);
        gml_ast_free(ast2);

        gml_ast *ast3 = NULL;
        assert(gml_parse_program("e2 = file_exists('test_dummy_v11.txt');", &ast3, NULL, 0) && ast3 != NULL);
        assert(gml_vm_execute(&vm, ast3));
        gml_value e2 = gml_vm_get(&vm, "e2");
        assert(e2.real == 0.0);
        gml_value_free(&e2);
        gml_ast_free(ast3);
    }

    /* Test date_get_year via GML VM */
    {
        gml_ast *ast = NULL;
        assert(gml_parse_program("yr = date_get_year(0);", &ast, NULL, 0) && ast != NULL);
        int ok = gml_vm_execute(&vm, ast);
        assert(ok);
        gml_value yr = gml_vm_get(&vm, "yr");
        assert(yr.kind == GML_V_REAL);
        assert(yr.real >= 1970.0);
        gml_value_free(&yr);
        gml_ast_free(ast);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v11 PASSED SUCCESSFULLY.\n");
    return 0;
}
