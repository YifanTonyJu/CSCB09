#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "getInfo.h"

#define COMPOSITE_TABLE_HEADER "\t PID\tFD\tFilename\t\tInode\n\t==============================================\n"
#define COMPOSITE_TABLE_INFOROW_ROWINDEX "%d"
#define COMPOSITE_TABLE_INFOROW_INFOCOL1_FOR_COMPLETE_PID "\t %d"
#define COMPOSITE_TABLE_INFOROW_INFOCOL1_FOR_SINGLE_PID "\t %d"
#define COMPOSITE_TABLE_INFOROW_INFOCOL2 "\t%d"
#define COMPOSITE_TABLE_INFOROW_INFOCOL3 "\t%s"
#define COMPOSITE_TABLE_INFOROW_INFOCOL4 "\t\t%lu\n"
#define COMPOSITE_TABLE_TAIL "\t ========================================\n\n"

void print_composed_table(ProcessInfo* complete_pids_arr, pid_t pid);
///_|> descry: this function print the composed table based on the information stored in the ProcessInfo array and the PID
///_|> complete_pids_arr: the ProcessInfo pointer to the array that store the complete information, type ProcessInfo*
///_|> pid: indicate the PID of the process, means need all process if pid == -1, type pid_t
///_|> returning: this function does not return anything

#endif // VNODES_H