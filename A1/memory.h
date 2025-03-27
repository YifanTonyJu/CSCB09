#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
/**
*Define a function to get total memory and return it.
*/
double get_total_memory();
/**
*Define a function to get available memory and return it.
*/
double get_available_memory ();
/**
*Define a function to display axes with memory limits scale.
*/
void display_memory_axes(double total_memory, int samples);
/**
*Define a function to display a plot in the axes
*based on sample, used memory and total memory.
*/
void display_memory_plot(double total_memory, double used_memory, int time);

#endif//MEMORY_H
