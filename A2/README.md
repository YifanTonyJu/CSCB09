# Recreating the System-Wide FD Tables

## 1.Metadata
**Author**: Yifan Ju

**Date**: 2025.3.8

## 2.Introduction
This project is to design a C program to print PIDs of each process, the related FD of each process and the file information includes filename and file inode number of the open files on the screen.This program can also save the composed table that contains all the information into a `.txt` or `.bin` based on user's CLA.

## 3.Approach Design
The main points of this project are to get the **real-time** information and maintain the **consistency** of the information presented. So in the first part, when I acquiring information of processes and FDs, I pay much attention to achieve these two points. In the second part, I will store the information I get for later use. At last, I will display or save the information based on the users' CLA.
## 4. Implementation
### I. Code Description
In order to acquire **real-time** information, I uses `opendir()` and `readdir()` functions in library `dirent.h` to always read **real-time** directory file(files are changing during each running processes).

For the consistency, I design the function to **immediately** get all the related information of an FD. Also, I will get all the fds once I access to a process (this also help me get **real-time information**). The order of get filename and inode of a open file is also an important factor for consistency maintaining, and I have explained this with comments in my code.

I choose **array** to store CDT `ProcessInfo` data that contains the PID, as array is easy for searching. I use **linked list** to store CDT `FDInfoNode` data that contains the filename, inode and FD of an open file. It is because the FDs in directory file "/proc/PID/fd/" are always changing, which cause the number of FDs not stable.

I also define a function to handle the CLAs so that the program can print or save the information based on CLA easily.

Each CLA has a source file that contains print or save functions to print or save the wanted table. I define some Macros for the table content in the corresponding head files, which can be easily modify if user want to change the table format later.

---

### II. Modules
#### Module 1: argumentHandler
##### Functions:
`void handle_arguments(int argc, char **argv, Arguments *args);`

#### Module 2: composite
##### Functions:
`void print_composed_table(ProcessInfo* complete_pids_arr, pid_t pid);`

#### Module 3: getInfo
##### Functions:
`void get_filename_inode(pid_t pid, int fd, char* filename, unsigned long* inode);`

`FDInfoNode* create_FDInfo_node(int fd, unsigned long inode, char* filename, ProcessInfo* pid_pt);`

`FDInfoNode* get_process_fds(ProcessInfo* pid_ptr);`

`void free_fd_list(FDInfoNode* head);`

`bool can_access_process(pid_t pid);`

`ProcessInfo* get_all_pids();`

`int search_pid(ProcessInfo* pids, pid_t pid);`

`void free_pid_array(ProcessInfo* pids);`

#### Module 4: outputHandler
##### Functions:
`void save_to_txt(const char* filename, ProcessInfo* complete_pids_arr, pid_t pid);`

`void save_to_bin(const char* filename, ProcessInfo* complete_pids_arr, pid_t pid);`

#### Module 5: perProcess
##### Functions:
`void print_processFD_table(ProcessInfo* complete_pids_arr, pid_t pid);`

#### Module 6: summary
##### Functions:
`void print_summary_table(ProcessInfo* complete_pids_arr);`

#### Module 7: systemWide
##### Functions:
`void print_systemwideFD_table(ProcessInfo* complete_pids_arr, pid_t pid);`

#### Module 8: threshold
##### Functions:
`void print_threshold_process(ProcessInfo* complete_pids_arr, int threshold);`

#### Module 9: vnodes
##### Functions:
`void print_VnodesFD_table(ProcessInfo* complete_pids_arr, pid_t pid);`

---

### III. Functions Description
#### Function: `handle_arguments`

`handle_arguments` handles CLA and stores the corresponding information in an `Arguments` struct. It identifies different CLAs, stores values to the struct fields, and ensures valid input. **If no commend line argument are given, it defaults to display the composite table.**

Uses **libraries**: `stdio.h` for error messages printing, `stdlib.h` for `exit()` and `strtol()`, `string.h` for argument comparison, and `ctype.h` for `bool` type.

