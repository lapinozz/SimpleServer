#pragma once

#include "utility.hpp"

#ifdef _WIN32
  /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501  /* Windows XP. */
  #endif
  #include <winsock2.h>
  #include <Ws2tcpip.h>

  #define NON_BLOCK_FLAG FIONBIO

  #define SOCKET_IS_INVALID(_v)(_v == INVALID_SOCKET)
  using SOCKLEN = int;
#else
  /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
  #include <unistd.h> /* Needed for close() */
  #include <fcntl.h>

  #define NON_BLOCK_FLAG O_NONBLOCK

  #define SOCKET_IS_INVALID(_v)(_v < 0)

  #define INVALID_SOCKET -1
  #define SOCKET_ERROR  -1
  using SOCKET = int;
  using SOCKLEN = unsigned int;
#endif

int sockInit(void)
{
  #ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1,1), &wsa_data);
  #else
    return 0;
  #endif
}

int sockQuit(void)
{
  #ifdef _WIN32
    return WSACleanup();
  #else
    return 0;
  #endif
}

int sockClose(SOCKET sock)
{
  #ifdef _WIN32
    #define CLOSE_SOCKET_FUNCTION closesocket
    #define SHUTDOWN_FLAG SD_SEND
  #else
    #define CLOSE_SOCKET_FUNCTION close
    #define SHUTDOWN_FLAG SHUT_WR
  #endif

  int status = 0;

  status = shutdown(sock, SHUTDOWN_FLAG);
  if(status != 0)
    return status;

//TODO set a timeout here
  while(true)
  {
      char buff[16];
      auto size = recv(sock, buff, sizeof(buff), 0);

      if(size <= 0)
        break;
  }

  status = CLOSE_SOCKET_FUNCTION(sock);

  return status;

  #undef CLOSE_SOCKET_FUNCTION
  #undef SHUTDOWN_FLAG
}

bool setBlocking(int fd, bool blocking)
{
    #ifdef WIN32
       unsigned long mode = blocking ? 0 : 1;
       return (ioctlsocket(fd, FIONBIO, &mode) == 0);
    #else
       int flags = fcntl(fd, F_GETFL, 0);
       if (flags < 0) return false;
       flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
       return (fcntl(fd, F_SETFL, flags) == 0);
    #endif
}

SOCKET makeListener(int port, bool blocking = true)
{
    auto listSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(SOCKET_IS_INVALID(listSocket))
        return listSocket;

    int iSetOption = 1;
    setsockopt(listSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

    struct sockaddr_in listAddr;
    memset(&listAddr, 0, sizeof(listAddr));

    listAddr.sin_family = AF_INET;
    listAddr.sin_addr.s_addr = INADDR_ANY;
    listAddr.sin_port = htons(port);

    auto error = bind(listSocket, (struct sockaddr *) &listAddr, sizeof(listAddr));
    if(error == SOCKET_ERROR)
        return error;

    listen(listSocket, 5);

    setBlocking(listSocket, blocking);

    return listSocket;
}

SOCKET makeSocket(SOCKET listener, uint64_t timeoutMs = 0, bool blocking = true)
{
    auto newSocket = INVALID_SOCKET;

    struct sockaddr_in newSocketAddr;
    SOCKLEN addrSize = sizeof(newSocketAddr);

    auto time = getTimeMs();

    while(SOCKET_IS_INVALID(newSocket) && (timeoutMs == 0 || getTimeMs() - time < timeoutMs))
        newSocket = accept(listener, (struct sockaddr*)&newSocketAddr, &addrSize);

    setBlocking(newSocket, blocking);

    return newSocket;
}
