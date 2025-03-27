#ifndef CLPROCESSOR_H
#define CLPROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int samples;//show sample number
    int tdelay;//show delay
    int show_memory;//if exists commend "--memory" then show 1, or show 0.
    int show_cpu;//if exists commend "--cpu" then show 1, or show 0.
    int show_cores;//if exists commend "--cores" then show 1, or show 0.
} CLArgs;

void commend_arguments(int argc, char **argv, CLArgs *args);
///_|> descry: this function handle the CLA and store the CLA info in the place pointed by args
///_|> argc: the number of command line arguments + 1, type int
///_|> argv: array of command line argument strings, type char**
///_|> args: the pointer that points to the place stores CLA info, type CLArgs *
///_|> returning: this function does not return anything

#endif//CLPROCESSOR_H