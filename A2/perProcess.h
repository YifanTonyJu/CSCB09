#ifndef PERPROCESS_H
#define PERPROCESS_H

#include "getInfo.h"

#define PERPROCESS_TABLE_HEADER "\t PID\tFD\n\t===========\n"
#define PERPROCESS_TABLE_INFOROW_ROWINDEX "%d"
#define PERPROCESS_TABLE_INFOROW_COL1_FOR_COMPLETE_PID "\t %d"
#define PERPROCESS_TABLE_INFOROW_COL1_FOR_SINGLE_PID "\t %d"
#define PERPROCESS_TABLE_INFOROW_COL2 "\t%d\n"
#define PERPROCESS_TABLE_TAIL "\t===========\n\n"

void print_processFD_table(ProcessInfo* complete_pids_arr, pid_t pid);
///_|> descry: this function print the process FD table based on the information stored in the ProcessInfo array and the PID
///_|> complete_pids_arr: the ProcessInfo pointer to the array that store the complete information, type ProcessInfo*
///_|> pid: indicate the PID of the process, means need all process if pid == -1, type pid_t
///_|> returning: this function does not return anything

#endif // PERPROCESS_H