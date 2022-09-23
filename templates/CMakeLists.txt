cmake_minimum_required(VERSION 3.9.0)
project(%s VERSION 0.1.0 LANGUAGES %s)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-ffunction-sections -fdata-sections)
endif()

%s(%s ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

target_include_directories(%s PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

include(CheckIPOSupported)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(%s PRIVATE -Wl,--gc-sections)
endif()

check_ipo_supported(RESULT result OUTPUT output LANGUAGES C)
if(result)
message(STATUS "IPO is supported")
set_property(TARGET %s PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
message(WARNING "IPO is not supported: ${output}")
endif()
