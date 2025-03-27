# System Monitoring Tool -- Concurrency & Signals

## 1.Metadata
**Author**: Yifan Ju

**Date**: 2025.3.26

## 2. Introduction
This project is a simple Linux system monitoring tool. It can show the **memory usage, CPU usage, number of CPU cores and the cores max. frequency** in real-time. It uses C language to get system information by reading special files like ` /proc/stat`, `/proc/cpuinfo` and `/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq` or using library like `sys/sysinfo.h`.

The program uses **pipes and forked child processes** to collect data. Each part of the system (memory, CPU, cores number, max. frequency) is handled by a separate process. The parent process collects the results and prints them on the screen with Escape codes.

This tool can also handle signals like `Ctrl-C` and `Ctrl-Z`. When the user presses `Ctrl-C`, the program will ask if they want to exit or not, while the the program will ignore `Ctrl-Z`.

The program supports **different** command line arguments. The user can choose how many samples to take, how long to wait between samples, and which parts of the system to monitor.

## 3. How I solve the Problem
First, I read the assignment requirements carefully. I saw that I needed to collect memory, CPU, and core information from the system, and display them in real-time. I also needed to **use multiple processes, pipes**, and handle signals like `Ctrl-C`.

I decided to divide the program into small parts. I created separate **.c** and **.h** files for **memory, CPU, cores, command line processing, and signal handling**. This helped me to keep the code clean and easy to manage.

To collect data at the same time, I used `fork()` to create **four child processes**. Each child is responsible for one part of the system: memory, CPU, core count, and max. frequency. I used `pipe()` to let each child process communicate with the parent process.

In the parent process, I used `read()` to get the results from the pipes. Then I used `printf()` and **escape codes** to draw the tables and plots on the screen.

To make sure the program can be stopped safely, I used `signal()` to handle SIGINT (`Ctrl-C`). When the user presses `Ctrl-C`, the program will ask if they want to exit, and kill all child processes if yes. I also ignore the signal SIGTSTP (`Ctrl-Z`) as the program should not be run in the background while running interactively.

I also added **command-line arguments** to let the user choose which information to display and how many samples to collect. I tested many cases to make sure the output is correct.

## 4. Implementation
### 4.i How did I implement my code
I started by writing the code to handle **command line arguments**. This part helps the program understand how many samples to take, the delay between samples, and which system information the user wants to see.

Then I wrote different functions to **get memory usage, CPU usage, and core information**. For example, I used `sys/sysinfo.h` and `/proc/stat` to get memory and CPU data. I also used `/proc/cpuinfo` and other system files to get core number and max. frequency.

After that, I used `fork()` to create child processes. Each child process handles one part of the data collection. I used `pipe()` to send the data from children to the parent process.

In the parent process, I used `read()` to get the data and `printf()` to show the results. I printed the coordinate axes first using simple print functions, and then printed the usage data as plots on the screen.

Finally, I also added **signal handling** so that if the user presses `Ctrl-C`, the program can ask for confirmation and then cleanly kill all the child processes. I also let the program to ignore `Ctrl-Z`.

### 4.ii The modules and functions
#### Main Program
- `main.c`
  - `main()`

#### Command Line Processor
- `CLprocesssor.c / CLprocesssor.h`
  - `commend_arguments(int argc, char **argv, CLArgs *args)`
  - `is_numeric(const char *str)` (helper function)

#### Memory Module
- `memory.c / memory.h`
  - `get_total_memory()`
  - `get_available_memory()`
  - `display_memory_axes(double total_memory, int samples)`
  - `display_memory_plot(double total_memory, double used_memory, int time)`

#### CPU Module
- `cpu.c / cpu.h`
  - `get_cpu_time(CPUtime *cpu_time)`
  - `get_cpu_usage(CPUtime *old_cpu_time, CPUtime *new_cpu_time)`
  - `display_cpu_axes(int samples)`
  - `display_cpu_plot(double cpu_usage, int time, CLArgs *args)`

#### Core Module
- `cores.c / cores.h`
  - `get_cpu_cores()`
  - `get_max_cpu_frequency()`
  - `display_core_graph(int cores_num)`

#### Signal Module
- `signalsprocessor.c / signalsprocessor.h`
  - `pid_mem_register(int pid_mem)`
  - `pid_cpu_register(int pid_cpu)`
  - `pid_core_count_register(int pid_core_count)`
  - `pid_core_freq_register(int pid_core_freq)`
  - `handle_sigint(int signo)`
  - `handle_sigtstp(int signo)`
  - `signal_handlers_binding()`
  
### 4. iii Function Descriptions
#### Function: `is_numeric`

This function returns whether the given string is all digit.

