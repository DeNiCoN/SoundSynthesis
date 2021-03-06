cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SoundSynthesis)

set(CONFIGURE_DEPENDS ON)
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false src/*.hpp src/*.cpp)
get_filename_component(full_path_main_cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ABSOLUTE)
list(REMOVE_ITEM SOURCES ${full_path_main_cpp})
set(SOURCES ${SOURCES})

add_library(ssynthesis-base STATIC ${SOURCES})
target_include_directories(ssynthesis-base PUBLIC "src/")

option(SS_BUILD_TESTS "Build ssynthesis tests" OFF)

if(SS_BUILD_TESTS)
  enable_testing()
  add_subdirectory("tests")
endif()

add_executable(ssynthesis "src/main.cpp")
target_link_libraries(ssynthesis ssynthesis-base)
