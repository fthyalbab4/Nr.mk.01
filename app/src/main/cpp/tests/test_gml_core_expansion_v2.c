#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "gm82_gml_builtins.h"
#include "gm82_gml_eval.h"
#include "gm82_runtime.h"

static void test_string_ord_at(void) {
    printf("Testing string_ord_at...\n");
    double code = gml_string_ord_at("ABC", 1.0);
    assert(code == 65.0);
    code = gml_string_ord_at("ABC", 2.0);
    assert(code == 66.0);
    code = gml_string_ord_at("ABC", 3.0);
    assert(code == 67.0);
    printf("  test_string_ord_at PASS\n");
}

static void test_directory_and_file_ops(void) {
    printf("Testing directory and file operations...\n");
    const char *dir_path = "/tmp/test_nor_dir";
    gml_directory_create(dir_path);
    assert(gml_directory_exists(dir_path) == 1.0);

    const char *f1 = "/tmp/test_nor_f1.txt";
    const char *f2 = "/tmp/test_nor_f2.txt";
    FILE *fp = fopen(f1, "w");
    if (fp) {
        fputs("NOR_MAKER_TEST", fp);
        fclose(fp);
    }

    assert(gml_file_copy(f1, f2) == 1.0);
    assert(gml_file_exists(f2) == 1.0);

    gml_file_delete(f1);
    gml_file_delete(f2);
    printf("  test_directory_and_file_ops PASS\n");
}

static void test_eval_buffers_and_alarms(void) {
    printf("Testing GML expression evaluation for buffers & alarms...\n");
    gm82_runtime rt;
    memset(&rt, 0, sizeof(rt));
    gm82_gml_set_runtime(&rt);

    gm82_instance self;
    memset(&self, 0, sizeof(self));
    self.alive = true;
    gm82_gml_set_self(&self);

    double val = 0;
    gm82_gml_eval_expr(&rt, &self, "buffer_create(128, 0, 1)", &val);
    assert(val >= 0);
    double buf_id = val;

    gm82_gml_eval_expr(&rt, &self, "buffer_write(0, 3, 12345)", &val);
    gm82_gml_eval_expr(&rt, &self, "buffer_seek(0, 0, 0)", &val);
    gm82_gml_eval_expr(&rt, &self, "buffer_read(0, 3)", &val);
    assert(val == 12345.0);

    gm82_gml_eval_expr(&rt, &self, "alarm_set(0, 60)", &val);
    gm82_gml_eval_expr(&rt, &self, "alarm_get(0)", &val);
    assert(val == 60.0);

    printf("  test_eval_buffers_and_alarms PASS\n");
}

int main(void) {
    printf("=== Testing GML Core Expansion V2 Suite ===\n");
    test_string_ord_at();
    test_directory_and_file_ops();
    test_eval_buffers_and_alarms();
    printf("GML_CORE_EXPANSION_V2_TEST_PASS\n");
    return 0;
}
