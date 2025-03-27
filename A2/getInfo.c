#include "getInfo.h"

void get_filename_inode(pid_t pid, int fd, char* filename, unsigned long* inode) {
    if (filename == NULL || inode == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to filename!\n");
        exit(1);
    }

    //get fd link file path and fdinfo file path
    char fd_path[MAX_PATH_LEN];
    char fdinfo_path[MAX_PATH_LEN];
    snprintf(fd_path, sizeof(fd_path), FD_LINK_FILE_PATH, pid, fd);
    snprintf(fdinfo_path, sizeof(fdinfo_path), FD_INFO_FILE_PATH, pid, fd);

    // get filename
    ssize_t len = readlink(fd_path, filename, MAX_FILENAME_LEN - 1); // get "(deleted)" to indicate that the file has been deleted
    if (len < 0) {
        filename[0] = '\0'; // mark as a blank name and tell get_process_fds() to skip it
        *inode = -1; // mark did not find the file and tell get_process_fds() to skip it
        return;
    }
    filename[len] = '\0'; // make sure it is a string

    // get inode
    struct stat file_info;
    if (stat(fd_path, &file_info) == 0) {
        *inode = file_info.st_ino;
        return;
    }
    // fail to get inode, the file may be not exist
    *inode = -1; // tell get_process_fds() to skip it

}

FDInfoNode* create_FDInfo_node(int fd, unsigned long inode, char* filename, ProcessInfo* pid_ptr) {
    // malloc the memory
    FDInfoNode* new_node = malloc(sizeof(FDInfoNode));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for FDInfoNode node!\n");
        exit(1);
    }

    //fill the node's field
    new_node->fd = fd;
    new_node->inode = inode;
    strcpy(new_node->filename, filename);
    new_node->pid_ptr = pid_ptr;
    new_node->next = NULL;

    return new_node;
}

FDInfoNode* get_process_fds(ProcessInfo* pid_ptr) {
    //get the fd directory path
    char fd_dic_path[MAX_PATH_LEN];
    snprintf(fd_dic_path, sizeof(fd_dic_path), FD_DIC_PATH, pid_ptr->pid);

    //open the fd directory
    DIR* dir = opendir(fd_dic_path);
    if (dir == NULL) {
        return NULL; // fail to access FD info, just tell get_all_pids() to skip it
    }

    struct dirent *line; // a pointer to one line of the directory
    FDInfoNode *head = NULL;
    FDInfoNode *current = NULL;

    //read the first line to get the head pointer of the linked list
    line = readdir(dir); //use opendir + readdir to achieve real_time

    while (line != NULL) {
        if (isdigit(line->d_name[0])) {
            int fd = strtol(line->d_name, NULL, 10); //get the fd from the read line

            //use the fd to get corresponding inode and filename
            unsigned long inode;
            char filename[MAX_FILENAME_LEN];


            //get inode and filename by the paths
            //first get filename and then inode to maintain consistency.
            //if delete a hard link, inode may still have a file to connect, while filename has been deleted.
            //so get filename name first to ensure that we can know that the file has been deleted and still get the consistent inode of that file.
            get_filename_inode(pid_ptr->pid, fd, filename, &inode);

            //check whether the fds number have exceeded the MAX_FDS_NUM
            if (pid_ptr->fd_num == MAX_FDS_NUM) {
                closedir(dir);
                return head;
            }

            //make sure we have legal filename and inode
            if (filename[0] != '\0' && inode > 0) {
                //create a FDInfoNode node with the info we have and add to the linked list
                FDInfoNode *new_node = create_FDInfo_node(fd, inode, filename, pid_ptr);
                if (head == NULL) {
                    head = new_node;
                    current = head;
                }
                else {
                    current->next = new_node;
                    current = new_node;
                }
                pid_ptr->fd_num += 1;
            }
        }
        line = readdir(dir); //read next line
    }

    closedir(dir); //finish reading the directory file and close the file.
    return head;
}

void free_fd_list(FDInfoNode* head) {
    FDInfoNode *successor = head;
    while (successor != NULL) {
        FDInfoNode *current = successor;
        successor = successor->next;
        free(current);
    }
}

bool can_access_process(pid_t pid) {
    char proc_path[256];
    struct stat info; //define a variable to store file info, want to get the UID

    //build the path to the directory of process 'pid'
    snprintf(proc_path, sizeof(proc_path), PID_DIC_PATH, pid);

    // get the file info
    if (stat(proc_path, &info) == 0) {
        if (info.st_uid == getuid()) {
            return true; // can access the process
        }
        return false;
    }
    return false; //fail to access to the process
}

ProcessInfo* get_all_pids() {
    //open the directory /proc
    DIR *dir = opendir(PROC);
    if(dir == NULL) {
        perror("Error: Fail to open /proc!\n");
        exit(1);
    }
    
    struct dirent *line; // a pointer to one line of the directory
    int count = 0;// the number of processes

    //read each line and get the process numbers
    line = readdir(dir);
    while (line != NULL) {
        if (isdigit(line->d_name[0])) {
            count += 1;
        }
        line = readdir(dir);
    }

    //move the cursor to the top
    rewinddir(dir);

    //get total number of process to declare an array
    if (count == 0) {
        return NULL; // no PIDs
    }
    if (count < 0) {
        fprintf(stderr, "Error: Have negative number of process!\n");
        exit(1);
    }

    //assign memory to store information
    ProcessInfo *process_arr = malloc(count * sizeof(ProcessInfo));
    if (process_arr == NULL) {
        perror("Fail to malloc an array!\n");
        exit(1);
    }

    int index;

    //set each fd_num to 0 to represent it is a place to be added
    //set each pid_num to 0 to represent it is a place to be modified
    for(index = 0; index < count; ++index) {
        process_arr[index].fd_num = 0;
        process_arr[index].pid_num = 0; // we need to store the real number of PIDs here
    }

    index = 0;
    //run through '/proc' to load pids into process_arr, and we only want the count number of pids, as processes don't change constantly.
    //we can accept that some boxes in the array isn't filled when the process is deleted.(due to real_time issue)
    //we use opendir + readdir to achieve real_time data
    line = readdir(dir);
    while (line != NULL && index < count) {
        if (isdigit(line->d_name[0])) {
            int current_pid = strtol(line->d_name, NULL, 10);// change d_name to int to get pid
            //ensure the process can be accessed
            if (can_access_process(current_pid)) {
                process_arr[index].pid = current_pid;
                process_arr[index].fdInfo_list = get_process_fds(&(process_arr[index])); //use the pid to get fds
                index += 1;
            }
        }
        line = readdir(dir); // read next line
    }
    process_arr[0].pid_num = index;//display the pid num in the first element

    closedir(dir);
    return process_arr;
}

int search_pid (ProcessInfo* pids, pid_t pid) {
    if (pids == NULL) {
        fprintf(stderr, "Error: Fail to find the array to be searched!\n");
        exit(1);
    }

    //search in the given array
    int index;
    for(index = 0; index < pids->pid_num; ++index) {
        if (pids[index].pid == pid) {
            return index;
        }
    }
    fprintf(stderr, "Error: Fail to find the pid, use exist one!\n");
    return -1;
}

void free_pid_array(ProcessInfo* pids) {
    if (pids == NULL) {
        return;
    }
    free(pids);
}