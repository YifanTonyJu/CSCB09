#ifndef CORES_H
#define CORES_H

#include <stdio.h>
#include <stdlib.h>
#include "CLprocesssor.h"
/**
*Define a function to get the number of cpu cores from
*corresponding file and return the number.
 */
int get_cpu_cores();

/**
*Define a function to get the max cpu frequency from
*corresponding file and return the number.
*/
double get_max_cpu_frequency();

/**
*Define a function that display a single core based on the core number.
*/
void display_single_core(int core_num, CLArgs *args);

#endif//CORES_H