#ifndef SYSTEMWIDE_H
#define SYSTEMWIDE_H

#include "getInfo.h"

#define SYSTEMWIDE_TABLE_HEADER "\t PID\tFD\tFilename\n\t========================================\n"
#define SYSTEMWIDE_TABLE_INFOROW_ROWINDEX "%d"
#define SYSTEMWIDE_TABLE_INFOROW_INFOCOL1_FOR_COMPLETE_PID "\t %d"
#define SYSTEMWIDE_TABLE_INFOROW_INFOCOL1_FOR_SINGLE_PID "\t %d"
#define SYSTEMWIDE_TABLE_INFOROW_INFOCOL2 "\t%d"
#define SYSTEMWIDE_TABLE_INFOROW_INFOCOL3 "\t%s\n"
#define SYSTEMWIDE_TABLE_TAIL "\t========================================\n\n"

void print_systemwideFD_table(ProcessInfo* complete_pids_arr, pid_t pid);
///_|> descry: this function print the system_wide FD table based on the information stored in the ProcessInfo array and the PID
///_|> complete_pids_arr: the ProcessInfo pointer to the array that store the complete information, type ProcessInfo*
///_|> pid: indicate the PID of the process, means need all process if pid == -1, type pid_t
///_|> returning: this function does not return anything

#endif // SYSTEMWIDE_H