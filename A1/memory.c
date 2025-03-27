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
        perror("sysinfo");
        exit(1);
    }
}

double get_available_memory () {
    long available_memory = 0;
    char buffer[256];
    char *buffer_ptr;

    //open file to read date about memory and check whether successfully open the file
        FILE *file = fopen("/proc/meminfo", "r");
        if (!file) {
        perror("Unable to open /proc/meminfo");
        exit(1);
        }

    //read each line of the file and find MemAvailable
    rewind(file);
    buffer_ptr = fgets(buffer, sizeof(buffer), file);
    while(buffer_ptr != NULL) {
        if(sscanf(buffer_ptr, "MemAvailable: %ld kB", &available_memory) == 1) {
            break;
        }
        buffer_ptr = fgets(buffer, sizeof(buffer), file);
    }
    fclose(file);

    return (available_memory / 1024.0) / 1024.0;

}

void display_memory_axes(double total_memory, int samples){
    printf ("\033[3;1Hv Memory");
    //print y-axe
    printf("\033[4;2H%d GB |", (int)total_memory);
    for (int i = 5; i <= 13; ++i) {
        printf("\033[%d;%dH%c", i, 8, '|');
    }
    //print x-axe
    printf("\033[14;3H0 GB");
    for (int j = 8; j <= samples + 8; ++j) {
        printf("\033[14;%dH-", j);
    }
}

void display_memory_plot(double total_memory, double used_memory, int time){
    int row = 13 - (int)(used_memory / (total_memory / 10));
    if (row < 4) {
        row = 4;
    }
    printf("\033[%d;%dH%c", row, 8 + time, '#');
    fflush(stdout);
}