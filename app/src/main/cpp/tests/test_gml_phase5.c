#include "gm82_runtime.h"
#include "gm82_gml_builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void test_string_expansions(void) {
    char buf[128];
    gml_string_copy("GameMaker82", 5, 5, buf, sizeof(buf));
    assert(strcmp(buf, "Maker") == 0);

    gml_string_replace("Hello World", "World", "NOR Maker", buf, sizeof(buf));
    assert(strcmp(buf, "Hello NOR Maker") == 0);

    gml_string_replace_all("foo bar foo baz foo", "foo", "qux", buf, sizeof(buf));
    assert(strcmp(buf, "qux bar qux baz qux") == 0);

    puts("test_string_expansions PASS");
}

static void test_collision_shapes(void) {
    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_gml_set_runtime(&rt);

    gm82_instance *target = gm82_runtime_instance_create(&rt, 1, 100, 100);
    assert(target != NULL);

    /* Line collision passing through target at (100, 100) */
    double hit_line = gml_collision_line(0, 0, 200, 200, 1, 0, 0);
    assert(hit_line == target->id);

    /* Line collision missing target */
    double miss_line = gml_collision_line(0, 0, 50, 50, 1, 0, 0);
    assert(miss_line == -4);

    /* Ellipse collision covering (100, 100) */
    double hit_ell = gml_collision_ellipse(80, 80, 120, 120, 1, 0, 0);
    assert(hit_ell == target->id);

    /* Ellipse collision missing (100, 100) */
    double miss_ell = gml_collision_ellipse(0, 0, 50, 50, 1, 0, 0);
    assert(miss_ell == -4);

    puts("test_collision_shapes PASS");
}

int main(void) {
    puts("=== Testing Phase 5 GML Expansion (Strings & Collision Shapes) ===");
    test_string_expansions();
    test_collision_shapes();
    puts("GML_PHASE5_TEST_PASS");
    return 0;
}
