#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "c_logger.h"
#include "clib.h"
#include "parse_args.h"
#include "templates.h"
#define EXAMPLE_NAME_DEF "Example_project"
const static char *EXAMPLE_NAME = EXAMPLE_NAME_DEF;

int main(int argc, char **argv)
{
    args_t args = {EXAMPLE_NAME_DEF, "C", 0};
    char *project_type = "add_executable";
    get_options(&argc, &argv, &args);
    if (args.name_set == 0)
    {
        args.name_set = 1;
        printf("Project name (" EXAMPLE_NAME_DEF "): ");
        if (clib_safe_gets((char *)&args.project_name, 64) == 0)
        {
            clib_strcpy((char *)&args.project_name, EXAMPLE_NAME, 64);
        }
    }
    {
        FILE *cmakelists = fopen("./CMakeLists.txt", "w");
        if (args.lib)
        {
            project_type = "add_library";
        }
        fprintf(cmakelists, cmakelists_txt_template, args.project_name, args.project_language, project_type, args.project_name, args.project_name, args.project_name, args.project_name);
        fclose(cmakelists);
    }
    {
        FILE *gitignore = fopen("./.gitignore", "w");
        fprintf(gitignore, gitignore_template);
        fclose(gitignore);
    }
    {
        clib_mkdir("./src/", 0777);
        FILE *main_c = fopen("./src/main.c", "w");
        fprintf(main_c, main_c_template);
        fclose(main_c);
    }
    return 0;
}