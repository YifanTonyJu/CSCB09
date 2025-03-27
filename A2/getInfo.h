#ifndef GETINFO_H
#define GETINFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>


#define MAX_FDS_NUM 1024 // most processes has fd limitation to be 1024
#define MAX_FILENAME_LEN 256
#define MAX_PATH_LEN 256
#define MAX_FILE_LINE_LEN 256
#define PROC "/proc"
#define PID_DIC_PATH "/proc/%d"
#define FD_DIC_PATH "/proc/%d/fd/"
#define FD_LINK_FILE_PATH "/proc/%d/fd/%d"
#define FD_INFO_FILE_PATH "/proc/%d/fdinfo/%d"

struct FDInfoNode; // declare to prevent running into loop
struct ProcessInfo; // declare to prevent running into loop

typedef struct ProcessInfo {
    pid_t pid; // the PID
    int fd_num; // the number of FD of 'pid' process
    int pid_num; // the number of the whole processes, only display in the first element of the array
    struct FDInfoNode* fdInfo_list; // the pointer to the head of FDInfoNode linked list
} ProcessInfo;

typedef struct FDInfoNode {
    int fd; //FD of an open file
    unsigned long inode; // the inode of the open file
    char filename[MAX_FILENAME_LEN]; //the filename of the open file
    ProcessInfo* pid_ptr; // the pointer to the ProcessInfo that store the process info
    struct FDInfoNode* next; // the pointer to the next node
} FDInfoNode;

void get_filename_inode(pid_t pid, int fd, char* filename, unsigned long* inode);
///_|> descry: the function get filename and inode of the file with FD 'fd' in 'pid' process
///_|> pid: indicate the PID of a process, type pid_t
///_|> fd: indicate the FD of of a open file, type int
///_|> filename: the pointer to the string that is going to store filename, type char*
///_|> inode: the pointer to the memory that stores inode, type unsigned long*
///_|> returning: this function does not return anything

FDInfoNode* create_FDInfo_node(int fd, unsigned long inode, char* filename, ProcessInfo* pid_pt);
///_|> descry: the function create a FDInfoNode by given FD, inode, filename and PID
///_|> fd: indicate the FD of of a open file, type int
///_|> inode: indicate the inode of of a open file, type unsigned long
///_|> filename: the pointer to a string stores filename, type char*
///_|> pid_pt: the pointer to a memory that stores ProcessInfo data
///_|> returning: the function return a pointer to a newly created FDInfoNode data, type FDInfoNode*


FDInfoNode* get_process_fds(ProcessInfo* pid_ptr);
///_|> descry: the function get all the FDs with the process PID in the memory pointed by 'pid_ptr' and store as a linked list
///_|> pid_ptr: the pointer to the memory that store a ProcessInfo data
///_|> returning: the function return the head pointer of the FDInfoNode linked list

void free_fd_list(FDInfoNode* head);
///_|> descry: the function free the FDInfoNode linked list
///_|> head: the pointer to the head of the linked list
///_|> returning: this function does not return anything

bool can_access_process(pid_t pid);
///_|> descry: the function can tell whether the process can be accessed by given the PID
///_|> pid: indicate a process PID, type pid_t
///_|> returning: this function return whether the process can be accessed, return true if yes, otherwise return false, type bool

ProcessInfo* get_all_pids();
///_|> descry: the function can get all the PIDs of the running processes, and store them in an array
///_|> returning: a pointer to the array that store PIDs and other informations, type ProcessInfo*

int search_pid (ProcessInfo* pids, pid_t pid);
///_|> descry: the function can search PID in a ProcessInfo array
///_|> pids: the pointer to the ProcessInfo array
///_|> pid: indicate the PID to be searched
///_|> returning: return the index of wanted pid if find, otherwise return -1, type int

void free_pid_array(ProcessInfo* pids);
///_|> descry: the function free the ProcessInfo array
///_|> pids: the pointer to the array
///_|> returning: this function does not return anything

#endif // GETINFO_H