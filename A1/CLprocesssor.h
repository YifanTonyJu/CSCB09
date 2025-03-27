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

/**
* Define a function that process the commend line given by users.
*/
void commend_arguments(int argc, char **argv, CLArgs *args);

#endif//CLPROCESSOR_H