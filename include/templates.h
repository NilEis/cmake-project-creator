#ifndef TEMPLATES_H
#define TEMPLATES_H

#define CMAKELISTS_TXT "cmake_minimum_required(VERSION 3.9.0)\nproject(%s VERSION 0.1.0 LANGUAGES %s)\n\nif (CMAKE_C_COMPILER_ID STREQUAL \"GNU\")\n    add_compile_options(-ffunction-sections -fdata-sections)\nendif()\n\n%s(%s ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)\n\ntarget_include_directories(%s PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)\n\ninclude(CheckIPOSupported)\n\nif (CMAKE_C_COMPILER_ID STREQUAL \"GNU\")\n    target_link_options(%s PRIVATE -Wl,--gc-sections)\nendif()\n\ncheck_ipo_supported(RESULT result OUTPUT output LANGUAGES C)\nif(result)\nmessage(STATUS \"IPO is supported\")\nset_property(TARGET %s PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)\nelse()\nmessage(WARNING \"IPO is not supported: ${output}\")\nendif()\n"
const char cmakelists_txt_template[] = CMAKELISTS_TXT;
#undef CMAKELISTS_TXT

#define MAIN_C "#include <stdio.h>\n\nint main(int argc, char **argv)\n{\n    printf(\"Hello, World!\\n\");\n    return 0;\n}\n"
const char main_c_template[] = MAIN_C;
#undef MAIN_C

#define GITIGNORE ".vscode/\nbuild/"
const char gitignore_template[] = GITIGNORE;
#undef GITIGNORE

#endif // TEMPLATES_H