- **Libraries used**: `ctype.h`
- **System calls**: None

---

#### Function: `commend_arguments`

`commend_arguments` handles command line arguments and stores them into a struct called `CLArgs`. It supports both positional arguments (samples, tdelay) and flagged arguments like `--memory`, `--cpu`, and `--cores`. If no option is provided, it will display all modules with default values for samples=20 , tdelay=500000 microsec.

- **Libraries used**: `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`
- **System calls**: None

---

#### Function: `get_total_memory`

This function gets the total system memory using `sysinfo()` and returns the result in GB.

- **Libraries used**: `stdio.h`, `stdlib.h`, `sys/sysinfo.h`
- **System calls**: `sysinfo()`

---

#### Function: `get_available_memory`

This function gets the free system memory using `sysinfo()` and returns the result in GB.

- **Libraries used**: `stdio.h`, `stdlib.h`, `sys/sysinfo.h`
- **System calls**: `sysinfo()`

---

#### Function: `display_memory_axes`

It prints the coordinate axis for the memory table.

- **Libraries used**: `stdio.h`
- **System calls**: None

---

#### Function: `display_memory_plot`

This function calculates the position of the plot based on used memory and sample index, then prints `#`.

- **Libraries used**: `stdio.h`
- **System calls**: None

---

#### Function: `get_cpu_time`

It reads `/proc/stat` to calculate total and active CPU time and stores them into a `CPUtime` struct.

- **Libraries used**: `stdio.h`, `stdlib.h`, `string.h`
- **System calls**: None

---

#### Function: `get_cpu_usage`

This function calculates CPU usage using the formula `(U2 - U1)/(T2 - T1) * 100` based on two `CPUtime` structs.

- **Libraries used**: None
- **System calls**: None

---

#### Function: `display_cpu_axes`

It prints the coordinate axis for the CPU table.

- **Libraries used**: `stdio.h`
- **System calls**: None

---

#### Function: `display_cpu_plot`

Prints the CPU usage value and plots `:` in the corresponding row based on the CPU usage percentage.

- **Libraries used**: `stdio.h`
- **System calls**: None

---

#### Function: `get_cpu_cores`

This function opens `/proc/cpuinfo` and counts how many times "processor" appears to get total core number.

- **Libraries used**: `stdio.h`, `stdlib.h`, `string.h`
- **System calls**: None

---

#### Function: `get_max_cpu_frequency`

It tries to read the max CPU frequency from `/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq`. If not found, it reads from `/proc/cpuinfo` instead.

- **Libraries used**: `stdio.h`, `stdlib.h`, `string.h`
- **System calls**: None

---

#### Function: `display_core_graph`

This function prints small core squares to represent each CPU core in a graph.

- **Libraries used**: `stdio.h`
- **System calls**: None

---

#### Function: `signal_handlers_binding`

This function binds signal handlers to `SIGINT` (Ctrl-C) and `SIGTSTP` (Ctrl-Z).

- **Libraries used**: `stdio.h`, `stdlib.h`, `signal.h`
- **System calls**: None

---

#### Functions: `pid_mem_register`, `pid_cpu_register`, `pid_core_count_register`, `pid_core_freq_register`

These functions are used to **store** and **retrieve** the Process ID (PID) of each corresponding process. They act as simple registries using `static` variables to hold the PID across function calls.

- **Libraries used**: None
- **System calls**: None

---

#### Function: `handle_sigint`

When Ctrl-C is pressed, this function asks user to decide whether to exit, if `y` or `Y` then exit the program otherwise continue.

- **Libraries used**: `stdio.h`, `stdlib.h`, `signal.h`, `unistd.h`
- **System calls**: None

---

#### Function: `handle_sigtstp`

This function ignores Ctrl-Z (`SIGTSTP`) to prevent backgrounding the program.

- **Libraries used**: None
- **System calls**: None

---

#### Function: `main`

This is the main function of the program. It handles command-line arguments, sets up signal handlers, creates child processes for each module (memory, CPU, cores), sets up pipes, and prints the graphs. It also handles reading from pipes and outputting the corresponding graph information in real time.

- **Libraries used**: `stdio.h`, `stdlib.h`, `string.h`, `unistd.h`, `sys/types.h`, `sys/wait.h`, `signal.h`
- **System calls**: `fork()`, `pipe()`, `write()`, `read()`, `close()`, `kill()`, `waitpid()`

