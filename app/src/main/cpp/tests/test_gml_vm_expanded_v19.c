#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v19] Starting...\n");

    /* 1. Direct function checks for ds_priority */
    double prio = gml_ds_priority_create();
    assert(prio >= 0.0);
    assert(gml_ds_priority_empty(prio) == 1.0);

    gml_ds_priority_add(prio, 100.0, 10.0);
    gml_ds_priority_add(prio, 200.0, 50.0);
    gml_ds_priority_add(prio, 300.0, 5.0);

    assert(gml_ds_priority_size(prio) == 3.0);
    assert(gml_ds_priority_find_max(prio) == 200.0);
    assert(gml_ds_priority_find_min(prio) == 300.0);

    assert(gml_ds_priority_delete_max(prio) == 200.0);
    assert(gml_ds_priority_size(prio) == 2.0);

    assert(gml_ds_priority_delete_min(prio) == 300.0);
    assert(gml_ds_priority_size(prio) == 1.0);

    gml_ds_priority_clear(prio);
    assert(gml_ds_priority_size(prio) == 0.0);
    assert(gml_ds_priority_empty(prio) == 1.0);

    gml_ds_priority_destroy(prio);

    /* 2. Direct function checks for ds_grid_get_mean and distance_to_point */
    double grid = gml_ds_grid_create(4, 4);
    assert(grid >= 0.0);
    gml_ds_grid_set(grid, 0, 0, 10.0);
    gml_ds_grid_set(grid, 1, 0, 20.0);
    gml_ds_grid_set(grid, 0, 1, 30.0);
    gml_ds_grid_set(grid, 1, 1, 40.0);

    double mean_val = gml_ds_grid_get_mean(grid, 0, 0, 1, 1);
    assert(mean_val == 25.0);
    gml_ds_grid_destroy(grid);

    /* 3. AST VM Execution checks */
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test AST ds_priority_create() */
    gml_ast call_prio_create = { GML_AST_CALL, .text = "ds_priority_create", .items = NULL, .count = 0 };
    gml_ast ret_prio_create = { GML_AST_RETURN, .left = &call_prio_create };

    int ok = gml_vm_execute(&vm, &ret_prio_create);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    double vm_prio = vm.return_value.real;
    assert(vm_prio >= 0.0);

    /* Test AST ds_priority_add(vm_prio, 42.0, 100.0) */
    gml_ast a_id = { GML_AST_NUMBER, .number = vm_prio };
    gml_ast a_val = { GML_AST_NUMBER, .number = 42.0 };
    gml_ast a_prio = { GML_AST_NUMBER, .number = 100.0 };
    const gml_ast *p_add_args[3] = { &a_id, &a_val, &a_prio };
    gml_ast call_prio_add = { GML_AST_CALL, .text = "ds_priority_add", .items = (gml_ast**)p_add_args, .count = 3 };
    gml_ast ret_prio_add = { GML_AST_RETURN, .left = &call_prio_add };

    ok = gml_vm_execute(&vm, &ret_prio_add);
    assert(ok == 1);

    /* Test AST ds_priority_find_max(vm_prio) -> 42.0 */
    const gml_ast *p_find_args[1] = { &a_id };
    gml_ast call_prio_find = { GML_AST_CALL, .text = "ds_priority_find_max", .items = (gml_ast**)p_find_args, .count = 1 };
    gml_ast ret_prio_find = { GML_AST_RETURN, .left = &call_prio_find };

    ok = gml_vm_execute(&vm, &ret_prio_find);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 42.0);

    /* Test AST window_set_caption("NOR Maker 2026") */
    gml_ast cap_str = { GML_AST_STRING, .text = "NOR Maker 2026" };
    const gml_ast *cap_args[1] = { &cap_str };
    gml_ast call_cap = { GML_AST_CALL, .text = "window_set_caption", .items = (gml_ast**)cap_args, .count = 1 };
    gml_ast ret_cap = { GML_AST_RETURN, .left = &call_cap };

    ok = gml_vm_execute(&vm, &ret_cap);
    assert(ok == 1);

    /* Clean up prio */
    gml_ds_priority_destroy(vm_prio);

    printf("[test_gml_vm_expanded_v19] PASS\n");
    return 0;
}
