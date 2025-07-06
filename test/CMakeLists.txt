cmake_minimum_required(VERSION 3.13)
project(org_chart_test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR ${CMAKE_SOURCE_DIR})
set(TEST_DIR ${SRC_DIR}/test)
set(MODEL_DIR ${SRC_DIR}/models)
set(CONTROLLER_DIR ${SRC_DIR}/controllers)
set(THIRD_PARTY_DIR ${SRC_DIR}/third_party)

add_subdirectory(${THIRD_PARTY_DIR}/googletest ${CMAKE_BINARY_DIR}/googletest-build)

set(TEST_SOURCES
    ${TEST_DIR}/test_main.cc
    ${TEST_DIR}/User_test.cc
    ${TEST_DIR}/Person_test.cc
    ${MODEL_DIR}/User.cc
    ${MODEL_DIR}/Person.cc
    ${MODEL_DIR}/Department.cc
    ${MODEL_DIR}/Job.cc
)

add_executable(org_chart_test ${TEST_SOURCES})
set_target_properties(org_chart_test PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(org_chart_test PRIVATE
    ${SRC_DIR}
    ${MODEL_DIR}
    ${CONTROLLER_DIR}
    ${TEST_DIR}
)

target_link_libraries(org_chart_test PRIVATE
    drogon
    trantor
    pthread
    gtest
    gtest_main
)

target_compile_options(org_chart_test PRIVATE
    $<$<CONFIG:Debug>:--coverage -O0 -g>
)
target_link_options(org_chart_test PRIVATE
    $<$<CONFIG:Debug>:--coverage>
)

enable_testing()
add_test(NAME org_chart_test COMMAND org_chart_test)
