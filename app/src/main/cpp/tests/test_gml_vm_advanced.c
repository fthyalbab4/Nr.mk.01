#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "gml_vm.h"
#include "gm82_gml_builtins.h"

static void test_color_functions(void) {
    double col = gml_make_color_rgb(100, 150, 200);
    assert(gml_color_get_red(col) == 100);
    assert(gml_color_get_green(col) == 150);
    assert(gml_color_get_blue(col) == 200);
    printf("test_color_functions PASS\n");
}

static void test_string_format(void) {
    char out[128];
    gml_string_format(12.3456, 5, 2, out, sizeof(out));
    assert(strstr(out, "12.35") != NULL);
    printf("test_string_format PASS\n");
}

static void test_file_find_stubs(void) {
    const char *f = gml_file_find_first("*.txt", 0);
    assert(f != NULL);
    const char *n = gml_file_find_next();
    assert(n != NULL);
    gml_file_find_close();
    printf("test_file_find_stubs PASS\n");
}

int main(void) {
    printf("=== Testing Advanced GML VM Built-ins ===\n");
    test_color_functions();
    test_string_format();
    test_file_find_stubs();
    printf("GML_VM_ADVANCED_TEST_PASS\n");
    return 0;
}
