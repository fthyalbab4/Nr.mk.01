#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "gml_frontend.h"
#include "gml_vm.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    // 1. Test dot_product, dot_product_3d, point_distance_3d
    {
        const char* script =
            "dp = dot_product(1, 0, 0, 1);\n"
            "dp3 = dot_product_3d(1, 2, 3, 4, 5, 6);\n"
            "pdist3 = point_distance_3d(0, 0, 0, 2, 3, 6);\n";
        gml_ast* ast = NULL;
        char errbuf[256] = {0};
        int ok = gml_parse_program(script, &ast, errbuf, sizeof(errbuf));
        if (!ok) {
            printf("Parse error 1: %s\n", errbuf);
        }
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_dp = gml_vm_get(&vm, "dp");
        assert(fabs(v_dp.real - 0.0) < 0.001);
        gml_value_free(&v_dp);

        gml_value v_dp3 = gml_vm_get(&vm, "dp3");
        assert(fabs(v_dp3.real - 32.0) < 0.001);
        gml_value_free(&v_dp3);

        gml_value v_pdist3 = gml_vm_get(&vm, "pdist3");
        assert(fabs(v_pdist3.real - 7.0) < 0.001);
        gml_value_free(&v_pdist3);

        gml_ast_free(ast);
    }

    // 2. Test angle_difference, clamp, lerp, sign
    {
        const char* script =
            "ad = angle_difference(90, 0);\n"
            "cl = clamp(15, 0, 10);\n"
            "lp = lerp(0, 100, 0.25);\n"
            "sn = sign(0 - 42);\n";
        gml_ast* ast = NULL;
        char errbuf[256] = {0};
        int ok = gml_parse_program(script, &ast, errbuf, sizeof(errbuf));
        if (!ok) {
            printf("Parse error 2: %s\n", errbuf);
        }
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_ad = gml_vm_get(&vm, "ad");
        assert(fabs(v_ad.real - 90.0) < 0.001);
        gml_value_free(&v_ad);

        gml_value v_cl = gml_vm_get(&vm, "cl");
        assert(fabs(v_cl.real - 10.0) < 0.001);
        gml_value_free(&v_cl);

        gml_value v_lp = gml_vm_get(&vm, "lp");
        assert(fabs(v_lp.real - 25.0) < 0.001);
        gml_value_free(&v_lp);

        gml_value v_sn = gml_vm_get(&vm, "sn");
        assert(fabs(v_sn.real - (-1.0)) < 0.001);
        gml_value_free(&v_sn);

        gml_ast_free(ast);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v14 PASSED SUCCESSFULLY.\n");
    return 0;
}
