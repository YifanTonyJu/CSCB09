#ifndef SIGNALSPROCESSORS_H
#define SIGNALSPROCESSORS_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define GET_PID -1
#define LOAD_SUCCEED 0
#define EMPTY_REGISTER -2 // try to be different from GET_PID
#define PID_MEM_INDEX 0
#define PID_CPU_INDEX 1
#define PID_CORE_COUNT_INDEX 2
#define PID_CORE_FREQ_INDEX 3

int pid_mem_register(int pid_mem);
///_|> descry: this function is a register of pid_mem for Ctrl-C handler
///_|> pid_mem: the variable that store pid_mem to be load or -1 for getting, type int
///_|> returning: if pid_mem != -1, then store value in the pid_mem and return 0; if pid_mem == -1, return the pid_mem just stored

int pid_cpu_register(int pid_cpu);
///_|> descry: this function is a register of pid_cpu for Ctrl-C handler
///_|> pid_cpu: the variable that store pid_cpu to be load or -1 for getting, type int
///_|> returning: if pid_cpu != -1, then store value in the pid_cpu and return 0; if pid_cpu == -1, return the pid_cpu just stored

int pid_core_count_register(int pid_core_count);
///_|> descry: this function is a register of pid_core_count for Ctrl-C handler
///_|> pid_core_count: the variable that store pid_core_count to be load or -1 for getting, type int
///_|> returning: if pid_core_count != -1, then store value in the pid_core_count and return 0; if pid_core_count == -1, return the pid_core_count just stored

int pid_core_freq_register(int pid_core_freq);
///_|> descry: this function is a register of pid_core_freq for Ctrl-C handler
///_|> pid_core_freq: the variable that store pid_core_freq to be load or -1 for getting, type int
///_|> returning: if pid_core_freq != -1, then store value in the pid_core_freq and return 0; if pid_core_freq == -1, return the pid_core_freq just stored

void handle_sigint(int signo);
///_|> descry: this function handle the signal coming from "Ctrl-C"
///_|> signo: the signal that trigger this function, type int
///_|> returning: this function doesn't return anything

void handle_sigtstp(int signo);
///_|> descry : this function handle the signal coming from "Ctrl-Z"
///_|> signo: the signal that trigger this function, type int
///_|> returning: this function doesn't return anything

void signal_handlers_binding();
///_|> descry : this function bound signals with their handler functions
///_|> returning: this function doesn't return anything

#endif // SIGNALSPROCESSORS_H