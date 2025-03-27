#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include "getInfo.h"

#define COMPOSITE_TABLE_HEADER_FOR_TXT "\t PID\tFD\tFilename\t\tInode\n\t==============================================\n"
#define COMPOSITE_TABLE_INFOROW_ROWINDEX "%d"
#define COMPOSITE_TABLE_INFOROW_INFOCOL1_FOR_TXT_FOR_COMPLETE_PID "\t %d"
#define COMPOSITE_TABLE_INFOROW_INFOCOL1_FOR_TXT_FOR_SINGLE_PID "\t %d"
#define COMPOSITE_TABLE_INFOROW_INFOCOL2_FOR_TXT "\t%d"
#define COMPOSITE_TABLE_INFOROW_INFOCOL3_FOR_TXT "\t%s"
#define COMPOSITE_TABLE_INFOROW_INFOCOL4_FOR_TXT "\t\t%lu\n"
#define COMPOSITE_TABLE_TAIL_FOR_TXT "\t ========================================\n\n"

typedef struct composite_per_row_pid {
    pid_t pid; // the PID
    int fd; // the FD
    char filename[MAX_FILENAME_LEN]; // filename
    unsigned long inode; // the file inode
} CompositeRowPid; // define for single pid table in .bin

typedef struct composite_per_row {
    int row_index; // the row index
    pid_t pid; // the PID
    int fd; // the FD
    char filename[MAX_FILENAME_LEN]; // filename
    unsigned long inode; // the file inode
} CompositeRow; // define for complete pid table in .bin

void save_to_txt(const char* filename, ProcessInfo* complete_pids_arr, pid_t pid);
///_|> descry: the function save the composed table into .txt format
///_|> filename: a pointer to the string that store the .txt filename, type const char*
///_|> complete_pids_arr: a pointer to the array that store PIDs and related FD information, type ProcessInfo*
///_|> pid: indicated the process PID, means no specific one if is -1, type pid_t
///_|> returning: this function does not return anything


void save_to_bin(const char* filename, ProcessInfo* complete_pids_arr, pid_t pid);
///_|> descry: the function save the composed table into .bin format
///_|> filename: a pointer to the string that store the .bin filename, type const char*
///_|> complete_pids_arr: a pointer to the array that store PIDs and related FD information, type ProcessInfo*
///_|> pid: indicated the process PID, means no specific one if is -1, type pid_t
///_|> returning: this function does not return anything

#endif // OUTPUTHANDLER_H