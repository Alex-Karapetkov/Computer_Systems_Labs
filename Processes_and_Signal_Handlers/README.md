# 🧪 Lab: Process Creation and Signal Handling (CS 361)

This directory contains my implementation of a lab assignment for **CS 361: Computer Systems II** at James Madison University. The lab explores core UNIX/Linux system programming concepts including process management and signal handling in C.

The original lab specifications and starter materials were authored by **Professor Michael S. Kirkpatrick**. However, **all final code and submitted work in this lab reflect my own effort, design decisions, and understanding**. This lab may not be reused or adapted in any form.

---

## 📋 Lab Overview

In this lab, I implemented a process-driven C program that can:

- Fork a child process, monitor its termination, and report exit status.
- Set up custom signal handlers using `sigaction`.
- Safely intercept and respond to signals like `SIGINT` and `SIGSEGV`.
- Demonstrate asynchronous behavior using `setitimer` and signal timers.
- Map string signal names (e.g., `"INT"`, `"SEGV"`) to actual `SIG*` constants.

The lab demonstrates safe, predictable handling of system-level signals and provides foundational experience for building responsive, concurrent systems.

---

## 🛠️ Files

| File         | Description                                      |
|--------------|--------------------------------------------------|
| `main.c`     | Entry point that parses arguments, sets mode, and controls execution. |
| `child.c`    | Contains child process behavior logic.           |
| `signals.c`  | Signal lookup logic and custom signal handler implementations. |
| `signals.h`  | Definitions for signal mapping and handler function prototypes. |
| `child.h`    | Header for `child.c`.                            |

---

## ▶️ How to Compile and Run

1. **Compile the program**:
   ```bash
   gcc -o procs main.c child.c signals.c

### Or build using the included Makefile:
```bash
make
```

2. Run default mode (basic fork + wait):
   ```bash
   ./procs
   ```
3. run with signal override mode:
```bash
./procs -s INT     # Handles SIGINT (Ctrl+C)
./procs -s SEGV    # Triggers and handles a segmentation fault

```
The following signal names are recognized:

INT (SIGINT)
SEGV (SIGSEGV)
KILL, STOP, TSTP (not all are handled but are included for lookup)

## ⚠️ License and Academic Integrity
All lab materials originate from Professor Kirkpatrick and James Madison University. This repository is provided for educational viewing only.

🚫 Do not reuse, submit, or redistribute this code for any academic, commercial, or training purposes.

Please honor academic integrity and your institution’s collaboration policies.
