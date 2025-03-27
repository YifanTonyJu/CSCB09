#ifndef SUMMARY_H
#define SUMMARY_H

#include "getInfo.h"

#define SUMMARY_TABLE_HEADER "\t Summary Table\n\t =============\n"
#define SUMMARY_TABLE_INFO "%d (%d),  "

void print_summary_table(ProcessInfo* complete_pids_arr);
///_|> descry: this function print the summary table based on the information stored in the ProcessInfo array
///_|> complete_pids_arr: the ProcessInfo pointer to the array that store the complete information, type ProcessInfo*
///_|> returning: this function does not return anything

#endif // SUMMARY_H