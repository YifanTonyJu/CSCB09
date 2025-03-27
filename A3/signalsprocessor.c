# include "signalsprocessor.h"

int pid_mem_register(int pid_mem) {
    static int pid_mem_regi = EMPTY_REGISTER;
    if (pid_mem == GET_PID) {
        return pid_mem_regi;
    }
    else {
        pid_mem_regi = pid_mem;
        return LOAD_SUCCEED;
    }
}

int pid_cpu_register(int pid_cpu) {
    static int pid_cpu_regi = EMPTY_REGISTER;
    if (pid_cpu == GET_PID) {
        return pid_cpu_regi;
    }
    else {
        pid_cpu_regi = pid_cpu;
        return LOAD_SUCCEED;
    }
}

int pid_core_count_register(int pid_core_count) {
    static int pid_core_count_regi = EMPTY_REGISTER;
    if (pid_core_count == GET_PID) {
        return pid_core_count_regi;
    }
    else {
        pid_core_count_regi = pid_core_count;
        return LOAD_SUCCEED;
    }
}

int pid_core_freq_register(int pid_core_freq) {
    static int pid_core_freq_regi = EMPTY_REGISTER;
    if (pid_core_freq == GET_PID) {
        return pid_core_freq_regi;
    }
    else {
        pid_core_freq_regi = pid_core_freq;
        return LOAD_SUCCEED;
    }
}

void handle_sigint(int signo) {
    //get child process pid
    int pids[4];
    pids[PID_MEM_INDEX] = pid_mem_register(GET_PID);
    pids[PID_CPU_INDEX]= pid_cpu_register(GET_PID);
    pids[PID_CORE_COUNT_INDEX]= pid_core_count_register(GET_PID);
    pids[PID_CORE_FREQ_INDEX] = pid_core_freq_register(GET_PID);
    
    printf("\nGet Ctrl-C (SIGINT). Do you want to exit? (Use:y/n): ");
    fflush(stdout);  // we don't have '\n' at the end of the prompt, then use fflush() to make sure the prompt is shown on the screen
    
    char buffer[16];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        // EOF or no response
        printf("Sorry,no input detected.\n");
        return;
    }

    if (buffer[0] == 'y' || buffer[0] == 'Y') {
        printf("Exiting the program...\n");

        //kill child process first
        for (int i = 0; i < 4; ++i) {
            //check if is a valid
            if (pids[i] != EMPTY_REGISTER) {
                if (kill(pids[i], SIGTERM) == -1) {
                    perror("Failed to kill child process");
                    if (i == PID_MEM_INDEX) {
                        fprintf(stderr,"Please check process pid_mem.\n");
                    }
                    else if (i == PID_CPU_INDEX) {
                        fprintf(stderr,"Please check process pid_cpu.\n");
                    }
                    else if (i == PID_CORE_COUNT_INDEX) {
                        fprintf(stderr,"Please check process pid_core_count.\n");
                    }
                    else {
                        fprintf(stderr,"Please check process pid_core_freq.\n");
                    }
                }
            }
        }

        //exit the parent process
        exit(0);
    }
    else {
        printf("Continuing the program.\n");
    }
}

void handle_sigtstp(int signo) {
    //Ctrl-Z (SIGTSTP) is ignored in this program as the program should not be run in the background while running interactively.ss
}

void signal_handlers_binding() {
    // Bind handler with SIGINT (Ctrl-C)
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        fputs("Error: Failed to bind SIGINT handler.\n", stderr);
        exit(EXIT_FAILURE); // fail to bind the handler
    }

    // Bind handler with SIGTSTP (Ctrl-Z)
    if (signal(SIGTSTP, handle_sigtstp) == SIG_ERR) {
        fputs("Error: Failed to bind SIGTSTP handler.\n", stderr);
        exit(EXIT_FAILURE); // fail to bind the handler
    }
}