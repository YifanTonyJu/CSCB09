#include "argumentHandler.h"
#include "composite.h"
#include "getInfo.h"
#include "outputHandler.h"
#include "perProcess.h"
#include "summary.h"
#include "systemWide.h"
#include "threshold.h"
#include "Vnodes.h"

int main (int argc, char **argv) {
///_|> descry: the function process the commend line argument and print wanted information
///_|> argc: indicate the number of commend line arguments plus 1, type int
///_|> argv: the pointer to an array of pointers that point at the first element of each commend line argument strings, type char**
///_|> returning: this function return whether the main function works

    ProcessInfo* complete_pids_arr = get_all_pids(); //get all the required information

    Arguments argu_info;
    handle_arguments(argc, argv, &argu_info);
    if (argu_info.per_process) {
        print_processFD_table(complete_pids_arr, argu_info.pid);
    }
    if (argu_info.system_wide) {
        print_systemwideFD_table(complete_pids_arr, argu_info.pid);
    }
    if (argu_info.vnodes) {
        print_VnodesFD_table(complete_pids_arr, argu_info.pid);
    }
    if (argu_info.composite) {
        print_composed_table(complete_pids_arr,argu_info.pid);
    }
    if (argu_info.summary) {
        print_summary_table(complete_pids_arr);
    }
    if (argu_info.threshold != -1) {
        print_threshold_process(complete_pids_arr, argu_info.threshold);
    }
    if (argu_info.output_txt) {
        save_to_txt("compositeTable.txt", complete_pids_arr, argu_info.pid);
    }
    if (argu_info.output_binary) {
        save_to_bin ("compositeTable.bin", complete_pids_arr, argu_info.pid);
    }

    //free all the memory
    for(int i = 0; i < complete_pids_arr->pid_num; ++i) {
        free_fd_list(complete_pids_arr[i].fdInfo_list);
    }
    free_pid_array(complete_pids_arr);

    return 0;
}