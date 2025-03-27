#include "threshold.h"

void print_threshold_process(ProcessInfo* complete_pids_arr, int threshold) {
    printf(THRESHOLD_INFO_HEADER);

    //visit each element and print wanted info
    for(int pid_index = 0; pid_index < complete_pids_arr->pid_num; ++pid_index) {
        ProcessInfo pidInfo = complete_pids_arr[pid_index];
        if (pidInfo.fd_num > threshold) {
            printf(THRESHOLD_INFO, pidInfo.pid, pidInfo.fd_num);
        }
    }
    printf("\n\n");
}