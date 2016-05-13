#include "HttpImp.h"

HttpImp::HttpImp()
{
        fd_=open("./test.log",O_RDWR|O_CREAT|O_APPEND,S_IRWXU);
}

HttpImp::~HttpImp()
{
        close(fd_);
}

void HttpImp::operator()(void *arg,size_t length)
{
        write(fd_,arg,length);
        // int log_fd=open("./test.log",O_RDWR|O_CREAT|O_APPEND,S_IRWXU);
        // if(log_fd!=-1)
        //     ptci->tcp_log_fd_=log_fd;

}
