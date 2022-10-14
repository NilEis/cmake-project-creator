cmake_minimum_required(VERSION 3.9.0)
project(cmake_project_creator VERSION 0.1.0 LANGUAGES C)

link_libraries("-static")
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-ffunction-sections -fdata-sections)
endif()

add_subdirectory(extern/c-logger)
add_subdirectory(extern/clib)
if (NOT TARGET c_terminal)
    add_subdirectory(extern/terminal)
endif()

add_executable(cmake_project_creator src/main.c src/parse_args.c)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/templates/CMakeLists.txt)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/templates/main.c)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/templates/CMakeLists.txt CMAKELISTS_TXT)
string(REPLACE "\n" "\\n" CMAKELISTS_TXT "${CMAKELISTS_TXT}")
string(REPLACE "\"" "\\\"" CMAKELISTS_TXT "${CMAKELISTS_TXT}")

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/templates/main.c MAIN_C)
string(REPLACE "\\n" "\\\\n" MAIN_C "${MAIN_C}")
string(REPLACE "\n" "\\n" MAIN_C "${MAIN_C}")
string(REPLACE "\"" "\\\"" MAIN_C "${MAIN_C}")

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/templates/.gitignore GITIGNORE)
string(REPLACE "\n" "\\n" GITIGNORE "${GITIGNORE}")
string(REPLACE "\"" "\\\"" GITIGNORE "${GITIGNORE}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/templates.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/templates.h @ONLY)

target_include_directories(cmake_project_creator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

include(CheckIPOSupported)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(cmake_project_creator PRIVATE -Wl,--gc-sections)
endif()

check_ipo_supported(RESULT result OUTPUT output LANGUAGES C)
if(result)
message(STATUS "IPO is supported")
set_property(TARGET cmake_project_creator PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
message(WARNING "IPO is not supported: ${output}")
endif()

target_link_libraries(cmake_project_creator c_logger_static clib)

install(TARGETS cmake_project_creator CONFIGURATIONS Release RUNTIME DESTINATION .)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_NSIS_UNINSTALL_NAME "uninstall")
set(CPACK_NSIS_MENU_LINKS "https://github.com/NilEis/cmake-project-creator" "GitHub Repository" "cmake-create.exe" "cmake-create" "uninstall.exe" "Uninstall")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/NilEis/cmake-project-creator")
include(CPack)