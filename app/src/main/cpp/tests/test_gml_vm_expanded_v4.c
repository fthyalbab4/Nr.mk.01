#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "gml_vm.h"
#include "gm82_gml_builtins.h"
#include "gm82_runtime.h"

int main(void) {
    printf("=== Testing GML VM Expanded V4 Suite ===\n");

    /* Test DS Grid math functions via C builtins */
    double grid = gml_ds_grid_create(4, 4);
    assert(grid >= 0);
    gml_ds_grid_set(grid, 0, 0, 10);
    gml_ds_grid_set(grid, 1, 0, 20);
    gml_ds_grid_set(grid, 0, 1, 30);
    gml_ds_grid_set(grid, 1, 1, 40);

    double sum = gml_ds_grid_get_sum(grid, 0, 0, 1, 1);
    assert(sum == 100.0);

    double max_v = gml_ds_grid_get_max(grid, 0, 0, 1, 1);
    assert(max_v == 40.0);

    double min_v = gml_ds_grid_get_min(grid, 0, 0, 1, 1);
    assert(min_v == 10.0);

    double mean_v = gml_ds_grid_get_mean(grid, 0, 0, 1, 1);
    assert(mean_v == 25.0);

    gml_ds_grid_destroy(grid);

    /* Test move_snap and place_snapped */
    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_gml_set_runtime(&rt);

    gm82_instance inst;
    inst.id = 101;
    inst.x = 14;
    inst.y = 35;
    gm82_gml_set_self(&inst);

    assert(gml_place_snapped(16, 16) == 0.0);
    gml_move_snap(16, 16);
    assert(inst.x == 16.0);
    assert(inst.y == 32.0);
    assert(gml_place_snapped(16, 16) == 1.0);

    /* Test AST execution for move_snap via AST */
    gml_vm vm;
    gml_vm_init(&vm);

    inst.x = 10;
    inst.y = 10;
    gml_ast n_snap = { GML_AST_CALL, GML_T_NONE, 0, "move_snap", NULL, NULL, 0, NULL };
    gml_ast hsnap = { GML_AST_NUMBER, GML_T_NONE, 32, NULL, NULL, NULL, 0, NULL };
    gml_ast vsnap = { GML_AST_NUMBER, GML_T_NONE, 32, NULL, NULL, NULL, 0, NULL };
    gml_ast *items_s[2] = { &hsnap, &vsnap };
    n_snap.count = 2;
    n_snap.items = items_s;
    gml_ast stmt_snap = { GML_AST_EXPR_STMT, GML_T_NONE, 0, NULL, &n_snap, NULL, 0, NULL };

    int ok = gml_vm_execute(&vm, &stmt_snap);
    assert(ok);
    assert(inst.x == 0.0);
    assert(inst.y == 0.0);

    printf("  test_gml_vm_expanded_v4 PASS\n");
    printf("GML_VM_EXPANDED_V4_TEST_PASS\n");
    return 0;
}
