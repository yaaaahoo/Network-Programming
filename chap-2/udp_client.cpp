#include "common/common.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("usage: udpclient <IPaddress>\n");
        return 0;
    }

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr.s_addr);

    char send_line[MAX_LINE], recv_line[MAX_LINE];

    struct sockaddr* reply_addr = (struct sockaddr*)malloc(sizeof(servaddr));

    while (fgets(send_line, MAX_LINE, stdin) != NULL)
    {
        int i = strlen(send_line);
        if (send_line[i - 1] == '\n')
        {
            send_line[i - 1] = 0;
        }

        printf("now sending: %s\n", send_line);
        size_t srt = sendto(sock_fd, send_line, strlen(send_line), 0, (struct sockaddr*)(&servaddr), sizeof(servaddr));
        if (srt < 0)
        {
            printf("send failed.\n");
            return -1;
        }

        printf("send bytes: %zu\n", srt);

        socklen_t recv_len = 0;
        size_t rrt = recvfrom(sock_fd, recv_line, MAX_LINE, 0, reply_addr, &recv_len);
        if (rrt < 0)
        {
            printf("recv failed.\n");
            exit(-1);
        }

        std::cout << "rrt: " << rrt << std::endl;

        recv_line[rrt] = 0;
        printf("recv: %s\n", recv_line);
    }

    return 0;
}