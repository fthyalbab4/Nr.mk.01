cmake_minimum_required(VERSION 3.22.1)

project("gm82_android")

find_package(JNI QUIET)

include_directories(
    include
    .
)

if(JNI_FOUND)
    include_directories(${JNI_INCLUDE_DIRS})
endif()

file(GLOB GM82_SOURCES
    src/*.c
    gm82_jni.c
    gml_vm.c
)
list(FILTER GM82_SOURCES EXCLUDE REGEX ".*(test_|benchmark|gml_invoke|gml_do_until|gmk_probe_contract).*")

add_library(
    gm82_android
    SHARED
    ${GM82_SOURCES}
)

find_library(log-lib log)
find_library(jnigraphics-lib jnigraphics)
find_library(z-lib z)

if(log-lib)
    target_link_libraries(gm82_android ${log-lib})
endif()
if(jnigraphics-lib)
    target_link_libraries(gm82_android ${jnigraphics-lib})
endif()
if(z-lib)
    target_link_libraries(gm82_android ${z-lib})
else()
    target_link_libraries(gm82_android z)
endif()

target_link_libraries(
    gm82_android
    m
)

# Test executables for CTest / native verification
enable_testing()

file(GLOB GM82_TEST_FILES tests/*.c)
foreach(TEST_SRC ${GM82_TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC} ${GM82_SOURCES})
    target_link_libraries(${TEST_NAME} z m)
    add_test(NAME ${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>)
    set_tests_properties(${TEST_NAME} PROPERTIES WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../..")
endforeach()