## 5. Project Execution Pseudocode
```pseudocode
begin

    // Step 1: Handle command-line arguments and store them in a struct
    args = commend_arguments(argc, argv)

    // Step 2: Set up signal handlers for Ctrl-C and Ctrl-Z
    signal_handlers_binding()

    // Step 3: Create pipes for inter-process communication
    create pipe for memory
    create pipe for cpu
    create pipe for core_count
    create pipe for core_freq

    // Step 4: Fork child processes to collect system info
    fork process for memory info
        in child:
            refresh the signals handler
            get total memory
            for each sample:
                get available memory
                calculate used memory
                write to pipe

    fork process for CPU info
        in child:
            refresh the signals handler
            get initial cpu time
            for each sample:
                sleep delay
                get new cpu time
                calculate usage
                write to pipe

    fork process for core number
        in child:
            refresh the signals handler
            get number of cores
            write to pipe

    fork process for core max. frequency
        in child:
            refresh the signals handler
            get max cpu frequency
            write to pipe

    // Step 5: In parent process, close write ends and read from pipes
    if memory enabled:
        read total memory
        print memory table axes

    if cpu enabled:
        print cpu table axes

    for each sample:
        if memory enabled:
            read used memory from pipe
            print memory usage and plot

        if cpu enabled:
            read cpu usage from pipe
            print cpu usage and plot

    if core display enabled:
        read number of cores and max. freq
        print core info and draw graph

    // Step 6: Wait for all children to finish
    wait for all child processes

    // Step 7: Clean up and exit

end
```

## 6. How to compile the code
1.`make`: complie the whole project

2.`make clean`: clean all the .o file and the executable

3.`make help`: get help information of the makefile

## 7. Expected Result
If using a mistaken CLA, print "Error: Please use correct command line arguments!"

Using one of or several of the CLAs: `--memory`, `--cpu`, `--cores` will display the corresponding tables on the screen.

Using one or both of the CLAs: `--samples=N` and `--tdelay=T` will control the number of samples and the delay in microseconds between each sample.

The `samples` and `tdelay` arguments can also be passed as positional arguments, in order: `samples tdelay`.

If no arguments are passed, it will display all tables: memory, CPU and cores, using default values (samples=20, tdelay=500000).

If only `samples` and/or `tdelay` are given as positional arguments, it will still display all tables using the given values.

If `Ctrl-C` is got during execution, the program will prompt:
"Get Ctrl-C (SIGINT). Do you want to exit? (y/n):"
If user enters `y` or `Y`, all child processes will be terminated and the program will exit.
If user enters `n`, the program will continue running normally.

If `Ctrl-Z` is received, the program will ignore it and continue running without interruption.

**Note**: Several Testing cases can be found in the video "CSCB09_A3_Testing_video"

## 8. Test Cases

Using one of or several of the CLAs: `--memory`, `--cpu`, `--cores`, `--samples=N`, `--tdelay=T` will display the corresponding tables on the screen in real-time.

Using only `--cores` will display number of cores and maximum frequency.

Using both `--samples=N` and `--tdelay=T` will control the total number of samples and time delay between samples in microseconds.
They can also be passed as positional arguments: `./myMonitoringTool 5 200000`

If no arguments are given, the program will display all tables: memory, CPU, and cores with default values (samples=20, tdelay=500000).

If only one number is passed as argument, the program will treat it as `samples`, and use default `tdelay`.

If using a wrong CLA (e.g., `--memori`, `--samp=abc`), the program will print:
"Error: Please use correct command line arguments!"

When pressing `Ctrl-C` during execution, the program will prompt:
"Get Ctrl-C (SIGINT). Do you want to exit? (y/n):"
If user types `y` or `Y`, all child processes will be terminated and the program will exit.
If user types `n`, the program will continue.

When pressing `Ctrl-Z`, the program will ignore the signal and continue as usual.

## 9. Disclaimer
The program requires a terminal window with at least 24 lines of height.
If the terminal has fewer rows, the graphical table output may become disorganized or overlap.

The program is designed to run on Linux-based systems.
It may not function correctly on non-Linux platforms such as Windows or macOS.

The code does not handle terminals with extremely small widths, which may cause formatting issues.

## 10. References
1. https://man7.org/linux/man-pages/man2/signal.2.html
2. https://docs.kernel.org/admin-guide/pm/cpufreq.html#cpu-performance-scaling
3. https://man7.org/linux/man-pages/man3/isdigit.3p.html
4. https://man7.org/linux/man-pages/man3/fflush.3.html
5. https://man7.org/linux/man-pages/man3/fscanf.3p.html
6. https://man7.org/linux/man-pages/man3/sscanf.3.html
7. https://man7.org/linux/man-pages/man3/usleep.3.html
8. https://man7.org/linux/man-pages/man2/sysinfo.2.html
9. https://man7.org/linux/man-pages/man5/proc_cpuinfo.5.html
10. https://docs.kernel.org/cpu-freq/cpufreq-stats.html
11. https://github.com/younghz/Markdown