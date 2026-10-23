# AGENT_STATE.md — NOR_MAKER Plan B (ملزم لأي وكيل / Jules)

plan_B_percent: 94
is_100: false
claim_100_percent_allowed: false
release_tag: HOST_PROTOTYPE

last_result: PASS
last_test_log: "All 37 native C unit test suites verified PASS via CTest."
last_run_date: 2026-10-22

## rules (لا تُكسر)
- never claim 100%
- never say: complete engine, full GML VM done, production-ready, finished Plan B
- never rewrite overall architecture
- authority: GAPS_HONEST.md + this file > README > PR titles

## progress reality
Relative to complete Windows GameMaker 8.2 parity, native engine progress is estimated at ~94%.
Wired `window_get_x`, `window_get_y`, `window_set_position`, `window_set_size`, `parameter_count`, `parameter_string`, `environment_get_variable`, `clipboard_has_text`, `clipboard_get_text`, and `clipboard_set_text` in `gml_vm.c` and `gm82_gml_builtins.c`. Added `test_gml_vm_expanded_v21.c` to verify AST VM execution and direct builtin execution. All 37 CTest unit test suites pass successfully (100%). Validated Gradle Android debug APK build output at `app/build/outputs/apk/debug/app-debug.apk`.

## required_reply_format
PROGRESS: 94% (compared to full Windows GM82)
DONE: Added test_gml_vm_expanded_v21.c verifying window_get_x, window_get_y, window_set_position, window_set_size, parameter_count, parameter_string, environment_get_variable, clipboard_has_text, clipboard_get_text, and clipboard_set_text builtins both directly and via AST VM execution across all 37 CTest unit test suites.
TEST: All 37 native C unit test suites passing and Gradle assembleDebug successful.
RESULT: PASS
REMAINING: GLES Hardware Rendering, OpenSL Audio backend, precise per-pixel collisions, advanced particle effects, physical device testing.
NEXT: Continue expanding GML VM bytecode compiler capabilities and GLES rendering pipeline.
CLAIM_100: no
