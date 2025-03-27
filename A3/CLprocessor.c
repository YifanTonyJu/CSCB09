#include "CLprocesssor.h"
#include <ctype.h>

///_|> descry: this function returns whether the str is all digit
///_|> str: the pointer to the string to be checked, type const char *
///_|> returning: this function returns 1 if the given string is all digit, otherwise returns 0, type int
int is_numeric(const char *str) {
    // a str with no number is not a number.
    if (*str == '\0') {
        return 0;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

void commend_arguments(int argc, char **argv, CLArgs *args){
    args->samples = 20;
    args->tdelay = 500000;
    args->show_memory = 0;
    args->show_cpu = 0;
    args->show_cores = 0;

    // consider the case when only give positional arguments, start from index 1
    int arg_index = 1;
    if (arg_index < argc && is_numeric(argv[arg_index])) {
        args->samples = strtol(argv[arg_index], NULL, 10);
        arg_index += 1;
        if (arg_index < argc && is_numeric(argv[arg_index])) {
            args->tdelay = strtol(argv[arg_index], NULL, 10);
            arg_index += 1;
        }
    }

    // process other flagged arguments
    while (arg_index < argc) {
        if (strcmp(argv[arg_index], "--memory") == 0) {
            args->show_memory = 1;
        }
        else if (strcmp(argv[arg_index], "--cpu") == 0) {
            args->show_cpu = 1;
        }
        else if (strcmp(argv[arg_index], "--cores") == 0) {
            args->show_cores = 1;
        }
        else if (strncmp(argv[arg_index], "--samples=", 10) == 0) {
            if (is_numeric(argv[arg_index] + 10)) {
                args->samples = strtol(argv[arg_index] + 10, NULL, 10);
            }
            else {
                fprintf(stderr, "Error: Please use legal command line arguments!\n");
                exit(1);
            }
        }
        else if (strncmp(argv[arg_index], "--tdelay=", 9) == 0) {
            if (is_numeric(argv[arg_index] + 9)) {
                args->tdelay = strtol(argv[arg_index] + 9, NULL, 10);
            }
            else {
                fprintf(stderr, "Error: Please use legal command line arguments!\n");
                exit(1);
            }
        }
        else {
            fprintf(stderr, "Error: Please use legal command line arguments!\n");
            exit(1);
        }
        arg_index += 1;
    }

    // consider the case that no arguments
    if (args->show_memory == 0 && args->show_cpu == 0 && args->show_cores == 0) {
        args->show_memory = 1;
        args->show_cpu = 1;
        args->show_cores = 1;
    }
}