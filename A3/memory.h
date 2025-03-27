#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

#define MEM_Y_AXE_LEN 8

double get_total_memory();
///_|> descry: this function get the total amount of memory with "sys/sysinfo.h"
///_|> returning: this function returns the total memory of the system in GB, type double

double get_available_memory ();
///_|> descry: this function get the free amount of memory with "sys/sysinfo.h"
///_|> returning: this function returns the free memory of the system in GB, type double

void display_memory_axes(double total_memory, int samples);
///_|> descry: this function print the memory table axes on the screen
///_|> total_memory: indicates the total memory of the system, type double
///_|> samples: indicates the number of required samples, type int
///_|> returning: this function does not return anything

void display_memory_plot(double total_memory, double used_memory, int time);
///_|> descry: this function print a memory table plot in the table
///_|> total_memory: indicates the total memory of the system, type double
///_|> used_memory: indicates the used memory of the system, type double
///_|> time: indicate the num of the sample corresponding to the plot to be print, type int
///_|> returning: this function does not return anything

#endif//MEMORY_H
