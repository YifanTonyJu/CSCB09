#include "cores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_cpu_cores(){
    FILE *file = fopen("/proc/cpuinfo", "r");
    if (!file) {
        perror("Unable to open /proc/cpuinfo");
        exit(1);
    }

    int count = 0;
    char buffer[256];
    char *buffer_ptr;

    rewind(file);
    buffer_ptr = fgets(buffer, sizeof(buffer), file);
    
    while (buffer_ptr != NULL) {
        if (strncmp(buffer, "processor", 9) == 0) {
            count += 1;
        }
        buffer_ptr = fgets(buffer, sizeof(buffer), file);
    }

    fclose(file);
    return count;
}

double get_max_cpu_frequency() {
    FILE *file = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "r");
    if (!file) {
        perror("Unable to open /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq");
        //try to get max cpu frequency by reading another file /proc/cpuinfo
        file = fopen("/proc/cpuinfo", "r");
        if (!file) {
        perror("Unable to open /proc/cpuinfo");
        exit(1);
        }

        char buffer[256];
        double max_freq = 0.0;
        double current_freq;
        char *buffer_ptr;

        rewind(file);
        buffer_ptr = fgets(buffer, sizeof(buffer), file);

        //read each line of the file and get max frequency
        while (buffer_ptr != NULL) {
            if (sscanf(buffer_ptr, "cpu MHz : %lf", &current_freq) == 1) {
                if (current_freq > max_freq) {
                max_freq = current_freq;
                }
            }
            buffer_ptr = fgets(buffer, sizeof(buffer), file);
        }

        fclose(file);
        return max_freq / 1000.0;
    }
    long max_freq;
    fscanf(file, "%ld", &max_freq);
    fclose(file);

    return max_freq / 1000000.0;
}

void display_single_core(int core_num, CLArgs *args) {
    //find the place where the core is in the whole graph
    int row_index;
    int col_index;
    int lt_col_index;
    int lt_row_index;
    if (core_num % 4 != 0){
        row_index = core_num / 4 + 1;
        col_index = core_num - ((core_num / 4) * 4);
    }
    else {
        row_index = core_num / 4;
        col_index = 4;
    }

    //consider the case that memory and cpu also need to be shown
    if (args->show_memory + args->show_cpu == 2){
        //find the left top + coord
        lt_col_index = 1 + (col_index - 1) * 7;// get x coord
        lt_row_index = 3 * (row_index + 1);
    }
    //consider the case that only one of the other two graph need to be shown
    else if (args->show_memory + args->show_cpu == 1) {
        //find the left top + coord
        lt_col_index = 1 + (col_index - 1) * 7;// get x coord
        lt_row_index = 3 * (row_index + 2);
    }
    //only need to show cores
    else {
        //find the left top + coord
        lt_col_index = 1 + (col_index - 1) * 7;// get x coord
        lt_row_index = 1 + 3 * row_index;
    }

    // display a core based on the left top + coord
    printf("\033[%d;%dH%c", lt_row_index, lt_col_index, '+');
    printf("\033[%d;%dH%c", lt_row_index, lt_col_index + 4, '+');
    printf("\033[%d;%dH%c", lt_row_index + 2, lt_col_index, '+');
    printf("\033[%d;%dH%c", lt_row_index + 2, lt_col_index + 4, '+');
    printf("\033[%d;%dH%c", lt_row_index + 1, lt_col_index, '|');
    printf("\033[%d;%dH%c", lt_row_index + 1, lt_col_index + 4, '|');
    for (int i = 1; i <= 3; ++ i ) {
        printf("\033[%d;%dH%c", lt_row_index, lt_col_index + i, '-');
        printf("\033[%d;%dH%c", lt_row_index + 2, lt_col_index + i, '-');
    }
    return;
}