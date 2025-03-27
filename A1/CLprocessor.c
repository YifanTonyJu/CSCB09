#include "CLprocesssor.h"
#include <ctype.h>

/*
Define a function to check whether the str is a pure int number and return 1 if is true.
*/
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

    
    if (argc >= 2) {
        for (int i = 1; i < argc; ++i) {
            if (strcmp(argv[i], "--memory") == 0) {
                args->show_memory = 1;
            }
            else if (strcmp(argv[i], "--cpu") == 0) {
            args->show_cpu = 1;
            }
            else if (strcmp(argv[i], "--cores") == 0) {
            args->show_cores = 1;
            }
            //ensure the sample in the front is accompanied with tdelay
            else if (i == 1 && is_numeric(argv[i]) && argc >= 3 && is_numeric(argv[i + 1])) {
                    args->samples = strtol(argv[i], NULL, 10);
            }
            else if (i == 2 && is_numeric(argv[i])) {
                args->tdelay = strtol(argv[i], NULL, 10);
            }
            else if (strncmp(argv[i], "--samples=", 10) == 0) {
                args->samples = strtol(argv[i] + 10, NULL, 10);
            }
            else if (strncmp(argv[i], "--tdelay=", 9) == 0) {
                args->tdelay = strtol(argv[i] + 9, NULL, 10);
            }
            else {
                fprintf(stderr, "Error:Please use legal commend line arguments\n");
                exit(1);
            }
        }
        //consider the case that no --memory and --cpu and --tdelay
        if (args->show_memory == 0 && args->show_cpu == 0 && args->show_cores == 0) {
            args->show_memory = 1;
            args->show_cpu = 1;
            args->show_cores = 1;
        }
    }
    else {
    args->show_memory = 1;
    args->show_cpu = 1;
    args->show_cores = 1;
    }
}