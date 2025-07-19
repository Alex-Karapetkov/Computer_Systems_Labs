# 🔄 Lab: Inter-Process Communication with Pipes and `posix_spawn`

This lab demonstrates how to implement **inter-process communication (IPC)** between a parent and child process in C using **pipes** and **process creation** mechanisms like `fork()` and `posix_spawn()`. The parent spawns a child process that runs the Unix utility `cksum` to compute a checksum of a file, then captures and processes the result.

The lab is based on instructional material by **Professor Michael S. Kirkpatrick** as part of **CS 361: Computer Systems II** at James Madison University. However, the implementation presented here reflects my own original work and understanding.

> 🔒 This repository is for viewing and educational reference only. Do not reuse, copy, or modify this code for coursework, interviews, or other submissions.

---

## 🧪 What This Lab Does

- Accepts a filename as input.
- Spawns a child process to run `cksum`, using either:
  - `fork()` and `exec()` (default), or
  - `posix_spawn()` (optional via `-s` flag).
- Uses a pipe to redirect the child’s `stdout` to the parent.
- Parses the output to extract and print only the checksum value.

---

## 📁 Sample Files

A `data/` folder is included in this lab containing sample files for testing:

- `f1.txt`
- `f2.txt`
- `f3.txt`

These files contain test content that can be used to verify that the program correctly calculates and extracts checksum values via `cksum`.

## 🛠️ How to Compile and Run

### 🧰 Compile
```bash
gcc -o ipc main.c pipe.c
```
### ▶️ Run (default using fork()/exec())
```bash
./ipc data/f1.txt
```
### ▶️ Run using posix_spawn()
```bash
./ipc -s data/f2
```

## 📝 Notes
This lab was tested and run on the departmental Linux server stu.cs.jmu.edu.

The child’s stdout is piped into the parent process for parsing.

If a file does not exist or is invalid, the program will display an error and exit.
