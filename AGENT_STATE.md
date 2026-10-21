# AGENT_STATE.md — NOR_MAKER Plan B (ملزم لأي وكيل / Jules)

plan_B_percent: 93
is_100: false
claim_100_percent_allowed: false
release_tag: HOST_PROTOTYPE

last_result: PASS
last_test_log: "All 33 native C unit test suites and Android Debug APK build verified PASS."
last_run_date: 2026-10-17

## rules (لا تُكسر)
- never claim 100%
- never say: complete engine, full GML VM done, production-ready, finished Plan B
- never rewrite overall architecture
- authority: GAPS_HONEST.md + this file > README > PR titles

## progress reality
Relative to complete Windows GameMaker 8.2 parity, native engine progress is estimated at ~93%.
Implemented sprite origin (xoffset/yoffset) and scale (image_xscale/image_yscale) awareness in gm82_instance_get_bbox across all collision functions (place_meeting, instance_place, position_meeting, place_free, place_empty, collision_*), bounding box getters, and runtime step/draw. Added AST VM dispatches and evaluator builtins for draw_sprite_part, draw_sprite_part_ext, draw_text_transformed, window_set_caption, and window_get_caption. Added test_gml_vm_expanded_v17.c to verify origin-aware collision and VM dispatches. All 33 unit test suites pass and Gradle Android Debug APK builds successfully.

## required_reply_format
PROGRESS: 93% (compared to full Windows GM82)
DONE: Implemented sprite origin (xoffset/yoffset) & scale-aware collision bounding box calculations across all collision functions, added GML AST VM & evaluator dispatches for draw_sprite_part, draw_sprite_part_ext, draw_text_transformed, window_set_caption, window_get_caption, added test_gml_vm_expanded_v17.c, and verified native compilation (33 test suites) and Gradle Android debug APK build.
TEST: All 33 native C unit test suites passing and Gradle Android Debug APK build PASS.
RESULT: PASS
REMAINING: GLES Hardware Rendering, OpenSL Audio backend, precise per-pixel collision masking, advanced blend modes, device testing.
NEXT: Continue expanding GML VM bytecode compiler capabilities and GLES rendering pipeline.
CLAIM_100: no
