#include "gm82_runtime.h"
#include "gm82_gml_builtins.h"
#include "gm82_gml_eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) {
    puts("=== Testing GML ds_list Expression Evaluator Bindings ===");

    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_gml_set_runtime(&rt);

    gm82_instance *inst = gm82_runtime_instance_create(&rt, 0, 0, 0);
    gm82_gml_set_self(inst);

    double list = -1;
    assert(gm82_gml_eval_expr(&rt, inst, "ds_list_create()", &list) == true);
    assert(list >= 0);

    double res = 0;
    assert(gm82_gml_eval_expr(&rt, inst, "ds_list_add(0, 100)", &res) == true);
    assert(gm82_gml_eval_expr(&rt, inst, "ds_list_add(0, 200)", &res) == true);

    assert(gm82_gml_eval_expr(&rt, inst, "ds_list_size(0)", &res) == true);
    assert(res == 2.0);

    assert(gm82_gml_eval_expr(&rt, inst, "ds_list_find_value(0, 1)", &res) == true);
    assert(res == 200.0);

    assert(gm82_gml_eval_expr(&rt, inst, "ds_list_find_index(0, 100)", &res) == true);
    assert(res == 0.0);

    assert(gm82_gml_eval_expr(&rt, inst, "ds_list_delete(0, 0)", &res) == true);
    assert(gm82_gml_eval_expr(&rt, inst, "ds_list_size(0)", &res) == true);
    assert(res == 1.0);

    assert(gm82_gml_eval_expr(&rt, inst, "ds_list_destroy(0)", &res) == true);

    puts("GML_DS_LIST_EVAL_TEST_PASS");
    return 0;
}
