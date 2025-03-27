#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "getInfo.h"

#define THRESHOLD_INFO_HEADER "## Offending processes:\n"
#define THRESHOLD_INFO "%d (%d), "

void print_threshold_process(ProcessInfo* complete_pids_arr, int threshold);
///_|> descry: this function print the threshold table based on the information stored in the ProcessInfo array
///_|> complete_pids_arr: the Process/Info pointer to the array that store the complete information, type ProcessInfo*
///_|> threshold: indicate the limitation of the FD number of the process not to be printed, type int
///_|> returning: this function does not return anything

#endif // THRESHOLD_H