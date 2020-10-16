
#Download Gtest
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()


file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

include(GoogleTest)

add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests ssynthesis-base gtest_main)
target_include_directories(tests PUBLIC "../src")

gtest_discover_tests(tests)
