# Simple port scanner

A simple Windows TCP port scanner, written as a learning project to understand sockets, non-blocking I/O, and `select()`.

## How it works

1. Initializes Winsock via `WSAStartup`
2. For each port in the scanned range, opens a TCP socket (`socket`) and switches it to non-blocking mode (`ioctlsocket` + `FIONBIO`)
3. Calls `connect()` — in non-blocking mode this returns immediately with `WSAEWOULDBLOCK`, meaning the connection attempt is in progress
4. Uses `select()` with a timeout to wait until the socket becomes writable (connection succeeded) or an exception is signaled (connection failed), or the timeout expires (port filtered / host not responding)
5. Reports the port as open, closed, filtered or an error based on the result

This is a classic **TCP connect scan** — the simplest and least stealthy method (unlike nmap's SYN scan, it completes the full TCP handshake and doesn't require raw sockets or admin privileges).

## Build

**Requirements:**
- Windows 10/11
- Visual Studio 2022 (or newer) with the Desktop development with C++ workload
- Linked library: `ws2_32.lib`

**Steps:**
1. Open the project in Visual Studio
2. Set configuration to `Release` / `x64`
3. Build (`Ctrl+Shift+B`)

## Usage
