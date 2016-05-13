#ifndef THREAD_POOL_METHOD_IMP_H_
#define THREAD_POOL_METHOD_IMP_H_
#include<stddef.h>
class ThreadPoolMethodImp{
public:
        virtual void operator()(void * arg,size_t length){
        }
};

/* void ThreadPoolMethodImp::operator()(void *arg,size_t length) */
/* { */
/*         //do nothing */
/* } */
#endif
