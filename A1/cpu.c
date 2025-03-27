#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void get_cpu_time(CPUtime *cpu_time) {
    char buffer[256];
    char cpu[5];
    long user, nice, system, idle, iowait, irq, softirq, steal;
    
    //open the corresponding file
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Unable to open /proc/stat");
        exit(1);
    }

    //get data from the file
    rewind(file);
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "%s %ld %ld %ld %ld %ld %ld %ld %ld",cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);

    //check whether read the right line
    if (strcmp(cpu, "cpu") != 0) {
        fprintf(stderr, "Error: Read the wrong line!\n");
        exit(1);
    }

    //calculate total cpu time and active cpu time
    cpu_time->total = user + nice + system + idle + iowait + irq + softirq + steal;
    cpu_time->active = cpu_time->total - (idle + iowait);

    fclose(file);
}

double get_cpu_usage(CPUtime *old_cpu_time, CPUtime *new_cpu_time) {
    long old_total_time = old_cpu_time->total;
    long old_active_time = old_cpu_time->active;
    long new_total_time = new_cpu_time->total;
    long new_active_time = new_cpu_time->active;

    return ((new_active_time - old_active_time) / (double)(new_total_time - old_total_time)) * 100.0;
}

void display_cpu_axes(int samples, CLArgs *args ){
    //consider the case that memory utilization also need to be printed
    if (args->show_memory == 1) {
        printf ("\033[6;1Hv CPU");
        //print y-axe
        printf("\033[7;3H100%% |");
        for (int i = 8; i < 17; ++i) {
            printf("\033[%d;%dH%c", i, 8, '|');
        }
        //print x-axe
        printf("\033[17;5H0%%");
        for (int j = 8; j <= samples + 8; ++j) {
        printf("\033[17;%dH-", j);
        }
    }

    //consider other cases
    else {
        printf ("\033[3;1Hv CPU");
        //print y-axe
        printf("\033[4;3H100%% |");
        for(int i = 5; i <= 13; ++i) {
            printf("\033[%d;8H|", i);
        }
        //print x-axe
        printf("\033[14;5H0%%");
        for (int j = 8; j <= samples + 8; ++j) {
        printf("\033[14;%dH-", j);
        }
    }
}

void display_cpu_plot(double cpu_usage, int time, CLArgs *args){
    //consider the case that memory utilization also need to be printed
    if (args->show_memory == 1) {
        int row = 16 - (int)(cpu_usage / 10);
        if(row < 7) {
            row = 7;
        }
        printf("\033[%d;%dH%c", row, 8 + time, ':');
        fflush(stdout);
    }
    //consider other cases
    else {
        int row = 13 - (int)(cpu_usage / 10);
        if(row < 4) {
            row = 4;
        }
        printf("\033[%d;%dH%c", row, 8 + time, ':');
        fflush(stdout);
    }
}