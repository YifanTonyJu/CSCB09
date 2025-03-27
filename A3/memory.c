#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "memory.h"
#include <string.h>

double get_total_memory() {
    struct sysinfo info;

    if (sysinfo(&info) == 0) {
        long total_memory = info.totalram;
        return ((total_memory / 1024.0) / 1024.0) / 1024.0;
    }
    else {
        perror("sysinfo_total_memory");
        exit(1);
    }
}

double get_available_memory () {
    struct sysinfo info;

    if (sysinfo(&info) == 0) {
        long available_memory = info.freeram;
        return ((available_memory / 1024.0) / 1024.0) / 1024.0;
    }
    else {
        perror("sysinfo_available_memory");
        exit(1);
    }
}

void display_memory_axes(double total_memory, int samples) {
    printf("v Memory\n");

    // print y-axe
    printf(" %d GB |\n", (int)total_memory);
    for (int i = 0; i < MEM_Y_AXE_LEN - 1; ++i) {
        printf("       |\n");
    }

    // print x-axe
    printf("  0 GB ");
    for (int i = 0; i < samples; ++i) {
        printf("-");
    }
    printf("-\n");
}

void display_memory_plot(double total_memory, double used_memory, int time){
    printf("\x1b[%d;%df", 3, 11);
    printf("%.2f", used_memory);
    printf(" GB ");
    fflush(stdout); // print on the screen immediately

    int row = 0;// get the row in table

    if (used_memory > (int)(total_memory) && used_memory <= total_memory) {
        row = 8;
    }

    int max_memory = (int)total_memory;

    double per_label = (double)(max_memory) / MEM_Y_AXE_LEN;

    double remain = used_memory - (int)(used_memory / per_label) * per_label;
    if (remain == 0.0) {
        if (used_memory == 0) {
            row = 1;
        }
        else {
            row = (int)(used_memory / per_label);
        }
    }
    else {
        row = (int)(used_memory / per_label) + 1;
    }
    

    printf("\x1b[%d;%df", 12 - row, time + 8);
    printf("#");
    fflush(stdout); // print on the screen immediately
}