# AGENT_STATE.md — NOR_MAKER Plan B (ملزم لأي وكيل / Jules)

plan_B_percent: 93.5
is_100: false
claim_100_percent_allowed: false
release_tag: HOST_PROTOTYPE

last_result: PASS
last_test_log: "All 36 native C unit test suites verified PASS via CTest."
last_run_date: 2026-10-21

## rules (لا تُكسر)
- never claim 100%
- never say: complete engine, full GML VM done, production-ready, finished Plan B
- never rewrite overall architecture
- authority: GAPS_HONEST.md + this file > README > PR titles

## progress reality
Relative to complete Windows GameMaker 8.2 parity, native engine progress is estimated at ~93.5%.
Audited and wired 81 previously unwired GML built-in functions into gml_vm.c AST VM evaluator (including instance management, place/position checks, room controls, audio stubs, input helpers, alarms, scripts, timelines, paths, and instance variable getters/setters). Added test_gml_vm_expanded_v20.c to verify AST VM execution and direct builtin execution. All 36 CTest unit test suites pass successfully (100%). Configured debug keystore and validated Gradle Android debug APK build.

## required_reply_format
PROGRESS: 93.5% (compared to full Windows GM82)
DONE: Wired 81 GML built-in functions into gml_vm.c AST VM evaluator. Added test_gml_vm_expanded_v20.c verifying AST VM execution and direct builtin execution across all 36 CTest unit test suites.
TEST: All 36 native C unit test suites passing and Gradle assembleDebug successful.
RESULT: PASS
REMAINING: GLES Hardware Rendering pipeline, OpenSL ES Audio backend, precise per-pixel collisions, advanced particle effects, physical device testing.
NEXT: Continue expanding GML VM bytecode compiler capabilities and GLES rendering pipeline.
CLAIM_100: no