Uses **system calls**: `exit()` to terminate the program.

#### Function: `print_composed_table`

`print_composed_table` prints a composed table containing all FD related information for either a specific process (`pid >= 1`) or all processes (`pid == -1`) and the PID. It reads data from the `ProcessInfo` array, visits through the linked list of `FDInfoNode`, and formats the output accordingly.

Uses **libraries**: `stdio.h` for `printf()`, `stdlib.h` for `exit()`.

Uses **system calls**: None

#### Function: `get_filename_inode`

`get_filename_inode` finds the filename and inode of a file associated with a given FD (`fd`) in a specific process (`pid`). It builds file paths for `/proc/<pid>/fd/<fd>` and `/proc/<pid>/fdinfo/<fd>` with `snprintf()`, then uses `readlink()` to obtain the filename and `stat()` to get the inode.

Uses **libraries**: `stdio.h` for `fprintf()`, `stdlib.h` for `exit()`, `string.h` for `snprintf()`, `unistd.h` for `readlink()`, and `sys/stat.h` for `stat()`.

Uses **system calls**: `readlink()` to read links files in `/proc/<pid>/fd/`, and `stat()` to get file data.

#### Function: `create_FDInfo_node`

`create_FDInfo_node` dynamically allocates and creates an `FDInfoNode` data, storing FD, inode, filename, and corresponding process.

Uses **libraries**: `stdio.h` for error printing, `stdlib.h` for `malloc()` and `exit()`, `string.h` for `strcpy()`.

Uses **system calls**: None.

#### Function: `get_process_fds`

`get_process_fds` gets all FDs for a given process by reading the `/proc/<pid>/fd/` directory. It reads through directory lines, gets FDs, and **immediately** gets filename and inode information before storing them into a linked list.

Uses **libraries**: `stdio.h` for `printf()`, `stdlib.h` for `exit()`, `string.h` for `snprintf()`, `unistd.h` for `strtol()`,`dirent.h` for `opendir()`, `readdir()`, and `closedir()`.

Uses **system calls**: `opendir()` to access the `/proc/<pid>/fd/` directory, `readdir()` to read directory files, `closedir()` to close it.

#### Function: `free_fd_list`

`free_fd_list` releases memory allocated for an `FDInfoNode` linked list, visiting from the head and freeing each node.

Uses **libraries**: `stdlib.h` for `free()`.

Uses **system calls**: None.

#### Function: `can_access_process`

`can_access_process` checks if the current user has permission to access a specific process (`pid`) by checking the `/proc/<pid>` directory and comparing its owner UID with the current user’s UID.

Uses **libraries**: `stdio.h` for error printing, `stdlib.h` for `exit()`, `string.h` for `snprintf()`, `unistd.h` for `getuid()`, `sys/stat.h` for `stat()`.

Uses **system calls**: `stat()` to get file information.

#### Function: `get_all_pids`

`get_all_pids` reads `/proc` to collect PIDs of all running processes. It dynamically allocates a `ProcessInfo` array, find FDs information for each process, and returns the array.

Uses **libraries**: `stdio.h` for `printf()`, `stdlib.h` for `malloc()`, `string.h` for `snprintf()`, `dirent.h` for `opendir()`, `readdir()`, and `closedir()`.
Uses **system calls**: `opendir()` to access `/proc/`, `readdir()` to read directory files, `closedir()` to close it.

#### Function: `search_pid`

`search_pid` searches for a given `pid` in a `ProcessInfo` array and returns its index if found, otherwise returns `-1`.

Uses **libraries**: `stdio.h` for error printing.

Uses **system calls**: None.

#### Function: `free_pid_array`

`free_pid_array` releases memory allocated for a `ProcessInfo` array.

Uses **libraries**: `stdlib.h` for `free()`.

Uses **system calls**: None.

#### Function: `main`

