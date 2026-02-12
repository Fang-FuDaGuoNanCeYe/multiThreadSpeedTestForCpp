#define tasknum 300
//#define threadnum 14U 这个没用到，我开的1024线程
#define loops 3000000
#include<iostream>
#include"cudaWorker.cuh"

auto do6(const int numTime)
{
    std::chrono::duration<double,std::milli> average;
    for(int i = 0;i<numTime;++i)
    {
        Fang::Test::work6 one;
        std::cout<<"Eng: Round "<<i+1<<"Chn: 第"<<i+1<<"轮";
        average+=Fang::Test::work6Start(one);
    }
    average/=numTime;
    return average;
}

int main()
{
    
    using namespace Fang::Test;
    std::cout<<"\nCuda 1024 threads: "<<std::chrono::duration<double,std::milli>(do6(10)).count();
    return 0;
}