# System Monitoring Tool

## How did I solve the problem?

To implement the system monitoring tool, I structured the program into multiple modules, each responsible for handling different aspects of system monitoring: **Memory**, **CPU**, and **Cores**. Here’s how I approached each component:

## 1. Handling Command-Line Arguments
Since the command format has multiple variations with specific requirements, I designed a dedicated function to process these arguments. To store the processed data efficiently, I created a **struct** that holds relevant information:
- Whether **Memory, CPU, or Core monitoring** is required (set to `1` if needed, `0` otherwise).
- The **number of samples** (`--samples`) and **time delay** (`--tdelay`).
This allows for a structured and efficient way to handle user input.

## 2. Memory Monitoring Module
The **Memory module** consists of multiple functions:
- **Retrieve total memory** from the **sysinfo header file** (`sys/sysinfo.h`).
- **Retrieve available memory** from the **`/proc/meminfo` file**.
- This combination provides a **more accurate representation of actual memory usage**, aligning closely with `free -h` command output.
- **Draw memory axis** to display memory usage visually.
- **Plot memory usage points** on the graph.

## 3. CPU Monitoring Module
The **CPU module** required additional complexity due to time-based calculations:
- One function retrieves **total CPU time** from `/proc/stat`.
- Another function retrieves **both total and active CPU time** and stores them in a **struct**.
- A separate function calculates **CPU utilization** using two recorded snapshots of CPU time.
- Additional functions draw the **CPU usage axis** and **plot CPU usage points**.
- **To obtain the first CPU utilization value**, I perform an **initial CPU time snapshot** before the main loop starts. This ensures that the first recorded utilization is meaningful and based on real differences in CPU time.

## 4. Memory & CPU Synchronization
Since **CPU usage must always be displayed below Memory usage**, I implemented logic to check whether memory monitoring is enabled:
- If **Memory monitoring is enabled**, the **screen scrolls down by 10 lines** to ensure CPU information has space for display.
- CPU plotting coordinates adjust dynamically based on whether Memory is enabled.

## 5. Core Monitoring Module
The **Core module** retrieves and displays information about CPU cores and max CPU frequency:
- **The number of cores** is retrieved from `/proc/cpuinfo`.
- **The maximum CPU frequency** is read from two possible sources:
  - **Primary file** (from `/sys/devices/system/cpu/cpu0/cpufreq/`)
  - **Fallback method** (scanning `/proc/cpuinfo` for the highest frequency value).

For visualization:
- A function **draws small blocks** representing each core.
- The x-axis displays **the number of cores**.
- The position of Core information adjusts dynamically based on whether Memory and CPU monitoring are enabled:
  - If **both Memory and CPU are displayed**, the **screen scrolls down by 15 lines** before displaying the Core visualization.
  - If **only Memory or CPU is displayed**, the **screen scrolls down by 10 lines**.

## 6. Main Function
The `main()` function orchestrates the entire system:
1. **Processes command-line arguments.**
2. **Handles Memory monitoring** first.
3. **Handles CPU monitoring** next.
4. **Handles Core monitoring** last.
5. **Ensures the correct positioning of each component** by adjusting screen scrolling and axis placement.

## 7. Makefile for Compilation
To streamline compilation and execution, I wrote a **Makefile**:
- Running `make` **automatically compiles the program**.
- Running `make clean` **removes all compiled files**, ensuring a clean build environment.

## 8. Program Usage
### **Command Line Arguments**
The program accepts various command-line arguments:
<<<<<<< HEAD

| Argument | Description |
|----------|-------------|
| `--memory` | Display **only** memory usage. |
| `--cpu` | Display **only** CPU usage. |
| `--cores` | Display **only** core information. |
| `--samples=N` | Specify **N** times to collect and average statistics (Default: **20**). |
| `--tdelay=T` | Set delay **T** in microseconds between samples (Default: **500000 µs** = 0.5 seconds). |

## **Positional Arguments**
If no flags are used, the **first two numbers** are interpreted as:
```bash
./myMonitoringTool [samples] [tdelay]
```

## Final Thoughts

This project effectively combines **real-time system monitoring**, **dynamic terminal visualization**, and **structured modular programming**. By integrating **file parsing**, **system calls**, and **graphical representation in the terminal**, it provides a comprehensive tool for monitoring CPU, Memory, and Core usage.

### 🎉 Thank you for reading! See you next time! 🚀