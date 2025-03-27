#include <stdio.h>
#include <unistd.h>
#include "memory.h"
#include "CLprocesssor.h"
#include "cpu.h"
#include "cores.h"

int main( int argc, char **argv) {
    //process commend line
    CLArgs commends;
    commend_arguments(argc, argv, &commends);

    int samples = commends.samples;
    int delay = commends.tdelay;
    
    //print running parameters
    printf("\033[2J");
    printf("\033[H");
    printf(" Nbr of sample: %d -- every %d microSecs (%.3f secs)\n", samples, delay, (double)delay / 1000000);

    if (commends.show_memory == 1) {
        //get total memory and print axes
        double total_memory = get_total_memory();
        double available_memory = 0;
        if (total_memory <= 0) {
        printf("\033[4;1H");
        fprintf(stderr, "Error: Fail to get total memory\n");
        return 1;
        }
        display_memory_axes(total_memory, samples);

        //get used memory "samples" times and show it in axes
        for(int time = 1; time <= samples; ++time) {
            //get used memory
            available_memory = get_available_memory();
            if (available_memory < 0) {
                printf("\033[15;1H");
                fprintf(stderr, "Error: Fail to get available memory\n");
                return 1;
            }
            double used_memory = total_memory - available_memory;
            
            //print the used memory and corresponding plot
            printf("\033[3;11H%.2f GB", used_memory);
            display_memory_plot(total_memory, used_memory, time);
            if (time < samples) {
                usleep(delay);
            }
        }
        printf("\033[14;1H\n");
    }
    if (commends.show_cpu == 1) {
        if(commends.show_memory == 1) {
            //scrolling down 10 row so that the graph will not excess the terminal
            for (int i = 0; i < 10; i++) {
                printf("\033[1S");
            }
        }

        //find two places to store old and new cpu time data
        CPUtime *old_cpu_time = malloc(sizeof(CPUtime));
        CPUtime *new_cpu_time = malloc(sizeof(CPUtime));

        //get cpu time first time for calculation
        get_cpu_time(old_cpu_time);

        //print the cpu axes
        display_cpu_axes(samples, &commends);
        usleep(delay);
        //get cpu utilization "samples" times and show it in axes
        for (int time = 1; time <= samples; ++time) {
            get_cpu_time(new_cpu_time);
            double cpu_utilization = get_cpu_usage(old_cpu_time, new_cpu_time);
            old_cpu_time->total = new_cpu_time->total;
            old_cpu_time->active = new_cpu_time->active;
            if(commends.show_memory == 1) {
                printf("\033[6;8H%.2f %%", cpu_utilization);
            }
            else if(commends.show_memory == 0) {
                printf("\033[3;8H%.2f %%", cpu_utilization);
            }
            display_cpu_plot(cpu_utilization, time, &commends);
            if (time < samples) {
                usleep(delay);
            }
        }

        //free heap
        free(old_cpu_time);
        free(new_cpu_time);

        if(commends.show_memory == 1) {
                printf("\033[17;1H\n");
        }
        else if(commends.show_memory == 0) {
                printf("\033[14;1H\n");
        }
    }
    if (commends.show_cores == 1) {
        //get cores and max cpu frequency
        int cores_num = get_cpu_cores();
        double max_cpu_freq = get_max_cpu_frequency();
        
        //display the title
        if(commends.show_memory + commends.show_cpu == 2) {
            //scrolling down 15 row so that the graph will not excess the terminal
            for (int i = 0; i < 15; i++) {
                printf("\033[1S");
            }
            printf("\033[5;1Hv Number of Cores: %d @ %.2f GHz", cores_num, max_cpu_freq);
        }
        else if (commends.show_memory + commends.show_cpu == 1) {
            //scrolling down 10 row so that the graph will not excess the terminal
            for (int i = 0; i < 10; i++) {
                printf("\033[1S");
            }
            printf("\033[8;1Hv Number of Cores: %d @ %.2f GHz", cores_num, max_cpu_freq);
        }
        else {
            printf("\033[3;1Hv Number of Cores: %d @ %.2f GHz", cores_num, max_cpu_freq);
        }

        //display cores
        for(int num = 1; num <= cores_num; ++num) {
            display_single_core(num, &commends);
        }

        if(commends.show_memory + commends.show_cpu == 2) {
            printf("\033[20;1H\n");
        }
        else if(commends.show_memory + commends.show_cpu == 1) {
            printf("\033[23;1H\n");
        }
        else {
            printf("\033[18;1H\n");
        }
    }
    return 0;
}