`main` handles command line arguments, read process information, and prints or saves the wanted information. It first combines all process-related data using `get_all_pids()`, then handles arguments with `handle_arguments()`, determining which tables to display or save. It supports multiple output tables, including per-process, systemWide, Vnodes, composite, summary, and threshold filtering. Additionally, it can save the composed table in `.txt` or `.bin` formats.

Uses **libraries**: `stdio.h` for input/output, `stdlib.h` for memory allocation and `exit()`, `string.h` for string operations.

Uses **system calls**: None

#### Function: `save_to_txt`

`save_to_txt` saves the same composed table using `--composite` into a `.txt` file.

Uses **libraries**: `stdio.h` for `fprintf()`, `fopen()`, and `fclose()`, `stdlib.h` for `exit()`, `string.h` for `snprintf()`.

Uses **system calls**: None

#### Function: `save_to_bin`

`save_to_bin` saves the same composed table using `--composite` into a `.bin` file.

Uses **libraries**: `stdio.h` for `fwrite()`, `fopen()`, and `fclose()`, `stdlib.h` for `exit()`, `string.h` for `strncpy()`.

Uses **system calls**: None

#### Function: `print_processFD_table`

`print_processFD_table` prints a table containing FD information for either a specific process (`pid >= 1`) or all processes (`pid == -1`) and PID. It reads data from the `ProcessInfo` array, visits through the linked list of `FDInfoNode`, and formats the output accordingly.

Uses **libraries**: `stdio.h` for `printf()`, `stdlib.h` for `exit()`.

Uses **system calls**: None

#### Function: `print_summary_table`

`print_summary_table` prints a summary table showing the number of FDs for each process. It reads through the `ProcessInfo` array and prints the process ID along with the count of its FDs.

Uses **libraries**: `stdio.h` for `printf()`.

Uses **system calls**: None

#### Function: `print_systemwideFD_table`

`print_systemwideFD_table` prints a table containing PIDs, FDs for a specific process (`pid`) or all processes (`pid == -1`) and the filname. It reads data from the `ProcessInfo` array, visits through the linked list of `FDInfoNode`, and formats the output accordingly.

Uses **libraries**: `stdio.h` for `printf()`, `stdlib.h` for `exit()`.

Uses **system calls**: None

#### Function: `print_threshold_process`

`print_threshold_process` prints a list of processes that FDs count exceed threshold limitation. It reads through the `ProcessInfo` array and prints the process ID along with its open FD count if it exceeds the given limitation.

Uses **libraries**: `stdio.h` for `printf()`.

Uses **system calls**: None

#### Function: `print_VnodesFD_table`

`print_VnodesFD_table` prints a table containing FDs for a specific process (`pid`) or all processes (`pid == -1`) and the related inode. It reads data from the `ProcessInfo` array, visits through the linked list of `FDInfoNode`, and formats the output accordingly.

Uses **libraries**: `stdio.h` for `printf()`, `stdlib.h` for `exit()`.

Uses **system calls**: None

## 5. Project Execution Pseudocode

```pseudocode
begin

    // Step 1: Get all accessible PIDs and their related FDs, inodes, and filenames
    complete_pids_arr = get_all_pids()

    // Step 2: handle command-line arguments and store the argument information into the Arguments structure
    argu_info = handle_arguments(argc, argv)

    // Step 3: Check arguments and execute corresponding functions
    if (argu_info.per_process == TRUE)
        print_processFD_table(complete_pids_arr, argu_info.pid)

    if (argu_info.system_wide == TRUE)
        print_systemwideFD_table(complete_pids_arr, argu_info.pid)

    if (argu_info.vnodes == TRUE)
        print_VnodesFD_table(complete_pids_arr, argu_info.pid)

    if (argu_info.composite == TRUE)
        print_composed_table(complete_pids_arr, argu_info.pid)

    if (argu_info.summary == TRUE)
        print_summary_table(complete_pids_arr)

    if (argu_info.threshold != -1)
        print_threshold_process(complete_pids_arr, argu_info.threshold)

    if (argu_info.output_txt == TRUE)
        save_to_txt("compositeTable.txt", complete_pids_arr, argu_info.pid)

    if (argu_info.output_binary == TRUE)
        save_to_bin("compositeTable.bin", complete_pids_arr, argu_info.pid)

    // Step 4: Default behavior - If no argument is provided, print the composite table
    if (no argument is given)
        print_composed_table(complete_pids_arr, argu_info.pid)
    if (have specific pid and no other table related argument is given)
        print_processFD_table(complete_pids_arr, argu_info.pid)
        print_systemwideFD_table(complete_pids_arr, argu_info.pid)
        print_VnodesFD_table(complete_pids_arr, argu_info.pid)
        print_composed_table(complete_pids_arr, argu_info.pid)

    // Step 5: Free all allocated memory
    for (i = 0 to complete_pids_arr.pid_num - 1)
        free_fd_list(complete_pids_arr[i].fdInfo_list)
    free_pid_array(complete_pids_arr)

end
```
## 6. How to Complie the Code

