#ifndef VNODES_H
#define VNODES_H

#include "getInfo.h"

#define VNODES_TABLE_HEADER "\t   FD\t\tInode\n\t========================================\n"
#define VNODES_TABLE_INFOROW_ROWINDEX "%d"
#define VNODES_TABLE_INFOROW_COL1_FOR_COMPLETE_PID "\t   %d"
#define VNODES_TABLE_INFOROW_COL1_FOR_SINGLE_PID "\t   %d"
#define VNODES_TABLE_INFOROW_COL2 "\t\t%lu\n"
#define VNODES_TABLE_TAIL "\t========================================\n\n"

void print_VnodesFD_table(ProcessInfo* complete_pids_arr, pid_t pid);
///_|> descry: this function print the Vnode FD table based on the information stored in the ProcessInfo array and the PID
///_|> complete_pids_arr: the ProcessInfo pointer to the array that store the complete information, type ProcessInfo*
///_|> pid: indicate the PID of the process, means need all process if pid == -1, type pid_t
///_|> returning: this function does not return anything

#endif // VNODES_H