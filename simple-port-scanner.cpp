#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

enum PortStatus {
    PORTOPEN,
    CLOSED,
    FILTERED,
    PORTERROR
};

std::string port_status_to_string(PortStatus status) {
	switch (status) {
	case PORTOPEN: return "Open";
	case CLOSED: return "Closed";
	case FILTERED: return "Filtered";
	case PORTERROR: return "Error";
	default: return "Unknown";
	}
}

PortStatus scan_port(const std::string& ip, int port)
{
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        return PORTERROR;
    }

    u_long mode = 1;
    if (ioctlsocket(sock, FIONBIO, &mode) != 0) {
        std::cerr << "ioctlsocket failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        return PORTERROR;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) {
            closesocket(sock);
            return PORTERROR;
        }

        fd_set writefds;
        FD_ZERO(&writefds);
        FD_SET(sock, &writefds);

        fd_set exceptfds;
        FD_ZERO(&exceptfds);
        FD_SET(sock, &exceptfds);

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

        int selectResult = select(0, nullptr, &writefds, &exceptfds, &timeout);

        if (selectResult == 0) {
            closesocket(sock);
            return FILTERED;
        }
        if (FD_ISSET(sock, &exceptfds)) {
            closesocket(sock);
            return CLOSED;
        }
        if (FD_ISSET(sock, &writefds)) {
            closesocket(sock);
            return PORTOPEN;
        }
    }

    closesocket(sock);
    return PORTERROR;
}

int main()
{
    std::cout << "Ip to scan: ";

	std::string ip;
    std::cin >> ip;

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    for (int i = 1; i <= 1024; i++)
	{
		PortStatus status = scan_port(ip, i);
		if (status == PORTOPEN)
		{
			std::cout << "Port " << i << " is open." << std::endl;
		}
	}
    WSACleanup();
    return 0;
}