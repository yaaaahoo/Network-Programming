#include "common.h"

namespace common{
size_t readn(int fd,void* buffer,size_t size)
{
    char*buffer_ptr=static_cast<char*>(buffer);
    size_t len=size;
    while(len>0)
    {
        int res=read(fd,buffer_ptr,len);
        if(res<0)
        {
            if(errno==EINTR)
            {
                // 考虑非阻塞的情况，需要再次调用read
                continue;
            }
            else
            {
                // EOF，表示套接字关闭
                return -1;
            }
        }

        len-=res;
        buffer_ptr+=res;
    }

    // 返回实际读取字节数
    return size-len;
}
}