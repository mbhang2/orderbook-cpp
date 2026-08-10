cmake_minimum_required(VERSION 3.20)
project(orderbook_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# Warnings on. You want these loud while you are relearning the language.
add_compile_options(-Wall -Wextra -Wpedantic)

# ---------------------------------------------------------------- library
add_library(ob
  src/vwap.cpp
  src/extremes.cpp
  src/csv.cpp
)
target_include_directories(ob PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ---------------------------------------------------------------- tests
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

add_executable(ob_tests
  tests/test_vwap.cpp
  tests/test_extremes.cpp
)
target_link_libraries(ob_tests PRIVATE ob Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(ob_tests)

# ---------------------------------------------------------------- cli
add_executable(ob_features src/main.cpp)
target_link_libraries(ob_features PRIVATE ob)
