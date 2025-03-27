#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include "CLprocesssor.h"

typedef struct {
    long total;// CPU total running time
    long active;//CPU active running time
} CPUtime;

/**
*Define a function to get total CPU time and active CPU time
*from the corresponding file and store the data in a place pointed by cpu_time.
*/
void get_cpu_time(CPUtime *cpu_time);

/**
*Define a function to calculate CPU utilization with two given pointers
*that point to two places that store CPU time data.
*/
double get_cpu_usage(CPUtime *old_cpu_time, CPUtime *new_cpu_time);

/**
*Define a function to display axes for CPU utilization,
*the position is based on the commend lines.
*If there is memory utilization need to be shown, the the axes
*should be under it,or it has to be shown on the top.
*/
void display_cpu_axes(int samples, CLArgs *args );

/**
*Define a function to display plot for CPU utilization,
*the position is related to the axes position.
*/
void display_cpu_plot(double cpu_usage, int time, CLArgs *args);

#endif//CPU_H