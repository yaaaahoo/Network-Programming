#include "common/common.h"

using namespace common;

int main(int argc, char** argv)
{
    if(argc<2)
    {
        error(1, "need ip");
    }

    int fd=socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0)
    {
        error(errno, "create socket failed");
    }

    struct sockaddr_in caddr;
    caddr.sin_family=AF_INET;
    caddr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &caddr.sin_addr.s_addr);
    
    int ret=connect(fd, (struct sockaddr*)(&caddr), sizeof(caddr));
    if(ret!=0)
    {
        error(errno, "connect error");
    }

    char buffer[MAX_LEN];
    char* pre_str="I am ";
    struct iovec iov[2];
    iov[0].iov_base=pre_str;
    iov[0].iov_len=strlen(pre_str);

    while(1)
    {
        bzero(buffer, sizeof(buffer));
        if(fgets(buffer, sizeof(buffer), stdin)!=nullptr)
        {
            int len=strlen(buffer);
            if(buffer[len-1]=='\n')
            {
                buffer[len-1]='\0';
            }

            iov[1].iov_base=buffer;
            iov[1].iov_len=len;

            int wn=writev(fd, iov, 2);
            if(wn<0)
            {
                error(errno, "writev error");
            }
            
        }
    }

    return 0;
}