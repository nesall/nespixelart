cmake_minimum_required(VERSION 3.16)
project("nespixelart" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11) 

set(TARGET_NAME "nespixelart-main")

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

set(HEADER_FILES
  core/include/core.h
  core/include/palette.h
  )
if(WIN32)
  set(HEADER_FILES ${HEADER_FILES} ./nlohmann_json.natvis)
endif()

if(MSVC)
  message(STATUS "Using MSVC compiler")
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

add_library("${PROJECT_NAME}_core" STATIC
  ${HEADER_FILES}
  core/src/exporter.cpp
  core/src/core.cpp
  core/src/tilizer.cpp
  core/src/validator.cpp
)
add_library("${PROJECT_NAME}::core" ALIAS "${PROJECT_NAME}_core")

target_include_directories("${PROJECT_NAME}_core"
  PUBLIC
  core/include/
)

# Link libraries
#target_link_libraries(${PROJECT_NAME} PRIVATE
#  Catch2
#)

# ─── CLI Driver ──────────────
add_executable(${TARGET_NAME} main.cpp)
#target_include_directories(${TARGET_NAME} PRIVATE include/)
target_link_libraries(${TARGET_NAME} PRIVATE "${PROJECT_NAME}::core")

# Tests
enable_testing()

add_executable(tests
  tests/test_core.cpp
)

target_include_directories(tests PRIVATE include/)

target_link_libraries(tests
  PRIVATE
  "${PROJECT_NAME}_core"
  Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(tests)

# ─── Compiler Warnings ────────────────────────────────────────────────────────

target_compile_options(${PROJECT_NAME}_core PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /MP /GS>
  $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/RTC1>
)
