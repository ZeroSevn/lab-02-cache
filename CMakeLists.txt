cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.269.tar.gz"
        SHA1 "64024b7b95b4c86d50ae05b926814448c93a70a0"
)

project(lab-02-cache-lib)
project(lab-02-cache-app)

string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_library(lab-02-cache-lib STATIC ${CMAKE_CURRENT_SOURCE_DIR}/sources/source.cpp)
add_executable(lab-02-cache-app ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp)
add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp)

target_include_directories(lab-02-cache-lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(tests 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(lab-02-cache-app
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tests PRIVATE GTest::gtest_main
        lab-02-cache-lib)

enable_testing()
add_test(NAME unit_tests COMMAND tests) 

if(BUILD_COVERAGE)
  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(codecov)

  function(ENABLE_PROJECT_COVERAGE PROJECT)
    target_compile_options(${PROJECT} INTERFACE -O0 -g --coverage)
    if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
      target_link_options(${PROJECT} INTERFACE --coverage)
    else()
      target_link_libraries(${PROJECT} INTERFACE --coverage)
    endif()
  endfunction()

  enable_project_coverage(lab-02-cache-lib)
  enable_project_coverage(tests)
  list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
  coverage_evaluate()
endif()
