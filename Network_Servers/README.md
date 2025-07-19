# 📂 CS 361 Simple HTTP Server Lab

This repository contains my implementation of the **Simple HTTP Server Lab** for **CS 361: Computer Systems II** at James Madison University. The lab focuses on basic network programming and HTTP protocol fundamentals by creating a minimal web server that serves static files on the local machine.

The lab materials and specifications are based on the course content and guidance from **Professor Michael S. Kirkpatrick**. All code and implementation details reflect my own work.

---

## 🧠 Lab Overview

This lab implements a basic TCP server that performs the following:

- **Server Initialization:**  
  Creates a TCP server socket bound to `localhost` (127.0.0.1) on a user-specified or default port.

- **Connection Handling:**  
  Listens for and accepts a single incoming client connection.

- **HTTP GET Request Parsing:**  
  Reads the client's HTTP GET request and extracts the requested URI (e.g., `/`, `/index.html`).

- **Static File Serving:**  
  Attempts to open and read the requested file from the current directory.  
  - If the URI is `/`, serves `index.html`.  
  - If the file exists, sends a proper HTTP/1.0 response header with content length and serves the file contents.  
  - If the file does not exist or the request is invalid, returns a `404 Not Found` response.

- **Clean Shutdown:**  
  Closes the client and server sockets after serving the request.

---

## 🛠️ How to Compile and Run

### Build the program using GCC:
```bash
gcc -o webserver main.c server.c utils.c
```
### Or build using the included Makefile:
```bash
make
```

This will produce an executable named 'webserver'.

### Run the server:
```bash
./webserver [-p port]
```
- -p port: Optional. Specify the port or application protocol to listen on (default is set in the program).

- The server listens on localhost and serves one request before exiting.

### 📂 Sample Usage
After running the server, you can test it by opening a web browser or using tools like curl:

``` bash
curl http://localhost:PORT/
curl http://localhost:PORT/index.html
curl http://localhost:PORT/bootstrap.html
```
Replace PORT with the port number you specify or the default.

### 🗂️ Sample Files
Place the following HTML files (or your own) in the same directory as the executable for testing:

- index.html

- bootstrap.html

If a requested file is missing, the server will respond with a 404 error.

## ⚠️ License and Usage
The contents of this repository are provided for reference and educational review only.

No permission is granted to reuse, distribute, or submit this code, in whole or in part, for academic, commercial, or training purposes. The lab specifications and materials are the intellectual property of James Madison University and Professor Michael S. Kirkpatrick.

Please adhere strictly to your institution’s academic integrity policies.

If you have any questions about this lab, feel free to reach out.