1.`make`: complie the whole project

2.`make clean`: clean all the .o file and the executable

3.`make help`: get help information of the makefile

**Note**: In order to let computer know `readlink()`, I use `gcc -Wall -std=c11 -D_POSIX_C_SOURCE=200809L` to complie the project!

## 7.Expected Results
If using a mistaken CLA, print "Error: Please use correct commend line arguments!"

Using one of or several of the CLAs: `--per-process`, `--systemWide`, `--Vnodes`, `--composite`, `--summary`, `threshold=20` will display the corresponding tables on the screen.

Using one of or all the CLAs: `--output_TXT` and `--output.binary` will save the composed table in `.txt` or `.bin` file

If no arguments, then it will display composed table.

If type a number indicate PID, like `1234`,it will have several cases:

If no `--per-process`, `--systemWide`, `--Vnodes`, `--composite`, `--output_TXT` and `--output.binary`arguments, it will print all the tables corrsponding the specific PID and maybe summary table or threshold table if wanted.

If have one of or several of `--per-process`, `--systemWide`, `--Vnodes`, `--composite`, `--output_TXT` and `--output.binary` arguments, it will print wanted tables with specific PID or save the composed table with specific PID as `.txt` or `.bin` file based on the CLAs.

If can't find the process with specific pid, print error "Error: Fail to find the array to be searched!"

If the PID is negative, it print error "Error: Invalid PID: -1234, need to be positive!"

**Some samples can be found in "sample" directory!**

## 8.Test Cases

1.Cases that using singled or several CLA, with a specific PID or none

2.Case that have no arguments

3.Case that only have a number

4.Case that using wrong CLA, then it will print error message(unexpected).

5.Cases that using non-exist PID or negative PID, then it will print error message based on different cases(unexpected).

## 9. Disclaimers
THE PROJECT WORKS ONLY WHEN:

1. The filename length not exceed 256
2. The path name not exceed 256
3. The file line length not exceed 256
4. THe FDs of each process not exceed 1024

## 10. Reference
1. https://www.geeksforgeeks.org/what-is-pseudocode-a-complete-tutorial/
2. https://q.utoronto.ca/courses/372744/assignments/1450546?module_item_id=6426714
3. https://man7.org/linux/man-pages/man5/proc.5.html
4. https://tldp.org/LDP/Linux-Filesystem-Hierarchy/html/proc.html
5. https://man7.org/linux/man-pages/man2/stat.2.html
6. https://man7.org/linux/man-pages/man2/readlink.2.html
7. https://man7.org/linux/man-pages/man3/opendir.3.html
8. https://man7.org/linux/man-pages/man3/readdir.3.html
9. https://man7.org/linux/man-pages/man3/closedir.3.html
10. https://man7.org/linux/man-pages/man3/sscanf.3.html
11. https://man7.org/linux/man-pages/man3/snprintf.3p.html
12. https://man7.org/linux/man-pages/man2/geteuid.2.html
13. https://man7.org/linux/man-pages/man3/rewinddir.3.html
14. https://piazza.com/class/m5j5smmmxyx2s0/post/185
