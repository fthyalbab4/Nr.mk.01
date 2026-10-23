#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gml_vm.h"

int main(void) {
    printf("[test_gml_vm_expanded_v20] Starting...\n");

    /* 1. Direct function checks for ds_list_sort and ds_list_shuffle */
    double lst = gml_ds_list_create();
    assert(lst >= 0.0);

    gml_ds_list_add(lst, 50.0);
    gml_ds_list_add(lst, 10.0);
    gml_ds_list_add(lst, 30.0);
    gml_ds_list_add(lst, 20.0);
    assert(gml_ds_list_size(lst) == 4.0);

    /* Sort Ascending */
    gml_ds_list_sort(lst, 1.0);
    assert(gml_ds_list_find_value(lst, 0) == 10.0);
    assert(gml_ds_list_find_value(lst, 1) == 20.0);
    assert(gml_ds_list_find_value(lst, 2) == 30.0);
    assert(gml_ds_list_find_value(lst, 3) == 50.0);

    /* Sort Descending */
    gml_ds_list_sort(lst, 0.0);
    assert(gml_ds_list_find_value(lst, 0) == 50.0);
    assert(gml_ds_list_find_value(lst, 1) == 30.0);
    assert(gml_ds_list_find_value(lst, 2) == 20.0);
    assert(gml_ds_list_find_value(lst, 3) == 10.0);

    /* Shuffle */
    double shuf_ok = gml_ds_list_shuffle(lst);
    assert(shuf_ok == 1.0);
    assert(gml_ds_list_size(lst) == 4.0);

    gml_ds_list_destroy(lst);

    /* 2. Direct function checks for ds_map_find_first and ds_map_find_next */
    double mp = gml_ds_map_create();
    assert(mp >= 0.0);
    gml_ds_map_add(mp, 101.0, 500.0);
    gml_ds_map_add(mp, 202.0, 600.0);

    double k1 = gml_ds_map_find_first(mp);
    assert(k1 == 101.0);
    double k2 = gml_ds_map_find_next(mp, k1);
    assert(k2 == 202.0);

    gml_ds_map_destroy(mp);

    /* 3. Direct function checks for ds_grid_set_grid_region */
    double g_src = gml_ds_grid_create(4, 4);
    double g_dst = gml_ds_grid_create(4, 4);
    assert(g_src >= 0.0 && g_dst >= 0.0);

    gml_ds_grid_set(g_src, 0, 0, 99.0);
    gml_ds_grid_set(g_src, 1, 0, 88.0);

    double grid_copy_ok = gml_ds_grid_set_grid_region(g_dst, g_src, 0, 0, 1, 0, 2, 2);
    assert(grid_copy_ok == 1.0);
    assert(gml_ds_grid_get(g_dst, 2, 2) == 99.0);
    assert(gml_ds_grid_get(g_dst, 3, 2) == 88.0);

    gml_ds_grid_destroy(g_src);
    gml_ds_grid_destroy(g_dst);

    /* 4. AST VM Execution checks */
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test AST ds_map_create() -> map_id */
    gml_ast call_map_create = { GML_AST_CALL, .text = "ds_map_create", .items = NULL, .count = 0 };
    gml_ast ret_map_create = { GML_AST_RETURN, .left = &call_map_create };

    int ok = gml_vm_execute(&vm, &ret_map_create);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    double vm_mp = vm.return_value.real;
    assert(vm_mp >= 0.0);

    /* Test AST ds_map_add(vm_mp, 10.0, 77.0) */
    gml_ast a_map_id = { GML_AST_NUMBER, .number = vm_mp };
    gml_ast a_key = { GML_AST_NUMBER, .number = 10.0 };
    gml_ast a_val = { GML_AST_NUMBER, .number = 77.0 };
    const gml_ast *p_map_add_args[3] = { &a_map_id, &a_key, &a_val };
    gml_ast call_map_add = { GML_AST_CALL, .text = "ds_map_add", .items = (gml_ast**)p_map_add_args, .count = 3 };
    gml_ast ret_map_add = { GML_AST_RETURN, .left = &call_map_add };

    ok = gml_vm_execute(&vm, &ret_map_add);
    assert(ok == 1);

    /* Test AST ds_map_find_first(vm_mp) -> 10.0 */
    const gml_ast *p_find_first_args[1] = { &a_map_id };
    gml_ast call_find_first = { GML_AST_CALL, .text = "ds_map_find_first", .items = (gml_ast**)p_find_first_args, .count = 1 };
    gml_ast ret_find_first = { GML_AST_RETURN, .left = &call_find_first };

    ok = gml_vm_execute(&vm, &ret_find_first);
    assert(ok == 1);
    assert(vm.return_value.kind == GML_V_REAL);
    assert(vm.return_value.real == 10.0);

    gml_ds_map_destroy(vm_mp);

    printf("[test_gml_vm_expanded_v20] PASS\n");
    return 0;
}
