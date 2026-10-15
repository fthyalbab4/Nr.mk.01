# AGENT_STATE.md — NOR_MAKER Plan B (ملزم لأي وكيل / Jules)

plan_B_percent: 90
is_100: false
claim_100_percent_allowed: false
release_tag: HOST_PROTOTYPE

last_result: PASS
last_test_log: "All 28 native C unit test suites and Android Debug APK build verified PASS."
last_run_date: 2026-10-15

## rules (لا تُكسر)
- never claim 100%
- never say: complete engine, full GML VM done, production-ready, finished Plan B
- never rewrite overall architecture
- authority: GAPS_HONEST.md + this file > README > PR titles

## progress reality
Relative to complete Windows GameMaker 8.2 parity, native engine progress is estimated at ~90%.
Expanded GML AST VM execution dispatches in gml_vm.c for room, game, instance, movement, path, timeline, alarm, timer, script, and window builtins (instance_create, instance_destroy, instance_exists, instance_place, position_meeting, instance_nearest, instance_furthest, position_destroy, distance_to_object, distance_to_point, move_outside_solid, move_outside_all, move_contact_solid, move_bounce_solid, move_bounce_all, move_random, draw_clear, draw_sprite, room_goto, room_goto_next, room_goto_previous, room_restart, game_end, game_restart, path_start, path_end, path_get_number, timeline_start, timeline_stop, get_timer, delta_time, alarm_set, alarm_get, script_exists, script_execute, window_set_caption, window_get_caption). Added test_gml_vm_expanded_v12.c to verify AST VM execution. All 28 unit test suites pass and Gradle Android Debug APK builds successfully.

## required_reply_format
PROGRESS: 90% (compared to full Windows GM82)
DONE: Expanded GML AST VM handlers for room, game, instance, movement, path, timeline, alarm, timer, script, and window builtins, added test_gml_vm_expanded_v12.c, and verified native compilation (28 test suites) and Gradle Android debug APK build.
TEST: All 28 native C unit test suites passing (test_dual_load PASS, test_gml_comprehensive PASS, test_gml_core_expansion PASS, test_gml_core_expansion_v2 PASS, test_gml_degree_trig PASS, test_gml_ds_collisions PASS, test_gml_eval_builtins PASS, test_gml_full_support PASS, test_gml_phase5 PASS, test_gml_physics_expanded PASS, test_gml_physics_simulation PASS, test_gml_strings_and_views PASS, test_gml_vm_advanced PASS, test_gml_vm_core_parity PASS, test_gml_vm_execution PASS, test_gml_vm_expanded PASS, test_gml_vm_expanded_v10 PASS, test_gml_vm_expanded_v11 PASS, test_gml_vm_expanded_v12 PASS, test_gml_vm_expanded_v2 PASS, test_gml_vm_expanded_v3 PASS, test_gml_vm_expanded_v4 PASS, test_gml_vm_expanded_v5 PASS, test_gml_vm_expanded_v6 PASS, test_gml_vm_expanded_v7 PASS, test_gml_vm_expanded_v8 PASS, test_gml_vm_expanded_v9 PASS, test_runtime_guard PASS) and Gradle Android Debug APK build PASS.
RESULT: PASS
REMAINING: GLES Hardware Rendering, OpenSL Audio backend, precise per-pixel collisions, advanced particle effects, device testing.
NEXT: Continue expanding GML VM bytecode compiler capabilities and GLES rendering pipeline.
CLAIM_100: no
