
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#define SERV_PORT 12345

#define MAX_LINE 4096

namespace common
{
size_t readn(int fd, void* buffer, size_t size);
}