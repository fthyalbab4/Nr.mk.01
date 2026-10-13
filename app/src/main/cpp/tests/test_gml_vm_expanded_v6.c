#include "gm82_runtime.h"
#include "gm82_sound_runtime.h"
#include "gm82_gml_builtins.h"
#include "gm82_gml_eval.h"
#include "gm82_input.h"
#include "gml_frontend.h"
#include "gml_vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

void test_gml_vm_expanded_v6(void) {
    gml_vm vm;
    gml_vm_init(&vm);

    /* Test ds_grid_multiply_region C builtin */
    double grid_id = gml_ds_grid_create(4, 4);
    assert(grid_id >= 0);
    gml_ds_grid_clear(grid_id, 2.0);
    gml_ds_grid_multiply_region(grid_id, 0, 0, 1, 1, 5.0);
    /* 4 cells = 10.0, 12 cells = 2.0 -> sum = 40 + 24 = 64 */
    double sum = gml_ds_grid_get_sum(grid_id, 0, 0, 3, 3);
    assert(fabs(sum - 64.0) < 0.0001);
    gml_ds_grid_destroy(grid_id);

    /* Test ds_list_sort C builtin */
    double list_id = gml_ds_list_create();
    assert(list_id >= 0);
    gml_ds_list_add(list_id, 40);
    gml_ds_list_add(list_id, 10);
    gml_ds_list_add(list_id, 30);
    gml_ds_list_add(list_id, 20);

    gml_ds_list_sort(list_id, 1); /* ascending */
    assert(gml_ds_list_find_value(list_id, 0) == 10);
    assert(gml_ds_list_find_value(list_id, 1) == 20);
    assert(gml_ds_list_find_value(list_id, 2) == 30);
    assert(gml_ds_list_find_value(list_id, 3) == 40);

    gml_ds_list_sort(list_id, 0); /* descending */
    assert(gml_ds_list_find_value(list_id, 0) == 40);
    assert(gml_ds_list_find_value(list_id, 1) == 30);
    assert(gml_ds_list_find_value(list_id, 2) == 20);
    assert(gml_ds_list_find_value(list_id, 3) == 10);

    /* Test ds_list_shuffle C builtin */
    gml_ds_list_shuffle(list_id);
    assert(gml_ds_list_size(list_id) == 4);

    gml_ds_list_destroy(list_id);

    printf("  test_gml_vm_expanded_v6 PASS\n");
}

#ifndef RUNNING_FULL_SUITE
int main(void) {
    printf("=== Testing GML VM Expanded V6 Suite ===\n");
    test_gml_vm_expanded_v6();
    printf("GML_VM_EXPANDED_V6_TEST_PASS\n");
    return 0;
}
#endif
