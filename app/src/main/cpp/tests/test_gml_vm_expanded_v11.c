#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "gml_frontend.h"
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

int main(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    // 1. Test Sound Operations AST execution
    {
        const char* script =
            "p = sound_play(1);\n"
            "s = sound_isplaying(1);\n"
            "st = sound_stop(1);\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        if (!exec_ok) fprintf(stderr, "Sound test VM Error: '%s'\n", vm.error);
        assert(exec_ok);
        gml_value vp = gml_vm_get(&vm, "p");
        assert(vp.kind == GML_V_REAL);
        gml_value_free(&vp);
        gml_value vs = gml_vm_get(&vm, "s");
        assert(vs.kind == GML_V_REAL);
        gml_value_free(&vs);
        gml_ast_free(ast);
    }

    // 2. Test Surface & Draw AST execution
    {
        const char* script =
            "surf = surface_create(320, 240);\n"
            "exists = surface_exists(surf);\n"
            "sw = surface_get_width(surf);\n"
            "sh = surface_get_height(surf);\n"
            "target_ok = surface_set_target(surf);\n"
            "draw_set_color(16711680);\n"
            "draw_set_alpha(0.8);\n"
            "col = draw_get_color();\n"
            "alp = draw_get_alpha();\n"
            "draw_clear(0);\n"
            "draw_rectangle(10, 10, 50, 50, 0);\n"
            "draw_circle(100, 100, 20, 1);\n"
            "draw_line(0, 0, 100, 100);\n"
            "draw_text(5, 5, \"Hello Surface\");\n"
            "reset_ok = surface_reset_target();\n"
            "draw_surface(surf, 0, 0);\n"
            "free_ok = surface_free(surf);\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        if (!exec_ok) fprintf(stderr, "Surface test VM Error: '%s'\n", vm.error);
        assert(exec_ok);

        gml_value vsw = gml_vm_get(&vm, "sw");
        assert(vsw.kind == GML_V_REAL && vsw.real == 320.0);
        gml_value_free(&vsw);

        gml_value vsh = gml_vm_get(&vm, "sh");
        assert(vsh.kind == GML_V_REAL && vsh.real == 240.0);
        gml_value_free(&vsh);

        gml_value valp = gml_vm_get(&vm, "alp");
        assert(valp.kind == GML_V_REAL && valp.real == 0.8);
        gml_value_free(&valp);

        gml_ast_free(ast);
    }

    // 3. Test Room, Game, Path, Timeline & Particles AST execution
    {
        const char* script =
            "rm_ok = room_goto(0);\n"
            "p_ok = path_start(0, 4, 0, 1);\n"
            "tl_ok = timeline_start(0, 0, 1, 0);\n"
            "ps = part_system_create();\n"
            "pt = part_type_create();\n"
            "part_particles_create(ps, 50, 50, pt, 10);\n"
            "part_system_update(ps);\n"
            "part_system_destroy(ps);\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        if (!exec_ok) fprintf(stderr, "Room test VM Error: '%s'\n", vm.error);
        assert(exec_ok);

        gml_value vps = gml_vm_get(&vm, "ps");
        assert(vps.kind == GML_V_REAL);
        gml_value_free(&vps);

        gml_ast_free(ast);
    }

    // 4. Test MP Grid AST execution
    {
        const char* script =
            "grid = mp_grid_create(0, 0, 10, 10, 16, 16);\n"
            "mp_grid_add_cell(grid, 2, 2, 1);\n"
            "p_count = mp_grid_path(grid, 0, 0, 100, 100, 1);\n"
            "mp_grid_destroy(grid);\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        if (!exec_ok) fprintf(stderr, "MP Grid test VM Error: '%s'\n", vm.error);
        assert(exec_ok);

        gml_value vp_count = gml_vm_get(&vm, "p_count");
        assert(vp_count.kind == GML_V_REAL);
        gml_value_free(&vp_count);

        gml_ast_free(ast);
    }

    // 5. Test Date, Time & Window AST execution
    {
        const char* script =
            "dt = date_current_datetime();\n"
            "yr = date_get_year(dt);\n"
            "m = date_get_month(dt);\n"
            "d = date_get_day(dt);\n"
            "cur_t = current_time;\n"
            "window_set_caption(\"NOR Maker GM82 Engine\");\n"
            "cap = window_get_caption();\n";
        gml_ast* ast = NULL;
        int ok = gml_parse_program(script, &ast, NULL, 0);
        assert(ok && ast != NULL);
        int exec_ok = gml_vm_execute(&vm, ast);
        if (!exec_ok) fprintf(stderr, "Date test VM Error: '%s'\n", vm.error);
        assert(exec_ok);

        gml_value vcap = gml_vm_get(&vm, "cap");
        assert(vcap.kind == GML_V_STRING);
        assert(strcmp(vcap.string, "NOR Maker GM82 Engine") == 0);
        gml_value_free(&vcap);

        gml_ast_free(ast);
    }

    printf("ALL TEST CASES IN test_gml_vm_expanded_v11 PASSED SUCCESSFULLY.\n");
    return 0;
}
