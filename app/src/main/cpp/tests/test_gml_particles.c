#include "gm82_runtime.h"
#include "gm82_gml_builtins.h"
#include "gm82_particles.h"
#include "gm82_gml_eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) {
    puts("=== Testing GML Particle System Built-ins ===");

    gm82_particle_world world;
    gm82_particles_init(&world);
    gm82_particles_bind(&world);

    gm82_runtime rt;
    gm82_runtime_init(&rt);
    gm82_gml_set_runtime(&rt);

    gm82_instance *inst = gm82_runtime_instance_create(&rt, 0, 0, 0);
    gm82_gml_set_self(inst);

    double sys = gml_part_system_create();
    assert(sys >= 0);

    double pt = gml_part_type_create();
    assert(pt >= 0);

    assert(gml_part_type_color(pt, 0x00FF00) == 1.0);

    double res = 0;
    assert(gm82_gml_eval_expr(&rt, inst, "part_type_color(0, 16711680)", &res) == true);

    assert(gml_part_particles_create(sys, 100, 100, pt, 10) == 1.0);

    gml_part_system_update(sys);

    uint8_t rgba[32 * 32 * 4] = {0};
    gm82_part_system_draw(&world, (int)sys, rgba, 32, 32);

    assert(gml_part_system_destroy(sys) == 1.0);

    puts("GML_PARTICLES_TEST_PASS");
    return 0;
}
