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

The program will prompt for a target IP and scan ports 1–1024.

## Current limitations / roadmap

- **Scanning is sequential, not multithreaded** — each port is scanned one at a time with up to a ~500ms timeout, so a full 1–1024 scan can take a noticeable amount of time, especially against hosts that silently drop packets instead of actively refusing. **Multithreading is planned** to scan many ports concurrently and bring this down significantly.
- No support yet for scanning a custom port range or a list of specific ports (currently hardcoded to 1–1024)
- No hostname resolution — IP addresses only for now
- IPv4 only

## License

MIT — see [LICENSE](LICENSE)
