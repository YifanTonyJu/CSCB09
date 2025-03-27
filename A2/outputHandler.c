#include "outputHandler.h"

void save_to_txt(const char* filename, ProcessInfo* complete_pids_arr, pid_t pid) {
    //open or create the file with filename 'filename' to write
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Fail to create or open the file 'filename'!");
        exit(1);
    }

    //consider the case that we have a real PID
    if (pid >= 1) {
        int pid_index = search_pid(complete_pids_arr, pid);
        if (pid_index == -1) {
            //can't find the wanted pid
            exit(1);
        }
        fprintf(file, COMPOSITE_TABLE_HEADER_FOR_TXT);
        ProcessInfo pidInfo = complete_pids_arr[pid_index];

        //print info
        FDInfoNode *current = pidInfo.fdInfo_list; // current maybe NULL, so check it first in while loop

        while(current != NULL) {
            fprintf(file, COMPOSITE_TABLE_INFOROW_INFOCOL1_FOR_TXT_FOR_SINGLE_PID, current->pid_ptr->pid);
            fprintf(file, COMPOSITE_TABLE_INFOROW_INFOCOL2_FOR_TXT, current->fd);
            fprintf(file, COMPOSITE_TABLE_INFOROW_INFOCOL3_FOR_TXT, current->filename);
            fprintf(file, COMPOSITE_TABLE_INFOROW_INFOCOL4_FOR_TXT, current->inode);
            current = current->next;
        }
    }
    else {
        fprintf(file, COMPOSITE_TABLE_HEADER_FOR_TXT);
        int pid_index;
        for(pid_index = 0; pid_index < complete_pids_arr->pid_num; ++pid_index) {
            ProcessInfo pidInfo = complete_pids_arr[pid_index];

            //print info
            int table_row_index = 0;
            FDInfoNode *current = pidInfo.fdInfo_list; // current maybe NULL, so check it first in while loop

            while(current != NULL) {
                fprintf(file, COMPOSITE_TABLE_INFOROW_ROWINDEX, table_row_index);
                fprintf(file, COMPOSITE_TABLE_INFOROW_INFOCOL1_FOR_TXT_FOR_COMPLETE_PID, current->pid_ptr->pid);
                fprintf(file, COMPOSITE_TABLE_INFOROW_INFOCOL2_FOR_TXT, current->fd);
                fprintf(file, COMPOSITE_TABLE_INFOROW_INFOCOL3_FOR_TXT, current->filename);
                fprintf(file, COMPOSITE_TABLE_INFOROW_INFOCOL4_FOR_TXT, current->inode);
                table_row_index += 1;
                current = current->next;
            }
        }
    }

    fprintf(file, COMPOSITE_TABLE_TAIL_FOR_TXT);
    fclose(file);
    printf("Successfully save the composed table in %s!\n", filename);
}

void save_to_bin(const char* filename, ProcessInfo* complete_pids_arr, pid_t pid) {
    //open or create the file with filename 'filename' to write
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Fail to create or open the file 'filename'!");
        exit(1);
    }

    //just print important PID and file info for faster speed and less memory usage
    CompositeRowPid line_pid; // declare a variable to store the single pid information to be written
    CompositeRow line; // declare a variable to store the complete pid information to be written

    //consider the case that we have a PID
    if (pid >= 1) {
        int pid_index = search_pid(complete_pids_arr, pid);
        if (pid_index == -1) {
            //can't find the wanted pid
            exit(1);
        }
        ProcessInfo pidInfo = complete_pids_arr[pid_index];

        //print info
        FDInfoNode *current = pidInfo.fdInfo_list; // current maybe NULL, so check it first in while loop

        while (current != NULL) {
            //just write each row as a CompositeRow data
            line_pid.pid = pid;
            line_pid.fd = current->fd;
            strncpy(line_pid.filename, current->filename, MAX_FILENAME_LEN);
            line_pid.inode = current->inode;

            fwrite(&line_pid, sizeof(CompositeRowPid), 1, file);
            //next row
            current = current->next;
        }
    }
    else {
        int pid_index;
        int table_row_index = 0;

        for(pid_index = 0; pid_index < complete_pids_arr->pid_num; ++pid_index) {
            ProcessInfo pidInfo = complete_pids_arr[pid_index];

            //print info
            FDInfoNode *current = pidInfo.fdInfo_list; // current maybe NULL, so check it first in while loop

            while(current != NULL) {
                //just write each row as a CompositeRow data
                line.row_index = table_row_index;
                line.pid = pid;
                line.fd = current->fd;
                strncpy(line.filename, current->filename, MAX_FILENAME_LEN);
                line.inode = current->inode;

                fwrite(&line, sizeof(CompositeRow), 1, file);
                table_row_index += 1;
                current = current->next;
            }
        }
    }

    fclose(file);
    printf("Successfully save the composed table in %s!\n", filename);
}