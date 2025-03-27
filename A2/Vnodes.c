#include "Vnodes.h"

void print_VnodesFD_table(ProcessInfo* complete_pids_arr, pid_t pid) {
    //consider the case that we have a real PID
    if (pid >= 1) {
        int pid_index = search_pid(complete_pids_arr, pid);
        if (pid_index == -1) {
            //can't find the wanted pid
            exit(1);
        }
        printf(VNODES_TABLE_HEADER);
        ProcessInfo pidInfo = complete_pids_arr[pid_index];

        //print info
        FDInfoNode *current = pidInfo.fdInfo_list; // current maybe NULL, so check it first in while loop

        if (current == NULL) {
            printf("Sorry, the /proc/%d/fd/ directory can't be accessed, try another PID.\n", pid);
        }
        while(current != NULL) {
            printf(VNODES_TABLE_INFOROW_COL1_FOR_SINGLE_PID, current->fd);
            printf(VNODES_TABLE_INFOROW_COL2, current->inode);
            current = current->next;
        }
    }
    else {
        printf(VNODES_TABLE_HEADER);
        int pid_index;
        for(pid_index = 0; pid_index < complete_pids_arr->pid_num; ++pid_index) {
            ProcessInfo pidInfo = complete_pids_arr[pid_index];

            //print info
            int table_row_index = 0;
            FDInfoNode *current = pidInfo.fdInfo_list; // current maybe NULL, so check it first in while loop

            while(current != NULL) {
                printf(VNODES_TABLE_INFOROW_ROWINDEX, table_row_index);
                printf(VNODES_TABLE_INFOROW_COL1_FOR_COMPLETE_PID, current->fd);
                printf(VNODES_TABLE_INFOROW_COL2, current->inode);
                table_row_index += 1;
                current = current->next;
            }
        }
    }

    printf(VNODES_TABLE_TAIL);
}