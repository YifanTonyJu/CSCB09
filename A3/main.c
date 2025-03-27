#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      // fork, pipe, close, usleep
#include <sys/types.h>   // pid_t
#include <sys/wait.h>    // waitpid
#include "CLprocesssor.h"
#include "cpu.h"
#include "memory.h"
#include "cores.h"
#include "signalsprocessor.h"

///_|> descry: this function is the main entry point of the project, responsible for command line processing, signal handling, process forking, inter-process communication with pipes, and display the info on screen.
///_|> argc: the number of command line arguments + 1, type int
///_|> argv: array of command line argument strings, type char**
///_|> returning: this function return whether the project has successfully run, return 0 if yes, otherwise will exit the program, type int
int main(int argc, char **argv) {
    // process commend line arguments
    CLArgs commends;
    commend_arguments(argc, argv, &commends);

    // process Ctrl-C and Ctrl-Z
    signal_handlers_binding();

    // store the sample and delay info
    int samples = commends.samples;
    int delay = commends.tdelay;

    // clean the screen and move the cursor to the top. then print the sample and delay information
    printf("\033[2J\033[H");
    printf(" Nbr of sample: %d -- every %d microSecs (%.3f secs)\n", samples, delay, (double)delay / 1000000);
    printf("\n");

    // create 4 arrays to store pipes' fd
    int pipe_mem[2], pipe_cpu[2], pipe_core_count[2], pipe_core_freq[2];

    // create pipes with error checking
    if (pipe(pipe_mem) == -1) {
        perror("creating pipe_mem failed");
        exit(1);
    }
    if (pipe(pipe_cpu) == -1) {
        perror("creating pipe_cpu failed");
        exit(1);
    }
    if (pipe(pipe_core_count) == -1) {
        perror("creating pipe_core_count failed");
        exit(1);
    }
    if (pipe(pipe_core_freq) == -1) {
        perror("creating pipe_core_freq failed");
        exit(1);
    }

    // start to get data and write to the created pipes

    // create a process to get total memory and memory usage with error checking
    pid_t pid_mem = fork();
    if (pid_mem == -1) {
        perror("creating process pid_mem failed");
        exit(1);
    }
    //load the pid into register
    if (pid_mem != 0) {
        if (pid_mem_register(pid_mem) != 0) {
            fprintf(stderr, "Error: Fail to load the pid_mem!\n");
            exit(1);
        }
    }

    // the mem process
    if (pid_mem == 0) {
        signal(SIGINT, SIG_DFL); //refresh SIGINT
        signal(SIGTSTP, SIG_IGN); //refresh SIGTSTP
        // close read end with error checkings
        if (close(pipe_mem[0]) == -1) {
            perror("fail to close pipe_mem[0] in child");
            exit(1);
        }

        if (commends.show_memory == 1) {
            double total_memory = get_total_memory(); // get total memory

            //checking the total memory result
            if (total_memory <= 0) {
                fprintf(stderr, "Error: failed to read total memory!\n");
                exit(1); // impossible!
            }

            // write the total memory result to pipe_mem
            if (write(pipe_mem[1], &total_memory, sizeof(double)) == -1) {
                perror("write total memory failed");
                exit(1);
            }

            // get memory usage with several iterations
            for (int i = 1; i <= samples; ++i) {
                double used = total_memory - get_available_memory(); // get memory usage

                // write the memory usage result to the pipe_mem with error checking
                if (write(pipe_mem[1], &used, sizeof(double)) == -1) {
                    perror("write used memory failed");
                    exit(1);
                }

                // wait for next iteration, except for the last iteration
                if (i < samples) {
                    usleep(delay); // wait for next iteration
                }
            }
        }

        // close child write end with error checking
        if (close(pipe_mem[1]) == -1) {
            perror("fail to close pipe_mem[1] in child");
            exit(1);
        }

        exit(0); // the process finished
    }

    // create a process to get cpu usage with error checking
    pid_t pid_cpu = fork();
    if (pid_cpu == -1) {
        perror("creating process pid_cpu failed");
        exit(1);
    }
    //load the pid into register
    if (pid_cpu != 0) {
        if (pid_cpu_register(pid_cpu) != 0) {
            fprintf(stderr, "Error: Fail to load the pid_cpu!\n");
            exit(1);
        }
    }

    // the cpu process
    if (pid_cpu == 0) {
        signal(SIGINT, SIG_DFL); //refresh SIGINT
        signal(SIGTSTP, SIG_IGN); //refresh SIGTSTP
        // close read end with error checking
        if (close(pipe_cpu[0]) == -1) {
            perror("fail to close pipe_cpu[0] in child");
            exit(1);
        }

        if (commends.show_cpu == 1) {
            CPUtime old;//store U_1 and T_1
            CPUtime new;//store U_2 and T_2

            get_cpu_time(&old); // get cpu info first before the interation begin
            usleep(delay); // stop for the iteration begin

            // start the iterations
            for (int i = 1; i <= samples; ++i) {
                get_cpu_time(&new); //get U_2 and T_2
                double usage = get_cpu_usage(&old, &new); // get cpu usage
                old = new; // update U_1 and T_1 for next interation

                // write the cpu usage to the pipe_cpu with error checking
                if (write(pipe_cpu[1], &usage, sizeof(double)) == -1) {
                    perror("write cpu usage failed");
                    exit(1);
                }

                // wait for next iteration, except for the last iteration
                if (i < samples) {
                    usleep(delay);
                }
            }
        }

        // close child write end with error checking
        if (close(pipe_cpu[1]) == -1) {
            perror("fail to close pipe_cpu[1] in child");
            exit(1);
        }

        exit(0); // the process finished
    }

    // create a process to get cores number with error checking
    pid_t pid_core_count = fork();
    if (pid_core_count == -1) {
        perror("creating process pid_core_count failed");
        exit(1);
    }
    //load the pid into register
    if (pid_core_count != 0) {
        if (pid_core_count_register(pid_core_count) != 0) {
            fprintf(stderr, "Error: Fail to load the pid_core_count!\n");
            exit(1);
        }
    }

    // the cores number process
    if (pid_core_count == 0) {
        signal(SIGINT, SIG_DFL); //refresh SIGINT
        signal(SIGTSTP, SIG_IGN); //refresh SIGTSTP
        // close read end with error checking
        if (close(pipe_core_count[0]) == -1) {
            perror("close pipe_core_count[0] in child");
            exit(1);
        }

        if (commends.show_cores == 1) {
            int cores = get_cpu_cores(); // get cores num

            // write the num to the pipe_core_count with error checking
            if (write(pipe_core_count[1], &cores, sizeof(int)) == -1) {
                perror("write core count failed");
                exit(1);
            }
        }

        // close child write end with error checking
        if (close(pipe_core_count[1]) == -1) {
            perror("fail to close pipe_core_count[1] in child");
            exit(1);
        }

        exit(0); // the process finished
    }

    // create a process to get cpu max. frequency with error checking
    pid_t pid_core_freq = fork();
    if (pid_core_freq == -1) {
        perror("creating pid_core_freq failed");
        exit(1);
    }
    //load the pid into register
    if (pid_core_freq != 0) {
        if (pid_core_freq_register(pid_core_freq) != 0) {
            fprintf(stderr, "Error: Fail to load the pid_core_freq!\n");
            exit(1);
        }
    }

    // the max. frequency process
    if (pid_core_freq == 0) {
        signal(SIGINT, SIG_DFL); //refresh SIGINT
        signal(SIGTSTP, SIG_IGN); //refresh SIGTSTP
        // close read end with error checking
        if (close(pipe_core_freq[0]) == -1) {
            perror("fail to close pipe_core_freq[0] in child");
            exit(1);
        }
        if (commends.show_cores == 1) {
            double freq = get_max_cpu_frequency(); // get max. frequency

            // write the max. frequency to the pipe_core_freq with error checking
            if (write(pipe_core_freq[1], &freq, sizeof(double)) == -1) {
                perror("write core freq failed");
                exit(1);
            }
        }

        // close child write end with error checking
        if (close(pipe_core_freq[1]) == -1) {
            perror("fail to close pipe_core_freq[1] in child");
            exit(EXIT_FAILURE);
        }

        exit(0); // the process finished
    }

    // start to read data from pipes and display on the screen

    // close write ends with error checking, the parent process just need to read the pipe
    if (close(pipe_mem[1]) == -1) {
        perror("close pipe_mem[1] failed");
        exit(1);
    }
    if (close(pipe_cpu[1]) == -1) {
        perror("close pipe_cpu[1] failed");
        exit(1);
    }
    if (close(pipe_core_count[1]) == -1) {
        perror("close pipe_core_count[1] failed");
        exit(1);
    }
    if (close(pipe_core_freq[1]) == -1) {
        perror("close pipe_core_freq[1] failed");
        exit(1);
    }

    // display both mem info and cpu info
    if (commends.show_memory == 1 && commends.show_cpu  == 1) {
        double total_memory; // store total memory

        // read from pipe_mem to get total memory with error checking
        if (read(pipe_mem[0], &total_memory, sizeof(double)) == -1) {
            perror("read total memory failed");
            exit(1);
        }

        display_memory_axes(total_memory, samples); // display the mem usage table
        printf("\n");
        display_cpu_axes(samples); // display the cpu usage table

        // get mem usage and cpu usage
        for (int t = 1; t <= samples; ++t) {
            double used_mem, cpu_use; // store the mem usage and cpu usage

            // read from pipe_mem and pipe_cpu to get memory usage and cpu usage with error checking
            if (read(pipe_mem[0], &used_mem, sizeof(double)) == -1) {
                perror("read used memory failed");
                exit(1);
            }
            if (read(pipe_cpu[0], &cpu_use, sizeof(double)) == -1) {
                perror("read cpu usage failed");
                exit(1);
            }

            //display mem usage and cpu usage on the screen
            display_memory_plot(total_memory, used_mem, t); // print mem plot on the screen
            display_cpu_plot(cpu_use, t, &commends); // print cpu plot on the screen
        }
        printf("\x1b[%d;%df", 24, 1);
        printf("\n\n");
    }

    // just display mem info
    else if (commends.show_memory == 1 && commends.show_cpu != 1) {
        double total_memory; // store total memory

        // read from pipe_mem to get total memory with error checking
        if (read(pipe_mem[0], &total_memory, sizeof(double)) == -1) {
            perror("read total memory failed");
            exit(1);
        }

        display_memory_axes(total_memory, samples);  // display the mem usage table

        // get mem usage
        for (int t = 1; t <= samples; ++t) {
            double used; // store mem usage

            // read from pipe_mem to get mem usage with error checking
            if (read(pipe_mem[0], &used, sizeof(double)) == -1) {
                perror("read used memory failed");
                exit(1);
            }

            // display mem info on the screen
            display_memory_plot(total_memory, used, t); // print mem plot on the screen
        }
        printf("\x1b[%d;%df", 13, 1);
        printf("\n\n");
    }

    // just display cpu info
    else if (commends.show_memory != 1 && commends.show_cpu == 1) {
        display_cpu_axes(samples); // display the cpu usage table
        for (int t = 1; t <= samples; ++t) {
            double usage; // store cpu usage

            // read from pipe_cpu to get cpu usage with error checking
            if (read(pipe_cpu[0], &usage, sizeof(double)) == -1) {
                perror("read cpu usage failed");
                exit(1);
            }

            // display cpu info on the screen
            display_cpu_plot(usage, t, &commends); // print cpu plot on the screen
        }

        printf("\x1b[%d;%df", 13, 1);
        printf("\n\n");
    }

    // display cores info
    if (commends.show_cores == 1) {
        int cores; // store cores num
        double freq; // store max. frequency

        // read from pipe_core_count and pipe_core_freq to get cores num and cores max. frequency with error checking
        if (read(pipe_core_count[0], &cores, sizeof(int)) == -1) {
            perror("read core count failed");
            exit(1);
        }
        if (read(pipe_core_freq[0], &freq, sizeof(double)) == -1) {
            perror("read core freq failed");
            exit(1);
        }

        // display cores num and max. frequency info
        printf("v Number of Cores: %d @ %.2f GHz\n", cores, freq); // print cores max. frequency

        // display cores graph
        display_core_graph(cores);
    }

    // wait process to adopt them with error checking
    int status; // store exit status info
    if (waitpid(pid_mem, &status, 0) == -1) {
        perror("waitpid pid_mem failed");
        exit(1);
    }
    if (waitpid(pid_cpu, &status, 0) == -1) {
        perror("waitpid pid_cpu failed");
        exit(1);
    }
    if (waitpid(pid_core_count, &status, 0) == -1) {
        perror("waitpid pid_core_count failed");
        exit(1);
    }
    if (waitpid(pid_core_freq, &status, 0) == -1) {
        perror("waitpid pid_core_freq failed");
        exit(1);
    }

    // close read ends in the parent process with error checking, as we have finished the reading part
    if (close(pipe_mem[0]) == -1) {
        perror("close pipe_mem[0] failed");
        exit(1);
    }
    if (close(pipe_cpu[0]) == -1) {
        perror("close pipe_cpu[0] failed");
        exit(1);
    }
    if (close(pipe_core_count[0]) == -1) {
        perror("close pipe_core_count[0] failed");
        exit(1);
    }
    if (close(pipe_core_freq[0]) == -1) {
        perror("close pipe_core_freq[0] failed");
        exit(1);
    }

    // the whole process finished
    return 0;
}