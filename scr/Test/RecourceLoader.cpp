#include <iostream>
#include "Utility/TestProc.hpp"
#include "Utility/RecourceLoader.hpp"

using namespace std;
using namespace util;

struct A: public ARecource{
    virtual ~A(){}

    virtual void Load(){
        a=1;
    }  

    int a;
};
struct B: public ARecource{
    virtual ~B(){}

    virtual void Load(){
        a=2;
    }  

    int a;
};
struct C: public ARecource{
    virtual ~C(){}

    virtual void Load(){
        a=3;
    }  

    int a;
};

void Test(){
    Recources.Add<A>();
    if(Recources.Get<A>().a!=1)throw std::logic_error("Cant find A, added A");

    Recources.Add<B>();
    if(Recources.Get<A>().a!=1)throw std::logic_error("Cant find A, added A, B");
    if(Recources.Get<B>().a!=2)throw std::logic_error("Cant find B, added A, B");

    Recources.Add<C>();
    if(Recources.Get<A>().a!=1)throw std::logic_error("Cant find A, added A, B, C");
    if(Recources.Get<B>().a!=2)throw std::logic_error("Cant find B, added A, B, C");
    if(Recources.Get<C>().a!=3)throw std::logic_error("Cant find C, added A, B, C");
}

int main(){
    testutil::TestException("recource-loader", Test);
}