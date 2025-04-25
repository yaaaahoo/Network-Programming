#include "common.h"

namespace common
{
size_t readn(int fd, void* buffer, size_t size)
{
    char* buffer_ptr = static_cast<char*>(buffer);
    size_t len = size;
    while (len > 0)
    {
        int res = read(fd, buffer_ptr, len);
        if (res < 0)
        {
            if (errno == EINTR)
            {
                // 考虑非阻塞的情况，需要再次调用read
                continue;
            }
            else
            {
                return -1;
            }
        }
        else if (res == 0)
        {
            // EOF，表示套接字关闭
            break;
        }

        len -= res;
        buffer_ptr += res;
    }

    // 返回实际读取字节数
    return size - len;
}

void error(int err, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (err)
        fprintf(stderr, ": %s (%d)\n", strerror(err), err);

    exit(1);
}
}  // namespace common