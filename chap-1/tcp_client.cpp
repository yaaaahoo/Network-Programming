#include "common/common.h"

#define MSG_SIZE 10240

void SendData(int sockfd)
{
    char* buffer = (char*)malloc(MSG_SIZE + 1);
    for (int i = 0; i < MSG_SIZE; ++i)
    {
        buffer[i] = 'a';
    }
    buffer[MSG_SIZE] = '\0';

    const char* c_ptr = buffer;
    size_t remaining = strlen(buffer);
    while (remaining > 0)
    {
        int n_written = send(sockfd, c_ptr, remaining, 0);
        std::cout << "send into buffer " << n_written << std::endl;
        if (n_written <= 0)
        {
            std::cout << "send failed" << std::endl;
            return;
        }

        remaining -= n_written;
        c_ptr += n_written;
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: tcpclient <IPaddress>" << std::endl;
        return 0;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    int conn_rt = connect(sockfd, (struct sockaddr*)(&servaddr), (socklen_t)(sizeof(servaddr)));
    if (conn_rt < 0)
    {
        std::cout << "connect failed" << std::endl;
    }

    SendData(sockfd);

    return 0;
}