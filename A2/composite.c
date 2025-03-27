#include "composite.h"

void print_composed_table(ProcessInfo* complete_pids_arr, pid_t pid) {
    //consider the case that we have a PID
    if (pid >= 1) {
        int pid_index = search_pid(complete_pids_arr, pid); //check the pid in the array
        if (pid_index == -1) {
            //can't find the wanted pid
            exit(1);
        }
        printf(COMPOSITE_TABLE_HEADER);
        ProcessInfo pidInfo = complete_pids_arr[pid_index];

        //print info
        FDInfoNode *current = pidInfo.fdInfo_list; // current maybe NULL, so check it first in while loop

        if (current == NULL) {
            printf("Sorry, the /proc/%d/fd/ directory can't be accessed, try another PID.\n", pid);
        }
        while(current != NULL) {
            printf(COMPOSITE_TABLE_INFOROW_INFOCOL1_FOR_SINGLE_PID, current->pid_ptr->pid);
            printf(COMPOSITE_TABLE_INFOROW_INFOCOL2, current->fd);
            printf(COMPOSITE_TABLE_INFOROW_INFOCOL3, current->filename);
            printf(COMPOSITE_TABLE_INFOROW_INFOCOL4, current->inode);
            current = current->next;
        }
    }
    else {
        printf(COMPOSITE_TABLE_HEADER);
        int pid_index;
        int table_row_index = 0;
        for(pid_index = 0; pid_index < complete_pids_arr->pid_num; ++pid_index) {
            ProcessInfo pidInfo = complete_pids_arr[pid_index];

            //print info
            FDInfoNode *current = pidInfo.fdInfo_list; // current maybe NULL, so check it first in while loop

            while(current != NULL) {
                printf(COMPOSITE_TABLE_INFOROW_ROWINDEX, table_row_index);
                printf(COMPOSITE_TABLE_INFOROW_INFOCOL1_FOR_COMPLETE_PID, current->pid_ptr->pid);
                printf(COMPOSITE_TABLE_INFOROW_INFOCOL2, current->fd);
                printf(COMPOSITE_TABLE_INFOROW_INFOCOL3, current->filename);
                printf(COMPOSITE_TABLE_INFOROW_INFOCOL4, current->inode);
                table_row_index += 1;
                current = current->next;
            }
        }
    }

    printf(COMPOSITE_TABLE_TAIL);
}