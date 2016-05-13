#include<iostream>
#include<memory>
#include"ThreadPool.h"
using namespace std;
class BasicMethod{
public:
        virtual      void operator()(void *arg)
                {
                        cout << "BasicMethod" << "\n";
                }
};


class AbstractMethod{
public:
        AbstractMethod(BasicMethod * bm){
                pbm_=bm;
        }
        BasicMethod * pbm_;
        void operator()(void * arg)
                {
                        (*pbm_)(arg);
                }
};




class FirstMethod:public BasicMethod{
public:
        void operator()(void *arg)
                {
                        cout << "FirstMethod" << "\n";
                }
};

class SecondMethod:public BasicMethod{
public:
        void operator()(void * arg)
                {
                        cout << "SecondMethod" << "\n";
                }
};
int main()
{
        ThreadPool tp(2);
        FirstMethod fm;
        SecondMethod sm;
        BasicMethod bm;
        AbstractMethod ab(&fm);
        std::shared_ptr<AbstractMethod> ptr(&ab);
        tp.enqueue(*ptr,(void *)NULL);
        // tp.enqueue(ab,(void *)NULL);
        // tp.enqueue(ab,(void *)NULL);
        while(1){}
}
