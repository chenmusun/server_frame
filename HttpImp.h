#ifndef HTTP_IMP_H_
#define HTTP_IMP_H_
#include "ThreadPoolMethodImplementor.h"
#include <fcntl.h>
#include<unistd.h>

class HttpImp:public ThreadPoolMethodImp{
public:
        HttpImp();
        ~HttpImp();
        void operator()(void * arg,size_t length);
        int fd_;
};
#endif
