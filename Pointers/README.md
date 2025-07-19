# Lab 1: Working with Pointers

This lab focuses on practicing pointer usage, debugging, and string manipulation in C. The tasks include fixing incomplete struct declarations, tracing segmentation faults, implementing string parsing and merging functions, and understanding common pitfalls with C strings.

The lab materials and initial code were designed by **Professor Michael S. Kirkpatrick**. All code and documentation in this repository reflect **my own completed and tested work**.

## How to Run

1. Compile the lab code:
   ```bash
   make clean && make
2. Run the executable with:
   ```bash
   ./ptrs -m
3. Run the automated tests using:
   ```bash
   make test
  Note: 'make test' is used to check test results. For debugging, run the program manually as above.

4. To debug segmentation faults, use:
   ```bash
   gdb --args ./ptrs -m
  Then use 'start' and 'backtrace' within GDB to analyze faults.


## 🧾 License
The contents of this repository are provided for reference and educational review only.

🔒 No permission is granted to reuse, distribute, or submit this code, in whole or in part, for academic, commercial, or training purposes. The lab specifications and structure are the intellectual property of James Madison University and Professor Michael S. Kirkpatrick.

If you are a student in CS 361 or another course, please adhere strictly to your institution’s academic integrity policies.
