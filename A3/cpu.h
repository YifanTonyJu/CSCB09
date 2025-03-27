#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include "CLprocesssor.h"

#define CPU_Y_AXE_LEN 8

typedef struct {
    long total;// CPU total running time
    long active;//CPU active running time
} CPUtime;

void get_cpu_time(CPUtime *cpu_time);
///_|> descry: this function read from "/proc/stat" to get cpu total time and cpu active time, then store the info in the place pointed by cpu_time
///_|> cpu_time: a pointer to the place that store cpu time info, type CPUtime *
///_|> returning: this function does not return anything

double get_cpu_usage(CPUtime *old_cpu_time, CPUtime *new_cpu_time);
///_|> descry: this function use the formula (U_2 - U_1) / (T_2 - T_1) * 100 to calculate the cpu usage
///_|> old_cpu_time: the pointer to the place store U_1 and T_1, type CPUtime *
///_|> new_cpu_time: the pointer to the place store U_2 and T_2, type CPUtime *
///_|> returning: this function returns the cpu usage, type double

void display_cpu_axes(int samples);
///_|> descry: this function print the cpu table axes on the screen
///_|> samples: indicates the number of required samples, type int
///_|> returning: this function does not return anything

void display_cpu_plot(double cpu_usage, int time, CLArgs *args);
///_|> descry: this function print a cpu table plot on the screen
///_|> cpu_usage: indicate the cpu usage, type double
///_|> time: indicate the num of the sample corresponding to the plot to be print, type int
///_|> args: the pointer to the place stores commend line arguments info, type CLArgs *
///_|> returning: this function does not return anything

#endif//CPU_H