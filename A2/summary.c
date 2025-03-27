#include "summary.h"

void print_summary_table(ProcessInfo* complete_pids_arr) {
    printf(SUMMARY_TABLE_HEADER);

    //visit each element and print wanted info
    for(int pid_index = 0; pid_index < complete_pids_arr->pid_num; ++pid_index) {
        ProcessInfo pidInfo = complete_pids_arr[pid_index];
        if (pidInfo.fd_num > 0) {
            printf(SUMMARY_TABLE_INFO, pidInfo.pid, pidInfo.fd_num);
        }
    }
    printf("\n\n");
}