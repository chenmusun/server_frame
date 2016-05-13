#ifndef THREAD_POOL_METHOD_H_
#define THREAD_POOL_METHOD_H_
#include "ThreadPoolMethodImplementor.h"
#include "nedmalloc.h"
class ThreadPoolMethod{
public:
        ThreadPoolMethod(ThreadPoolMethodImp * imp){
                imp_=imp;
        }
        void operator()(void * arg,size_t length)
        {
                (*imp_)(arg,length);
                //回收内存
                nedalloc::nedfree(arg);
        }
private:
        ThreadPoolMethodImp * imp_;
};
#endif
