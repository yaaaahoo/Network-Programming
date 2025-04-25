
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdarg.h>        /* ANSI C header file */
#include <sys/uio.h>        /* for iovec{} and readv/writev */
#include <sys/un.h>

#include <iostream>

#define SERV_PORT 12345

#define MAX_LEN 4096

#define BACKLOG 1024

namespace common
{
struct Message
{
    enum class Type
    {
        TYPE1,
        TYPE2,
        PING,
        PONG,
    };

    Type type;
    char data[128];
};

size_t readn(int fd, void* buffer, size_t size);

void error(int err, char *fmt, ...);
}