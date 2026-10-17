#include <stdio.h>
#include <assert.h>
#include "gml_frontend.h"
#include "gml_vm.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    // Test instance_place, position_meeting, place_empty, distance_to_object, distance_to_point
    {
        const char* script =
            "res_inst_place = instance_place(100, 100, 0);\n"
            "res_pos_meet = position_meeting(50, 50, 0);\n"
            "res_place_emp = place_empty(0, 0);\n"
            "res_dist_obj = distance_to_object(0);\n"
            "res_dist_pt = distance_to_point(10, 20);\n";

        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_inst_place = gml_vm_get(&vm, "res_inst_place");
        assert(v_inst_place.real == -4.0); // noone when no runtime/instance exists
        gml_value_free(&v_inst_place);

        gml_value v_pos_meet = gml_vm_get(&vm, "res_pos_meet");
        assert(v_pos_meet.real == 0.0);
        gml_value_free(&v_pos_meet);

        gml_value v_place_emp = gml_vm_get(&vm, "res_place_emp");
        assert(v_place_emp.real == 1.0); // true (1) when no solid instances
        gml_value_free(&v_place_emp);

        gml_value v_dist_obj = gml_vm_get(&vm, "res_dist_obj");
        assert(v_dist_obj.real == 1000000.0); // fallback distance when no target exists
        gml_value_free(&v_dist_obj);

        gml_value v_dist_pt = gml_vm_get(&vm, "res_dist_pt");
        assert(v_dist_pt.real >= 0.0);
        gml_value_free(&v_dist_pt);

        gml_ast_free(ast);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v15 PASSED SUCCESSFULLY.\n");
    return 0;
}
