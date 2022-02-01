cmake_minimum_required(VERSION 3.22)

project(external_dependencies)

add_library(external_processhacker INTERFACE)
add_library(external::phnt ALIAS external_processhacker)
if (WIN32)
    target_include_directories(external_processhacker INTERFACE processhacker/phnt/include)
    target_link_libraries(external_processhacker INTERFACE ntdll)
    set_target_properties(external_processhacker PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:external_processhacker,INTERFACE_INCLUDE_DIRECTORIES>)
endif()
