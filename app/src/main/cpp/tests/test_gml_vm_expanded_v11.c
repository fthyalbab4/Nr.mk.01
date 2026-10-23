#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "gml_frontend.h"
#include "gml_vm.h"
#include "gm82_gml_builtins.h"
#include "gm82_particles.h"
#include "gm82_mp_grid.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    gm82_particle_world pworld;
    gm82_particles_init(&pworld);
    gm82_particles_bind(&pworld);

    gm82_mp_grid_world gridworld;
    gm82_mp_grid_world_init(&gridworld);
    gm82_mp_grid_bind(&gridworld);

    // 1. Test Surface VM execution
    {
        const char* script =
            "s = surface_create(64, 64);\n"
            "e1 = surface_exists(s);\n"
            "w = surface_get_width(s);\n"
            "h = surface_get_height(s);\n"
            "surface_free(s);\n"
            "e2 = surface_exists(s);\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_e1 = gml_vm_get(&vm, "e1");
        assert(v_e1.real == 1.0);
        gml_value_free(&v_e1);

        gml_value v_w = gml_vm_get(&vm, "w");
        assert(v_w.real == 64.0);
        gml_value_free(&v_w);

        gml_value v_h = gml_vm_get(&vm, "h");
        assert(v_h.real == 64.0);
        gml_value_free(&v_h);

        gml_value v_e2 = gml_vm_get(&vm, "e2");
        assert(v_e2.real == 0.0);
        gml_value_free(&v_e2);

        gml_ast_free(ast);
    }

    // 2. Test Particle and MP Grid VM execution
    {
        const char* script =
            "ps = part_system_create();\n"
            "pt = part_type_create();\n"
            "p_ok = part_particles_create(ps, 10, 10, pt, 5);\n"
            "grid = mp_grid_create(0, 0, 10, 10, 16, 16);\n"
            "c_ok = mp_grid_add_cell(grid, 2, 2, 1);\n"
            "mp_grid_destroy(grid);\n"
            "part_system_destroy(ps);\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_pok = gml_vm_get(&vm, "p_ok");
        assert(v_pok.real == 1.0);
        gml_value_free(&v_pok);

        gml_value v_cok = gml_vm_get(&vm, "c_ok");
        assert(v_cok.real == 1.0);
        gml_value_free(&v_cok);

        gml_ast_free(ast);
    }

    // 3. Test Date / Time builtins execution
    {
        const char* script =
            "now = date_current_datetime();\n"
            "yr = date_get_year(now);\n"
            "mth = date_get_month(now);\n"
            "ctime = current_time();\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        assert(exec_ok);

        gml_value v_yr = gml_vm_get(&vm, "yr");
        assert(v_yr.real >= 2020.0);
        gml_value_free(&v_yr);

        gml_value v_ctime = gml_vm_get(&vm, "ctime");
        assert(v_ctime.real >= 0.0);
        gml_value_free(&v_ctime);

        gml_ast_free(ast);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v11 PASSED SUCCESSFULLY.\n");
    return 0;
}
