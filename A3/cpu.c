#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void get_cpu_time(CPUtime *cpu_time) {
    char buffer[256];
    char cpu[5];
    long user, nice, system, idle, iowait, irq, softirq;
    
    //open the corresponding file
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Unable to open /proc/stat");
        exit(1);
    }

    //get data from the file
    rewind(file);
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "%s %ld %ld %ld %ld %ld %ld %ld",cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq);

    //check whether read the right line
    if (strcmp(cpu, "cpu") != 0) {
        fprintf(stderr, "Error: Read the wrong line!\n");
        exit(1);
    }

    //calculate total cpu time and active cpu time
    cpu_time->total = user + nice + system + idle + iowait + irq + softirq;
    cpu_time->active = cpu_time->total - idle;

    fclose(file);
}

double get_cpu_usage(CPUtime *old_cpu_time, CPUtime *new_cpu_time) {
    long old_total_time = old_cpu_time->total;
    long old_active_time = old_cpu_time->active;
    long new_total_time = new_cpu_time->total;
    long new_active_time = new_cpu_time->active;

    return ((new_active_time - old_active_time) / (double)(new_total_time - old_total_time)) * 100.0;
}

void display_cpu_axes(int samples){
    printf("v CPU\n");
    printf("  100%% |\n");
    for(int i = 0; i < CPU_Y_AXE_LEN - 1; ++i) {
        printf("       |\n");
    }

    printf("    0%% ");
    for(int i = 0; i < samples; ++i) {
        printf("-");
    }
    printf("-\n");
}

void display_cpu_plot(double cpu_usage, int time, CLArgs *args){
    int row = 0;// get the row in table

    double per_label = 100.0 / CPU_Y_AXE_LEN;

    double remain = cpu_usage - (int)(cpu_usage / per_label) * per_label;
    if (remain == 0.0) {
        if (cpu_usage == 0) {
            row = 1;
        }
        else {
            row = (int)(cpu_usage / per_label);
        }
    }
    else {
        row = (int)(cpu_usage / per_label) + 1;
    }

    if (args->show_memory == 1) {
        printf("\x1b[%d;%df", 14, 8);
        printf("%.2f", cpu_usage);
        printf(" %%  ");
        printf("\x1b[%d;%df", 23 - row, time + 8);
        printf(":");
        fflush(stdout); // print on the screen immediately
    }
    else {
        printf("\x1b[%d;%df", 3, 8);
        printf("%.2f", cpu_usage);
        printf(" %% ");
        printf("\x1b[%d;%df", 12 - row, time + 8);
        printf(":");
        fflush(stdout); // print on the screen immediately
    }
}