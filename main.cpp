#define tasknum 300ULL
#define threadnum 14U
#define loops 3000000
#include"atomicBunchWorker.hpp"
#include"atomicWorker.hpp"
#include"mutexWorker.hpp"
#include"workStealWorker.hpp"
#include"tbbWorker.hpp"
//#include"cudaWorker.cuh"
auto do1(const int numTime)
{
    std::chrono::duration<double,std::milli> average;
    for(int i = 0;i<numTime;++i)
    {
        Fang::Test::work1 one;
        std::print("Eng: Round {}; Chn: 第{}次",i+1,i+1);
        average+=Fang::Test::work1Start(one);
    }
    average/=numTime;
    return average;
}

auto do2(const int numTime)
{
    std::chrono::duration<double,std::milli> average;
    for(int i = 0;i<numTime;++i)
    {
        Fang::Test::work2 one;
        std::print("Eng: Round {}; Chn: 第{}次",i+1,i+1);
        average+=Fang::Test::work2Start(one);
    }
    average/=numTime;
    return average;
}

auto do3(const int numTime)
{
    std::chrono::duration<double,std::milli> average;
    for(int i = 0;i<numTime;++i)
    {
        Fang::Test::work3 one;
        std::print("Eng: Round {}; Chn: 第{}次",i+1,i+1);
        average+=Fang::Test::work3Start(one);
    }
    average/=numTime;
    return average;
}

auto do4(const int numTime)
{
    std::chrono::duration<double,std::milli> average;
    for(int i = 0;i<numTime;++i)
    {
        Fang::Test::work4 one;
        std::print("Eng: Round {}; Chn: 第{}次",i+1,i+1);
        average+=Fang::Test::work4Start(one);
    }
    average/=numTime;
    return average;
}

auto do5(const int numTime)
{
    std::chrono::duration<double,std::milli> average;
    for(int i = 0;i<numTime;++i)
    {
        Fang::Test::work5 one;
        std::print("Eng: Round {}; Chn: 第{}轮",i+1,i+1);
        average+=Fang::Test::work5Start(one);
    }
    average/=numTime;
    return average;
}
/*
auto do6(const int numTime)
{
    std::chrono::duration<double,std::milli> average;
    for(int i = 0;i<numTime;++i)
    {
        Fang::Test::work6 one;
        average+=Fang::Test::work6Start(one);
    }
    average/=numTime;
    return average;
}*/

int main()
{
    
    using namespace Fang::Test;
    
    std::println("\n\n{: >60s}: {}\n{: >60s}: {}\n{: >60s}: {}\n{: >60s}: {}\n{: >60s}: {}",//\n{: >40s}: {}",
        "Eng: mutex lock; Chn: mutex锁用时",do1(10),"Eng: atomic process ctrl; Chn: 原子进度控制用时",do2(10),
        "Eng: Batch Atomic process ctrl; Chn: 块原子获取用时",do3(10),"Eng: Work stealing; Chn: 工作偷取用时",do4(10),
        "Eng/Chn: intel (one)TBB parallel_for",do5(10));//,"Cuda 1024个线程",do6(10));
    return 0;
}