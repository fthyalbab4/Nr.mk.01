#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

int main(void) {
    printf("Starting test_gml_vm_expanded_v17...\n");

    /* 1. Direct builtin function checks */
    double d2 = gml_dot_product_normalised(3.0, 4.0, 3.0, 4.0);
    assert(fabs(d2 - 1.0) < 0.0001);

    double d3 = gml_dot_product_3d_normalised(0.0, 5.0, 0.0, 0.0, 10.0, 0.0);
    assert(fabs(d3 - 1.0) < 0.0001);

    /* 2. AST VM invocation */
    gml_vm vm;
    gml_vm_init(&vm);

    gml_ast a1 = { GML_AST_NUMBER, .number = 3.0 };
    gml_ast a2 = { GML_AST_NUMBER, .number = 4.0 };
    gml_ast a3 = { GML_AST_NUMBER, .number = 3.0 };
    gml_ast a4 = { GML_AST_NUMBER, .number = 4.0 };
    const gml_ast *args2d[4] = { &a1, &a2, &a3, &a4 };

    gml_ast call_2d = {
        GML_AST_CALL,
        .text = "dot_product_normalised",
        .items = (gml_ast**)args2d,
        .count = 4
    };

    gml_ast ret_stmt = {
        GML_AST_RETURN,
        .left = &call_2d
    };

    int ok = gml_vm_execute(&vm, &ret_stmt);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(fabs(vm.return_value.real - 1.0) < 0.0001);

    printf("test_gml_vm_expanded_v17 PASS!\n");
    return 0;
}
