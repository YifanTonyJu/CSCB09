#ifndef CORES_H
#define CORES_H

#include <stdio.h>
#include <stdlib.h>
#include "CLprocesssor.h"

int get_cpu_cores();
///_|> descry: this function get the cores num by reading "/proc/cpuinfo"
///_|> returning: this function returning the cores num, type int

double get_max_cpu_frequency();
///_|> descry: this function get the cores max. frequency by reading "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq" or "/proc/cpuinfo"
///_|> returning: this function returning the cores max. frequency, type double

void display_core_graph(int cores_num);
///_|> descry: this function draw the core squares in the cores graph, 4 squares a row
///_|> cores_num: indicates the number of the core squares to draw, type int
///_|> returning: this function does not return anything

#endif//CORES_H