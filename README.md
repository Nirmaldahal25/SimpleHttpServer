# Simple HTTP Server (C++)

A lightweight, multithreaded **HTTP server** written in **C++17** that serves static files from a directory.  
It uses a **thread pool** to efficiently handle multiple client connections using **blocking sockets**.

---

## ⚙️ Overview

This project implements a minimal HTTP server that:

- Accepts multiple client connections concurrently.
- Uses a **thread pool** to manage worker threads.
- Serves files from a specified **root folder** to a web browser.
- Supports basic **HTTP GET** requests for static content (HTML, CSS, JS, images, etc.).
- Uses **blocking sockets** for simplicity and predictable behavior.

---

## 🧠 How It Works

1. **Main Process**
   - Creates a listening socket and binds it to a specified port.
   - Waits for incoming client connections.
   - For each client connection, forwards the socket descriptor to a worker thread in the thread pool.

2. **Thread Pool**
   - A fixed number of worker threads are created at startup.
   - The main thread adds accepted client connections to a synchronized task queue.
   - Worker threads continuously fetch tasks from the queue and handle client requests.

3. **Request Handling**
   - Parses the client’s HTTP request line (e.g., `GET /index.html HTTP/1.1`).
   - Resolves the requested path to a file under the server’s root directory.
   - If the file exists, reads it and returns an HTTP `200 OK` response with proper headers.
   - If not, responds with `404 Not Found`.

---

