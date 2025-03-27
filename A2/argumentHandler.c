#include "argumentHandler.h"

void handle_arguments(int argc, char **argv, Arguments *args) {
    // initial the info struct
    args->per_process = false;
    args->system_wide = false;
    args->vnodes = false;
    args->composite = false;
    args->summary = false;
    args->output_txt = false;
    args->output_binary = false;
    args->threshold = -1;
    args->pid = -1;

    //read the arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--per-process") == 0) {
            args->per_process = true;
        }
        else if (strcmp(argv[i], "--systemWide") == 0) {
            args->system_wide = true;
        }
        else if (strcmp(argv[i], "--Vnodes") == 0) {
            args->vnodes = true;
        }
        else if (strcmp(argv[i], "--composite") == 0) {
            args->composite = true;
        }
        else if (strcmp(argv[i], "--summary") == 0) {
            args->summary = true;
        }
        else if (strncmp(argv[i], "--threshold=", 11) == 0) {
            sscanf(argv[i], "--threshold=%d", &(args->threshold));
        }
        else if (strcmp(argv[i], "--output_TXT") == 0) {
            args->output_txt = true;
        }
        else if (strcmp(argv[i], "--output_binary") == 0) {
            args->output_binary = true;
        }
        else {
            if (!isdigit(argv[i][0])) {
                fprintf(stderr, "Error: Please use correct commend line arguments!\n");
                exit(1);
            }
            int pid = strtol(argv[i], NULL, 10);
            if (pid > 0) {
                args->pid = pid;
            }
            else {
                fprintf(stderr, "Error: Invalid PID: %s, need to be positive!\n", argv[i]);
            }
        }
    }

    //if no flagged argument and positional arguments is passed to the program, then display composed table.
    if (!(args->per_process || args->system_wide || args->vnodes || args->composite || args->summary || args->threshold != -1 || args->output_binary || args->output_txt || args->pid != -1)) {
        args->composite = true;
    }

    //if we have a positional argument, and only give summary or threshold or .txt or .bin arguments, then display composed table of that pid and the summary or threshold table or save as .txt or .bin
    else if(args->pid != -1) {
        if (!(args->per_process || args->system_wide || args->vnodes || args->composite || args->output_binary || args->output_txt)) {
            // print all the tables
            args->per_process = true;
            args->system_wide = true;
            args->vnodes = true;
            args->composite = true;
        }
    }
}