#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "terminal.h"
#include "c_logger.h"
#include "clib.h"
#include "parse_args.h"
#include "templates.h"
#define EXAMPLE_NAME_DEF "Example_project"
const static char *EXAMPLE_NAME = EXAMPLE_NAME_DEF;

static void init_github(void);

int main(int argc, char **argv)
{
    args_t args = {EXAMPLE_NAME_DEF, "C", 0, 0};
    char *project_type = "add_executable";
    char modules[4096] = {0};
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
        FILE *gitignore = fopen("./.gitignore", "w");
        fprintf(gitignore, gitignore_template);
        fclose(gitignore);
        c_logger_log(C_LOGGER_VERBOSE, "created .gitignore\n");
    }
    {
        clib_mkdir("./src/", 0777);
        FILE *main_c = fopen("./src/main.c", "w");
        fprintf(main_c, main_c_template);
        fclose(main_c);
        c_logger_log(C_LOGGER_VERBOSE, "created src/main.c\n");
    }
    if (args.git)
    {
        init_github();
    }
    else
    {
        terminal_clear();
        printf("Initialise GitHub [Y/n]: ");
        int ch = clib_getch();
        if (ch == 'Y' || ch == 'y' || ch != 'N' || ch != 'n')
        {
            args.git = 1;
            init_github();
        }
    }
    if (args.git == 1)
    {
        char str[2048] = {0};
        printf("Enter external packages in the following format 'name,github_link;name,github_link;...':");
        scanf("%s", &str);
        size_t str_l = strlen(str);
        if (str_l >= 3)
        {
            char *entry = strtok(str, ";");
            while (entry != NULL)
            {
                char *name = NULL;
                char *link = NULL;
                link = strchr(entry, ',');
                link[0] = '\0';
                link++;
                name = entry;
                sprintf(modules, "%sadd_subdirectory(extern/%s)\n", modules, name);
                if (name == NULL || name == NULL)
                {
                    c_logger_log(C_LOGGER_ERROR, "'%s,%s' is not a valid combination\n", name, link);
                    break;
                }
                char *add_link = (char *)calloc(strlen(name) + strlen(link) + strlen("git submodule add l extern/n\0"), sizeof(char));
                sprintf(add_link, "git submodule add %s extern/%s", link, name);
                printf("name: %s\nlink: %s\n", name, link);
                system(add_link);
                free(add_link);
                entry = strtok(NULL, ";");
            }
        }
    }
    {
        FILE *cmakelists = fopen("./CMakeLists.txt", "w");
        if (args.lib)
        {
            project_type = "add_library";
        }
        fprintf(cmakelists, cmakelists_txt_template, args.project_name, args.project_language, project_type, args.project_name, args.project_name, modules, args.project_name, args.project_name);
        fclose(cmakelists);
        c_logger_log(C_LOGGER_VERBOSE, "Created CMakeLists.txt\n");
    }
    return 0;
}

void init_github(void)
{
    if (system("git --version") == 0)
    {
        system("git init && git add . && git commit -m \"Initial commit\"");
        terminal_clear();
        c_logger_log(C_LOGGER_VERBOSE, "Initialised git\n");
    }
    else
    {
        c_logger_log(C_LOGGER_ERROR, "git needs to be installed for --git to work (If git is installed please open an issue on github)\n");
    }
}