#include "parse_args.h"
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "c_logger.h"
#include "clib.h"
#include "terminal.h"

#define EXPAND_STR(x) STR(x)
#define STR(x) #x

typedef enum
{
    LANG_C = 0,
    LANG_CPP = 1,
    LANG_FORTRAN = 2
} lang_t;

static int arg_quiet = 0;
static int arg_lang = LANG_C;
static int arg_lib = 0;
static int arg_git = 0;

char *LANG_STR[] = {"C", "CXX", "Fortran"};

void get_options(int *argc, char ***argv, args_t *args)
{
    int arg_log_level = C_LOGGER_CONFIRMATION | C_LOGGER_ERROR | C_LOGGER_INFO | C_LOGGER_DEBUG;
    int c = 0;
    opterr = 0;
    while (1)
    {
        static struct option long_options[] = {
            {"quiet", no_argument, &arg_quiet, 1},
            {"verbose", no_argument, NULL, 'v'},
            {"name", required_argument, NULL, 'n'},
            {"lib", no_argument, NULL, 'l'},
            {"cpp", no_argument, &arg_lang, LANG_CPP},
            {"fortran", no_argument, &arg_lang, LANG_FORTRAN},
            {"help", no_argument, NULL, 'h'},
            {"info", no_argument, NULL, 'i'},
            {"git", no_argument, &arg_git, 1},
            {NULL, no_argument, NULL, 0}};
        int option_index = 0;
        c = getopt_long(*argc, *argv, "qvn:lhi", long_options, &option_index);
        if (c == -1)
        {
            break;
        }
        switch (c)
        {
        case 0:
            break;
        case 'q':
            arg_quiet = 1;
            break;
        case 'v':
            arg_quiet = 0;
            arg_log_level = C_LOGGER_CONFIRMATION | C_LOGGER_ERROR | C_LOGGER_INFO | C_LOGGER_DEBUG | C_LOGGER_VERBOSE;
            break;
        case 'n':
            clib_strcpy((char *)&args->project_name, optarg, 64);
            clib_replace_char_all((char *)&args->project_name, ' ', '_');
            args->name_set = 1;
            break;
        case 'l':
            arg_lib = 1;
            break;
        case 'h':
            puts(
                "-h, --help   \tGive this help text\n"
                "-i, --info   \tPrint information about the program\n"
                "-q, --quiet  \tBe quiet\n"
                "-v, --verbose\tBe verbose\n"
                "-n, --name   \tThe name of the project\n"
                "    --cpp    \tSet the language to CXX (standard: C)\n"
                "    --fortran\tSet the language to Fortran (standard: C)\n");
            exit(0);
            break;
        case 'i':
            puts(
                "Compiled with GCC " EXPAND_STR(__GNUC__) "." EXPAND_STR(__GNUC_MINOR__) "." EXPAND_STR(__GNUC_PATCHLEVEL__) " on "__DATE__
                                                                                                                             " at "__TIME__);
            exit(0);
            break;
        case '?':
            break;
        }
    }
    c_logger_set_level((!arg_quiet) * arg_log_level);
    args->project_language = LANG_STR[arg_lang];
    args->git = arg_git;
    args->lib = arg_lib;
}