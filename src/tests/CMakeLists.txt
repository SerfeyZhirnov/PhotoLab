cmake_minimum_required(VERSION 3.15)
project(PhotoLab_test VERSION 1.0 LANGUAGES CXX C)

#=============================================================== Setup CMake
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Qt6 REQUIRED COMPONENTS
  Core
  Widgets
)

include_directories(
  ${CMAKE_SOURCE_DIR}/../model
)

enable_testing()

add_executable(
  ${PROJECT_NAME}
  ${CMAKE_SOURCE_DIR}/../model/model.h
  ${CMAKE_SOURCE_DIR}/../model/model.cc
  ${CMAKE_SOURCE_DIR}/tests.cc
)

file(COPY ${CMAKE_SOURCE_DIR}/datasets
     DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
  Qt6::Core
  Qt6::Widgets
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    include(CodeCoverage)
    append_coverage_compiler_flags_to_target(${PROJECT_NAME})
    setup_target_for_coverage_lcov(
        NAME test_coverage
        EXECUTABLE ${PROJECT_NAME}
        EXCLUDE "${PROJECT_SOURCE_DIR}/build/*" "/usr/local/*" "/usr/include/*" "${PROJECT_SOURCE_DIR}/tests.cc"
        DEPENDENCIES ${PROJECT_NAME}
    )
endif()
