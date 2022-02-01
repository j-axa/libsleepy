cmake_minimum_required(VERSION 3.22)
project(libsleepy C CXX)

include(CheckSymbolExists)
include(CheckCSourceCompiles)
check_symbol_exists(nanosleep "time.h" HAVE_NANOSLEEP)
check_symbol_exists(usleep "unistd.h" HAVE_USLEEP)
check_c_source_compiles("
    #include <immintrin.h>
    int main() { _mm_pause();return 0; }"
    HAVE_PAUSE
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external)

add_library(sleepy INTERFACE)
target_include_directories(sleepy INTERFACE src)
target_link_libraries(sleepy INTERFACE external::phnt)
if (HAVE_NANOSLEEP)
    target_compile_definitions(sleepy INTERFACE -DSLEEPY_HAS_NANOSLEEP)
endif()
if (HAVE_USLEEP)
    target_compile_definitions(sleepy INTERFACE -DSLEEPY_HAS_USLEEP)
endif()
if (HAVE_PAUSE)
    target_compile_definitions(sleepy INTERFACE -DSLEEPY_HAS_PAUSE)
endif()

set(SLEEPY_BUILD_TEST ON CACHE BOOL "build test program")
if(SLEEPY_BUILD_TEST)
    add_executable(test src/test.cxx)
    target_link_libraries(test sleepy)
endif()