#ifndef ARGUMENT_HANDLER_H
#define ARGUMENT_HANDLER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

typedef struct argument_handling{
    bool per_process; // indicate whether display the process FD table
    bool system_wide; // indicate whether display the system-wide FD table
    bool vnodes; // indicate whether display the Vnodes FD table
    bool composite; // indicate whether display the composed table
    bool summary; // indicate whether display the summary table
    bool output_txt; // indicate whether to save into 'compositeTable.txt'
    bool output_binary; // indicate whether to save into 'compositeTable.bin'
    int threshold; // indicate the limitation of processes number, if there is no flag about limitation, then be -1
    int pid; //indicate the PID of the wanted process
} Arguments;

void handle_arguments(int argc, char **argv, Arguments *args);
///_|> descry: this function analyze the given commend line arguments and store the analyzed information into a struct called Arguments.
///_|> argc: indicate the number of commend line arguments plus 1, type int
///_|> argv: the pointer to an array of pointers that point at the first element of each commend line argument strings, type char**
///_|> args: the pointer to the memory that store Arguments data, type Arguments*
///_|> returning: this function does not return anything

#endif // ARGUMENT_HANDLER_H
