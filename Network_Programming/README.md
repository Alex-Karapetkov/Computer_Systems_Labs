# 📂 CS 361 Network Client Lab

This repository contains my implementation of the **Network Client Lab** for **CS 361: Computer Systems II** at James Madison University. The lab focuses on network programming concepts including DNS resolution, socket communication, and simple HTTP client implementation.

The lab materials and specifications are based on the course content and guidance from **Professor Michael S. Kirkpatrick**. All code and implementation details reflect my own work.

---

## 🧠 Lab Overview

This lab implements a command-line tool that performs the following:

- **DNS Resolution:**  
  Uses `getaddrinfo()` to resolve domain names for a given protocol and IP version (IPv4 or IPv6).  
  It outputs the resolved IP addresses along with their transport protocol (TCP or UDP) and IP version.

- **HTTP GET Requests (Optional):**  
  When requested, the program connects to the specified server and retrieves the HTTP headers for a specified file using an HTTP/1.0 GET request.  
  It displays the HTTP status line and the content type returned by the server.

---

## 🛠️ How to Compile and Run

### Using Makefile (recommended):

A `Makefile` is included to simplify compilation. To build the program, run:

```bash
make
```

To clean compiled files and executables, run:
```bash
make clean
```
### Manual compilation:
Alternatively, compile manually with:

```bash
gcc -o net main.c client.c
```

### Running the program:
```bash
./net [options] domain
```

Options:

-p P — Specify application protocol (default: http)

-6 — Use IPv6 addresses instead of IPv4

-w F — Retrieve the HTTP file F from the domain (optional)

📂 Sample Usage
Test the program with real domain names and HTTP files, for example:

```bash
./net -p http www.jmu.edu
./net -6 -w /index.html www.example.com
```
## ⚠️ License and Usage
The contents of this repository are provided for reference and educational review only.

🔒 No permission is granted to reuse, distribute, or submit this code, in whole or in part, for academic, commercial, or training purposes. The lab specifications and materials are the intellectual property of James Madison University and Professor Michael S. Kirkpatrick.

Please adhere strictly to your institution’s academic integrity policies.

If you have any questions about this lab, feel free to reach out.

