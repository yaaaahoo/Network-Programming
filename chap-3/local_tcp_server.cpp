#include <sys/un.h>

#include "common.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "server: need local sock file" << std::endl;
        return 0;
    }

    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    struct sockaddr_un cli_addr, server_addr;
    char* local_path = argv[1];
    unlink(local_path);
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, local_path);

    if (bind(listenfd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cout << "bind failed." << std::endl;
        return -1;
    }

    if (listen(listenfd, BACKLOG) < 0)
    {
        std::cout << "listen failed." << std::endl;
        return -1;
    }
}