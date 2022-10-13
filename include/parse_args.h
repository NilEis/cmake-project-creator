#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

typedef struct
{
    char project_name[64];
    char *project_language;
    int name_set;
    int git;
    int lib;
} args_t;

void get_options(int *argc, char ***argv, args_t *args);

#endif // PARSE_ARGS_H
