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
    if (file == NULL) {
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

void display_core_graph(int cores_num) {
    int row_num = cores_num / 4;
    int remain = cores_num - row_num * 4;

    for(int row = 0; row < row_num; ++row) {
        printf("+---+  +---+  +---+  +---+\n");
        printf("|   |  |   |  |   |  |   |\n");
        printf("+---+  +---+  +---+  +---+\n");
    }
    if (remain == 3) {
        printf("+---+  +---+  +---+\n");
        printf("|   |  |   |  |   |\n");
        printf("+---+  +---+  +---+\n");
    }
    else if (remain == 2) {
        printf("+---+  +---+\n");
        printf("|   |  |   |\n");
        printf("+---+  +---+\n");
    }
    else if (remain == 1) {
        printf("+---+\n");
        printf("|   |\n");
        printf("+---+\n");
    }
    else {
        return;
    }
    printf("\n");
}