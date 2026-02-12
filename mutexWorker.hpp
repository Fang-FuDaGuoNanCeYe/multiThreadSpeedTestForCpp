#pragma once
#include<map>
#include<thread>
#include<list>
#ifndef threadnum
#define threadnum 16U
#endif
#ifndef tasknum
#define tasknum 30000000ULL
#endif
#include"timeWaster.h"
#include<mutex>
#include<chrono>
#include<print>
namespace Fang::Test
{
    class work1
    {
        public:
        std::list<std::jthread> pool;
        std::list<std::pair<unsigned long long,unsigned long long>> tasks;
        std::multimap<unsigned long long, unsigned long long> result;
        std::list<std::mutex> mutexPool;
        std::mutex write;
        work1()
        {
            for(unsigned long long i=0;i<tasknum;++i)
            {
                tasks.emplace_back(i,i+1);
                mutexPool.emplace_back();
            }
        }
    };
    void doWork1(work1 & w)
    {
        
        std::list<std::mutex>::iterator iM=w.mutexPool.begin();
        std::multimap<unsigned long long, unsigned long long> resultLocal;
        for(auto i=w.tasks.begin();
        i!=w.tasks.end();++i,++iM)
        {
            if(iM->try_lock())
            {
                resultLocal.emplace(i->first,Fang::Test::doSomething(i->second));
            }
        }
        std::unique_lock<std::mutex> writer(w.write);
        w.result.insert(resultLocal.begin(),resultLocal.end());
        

    }

    auto work1Start(work1 & w)
    {
        auto now = std::chrono::high_resolution_clock::now();
        for(unsigned int i = 0;i<threadnum;++i)
        {
            w.pool.emplace_back(doWork1,std::ref(w));
        }
        for(auto i = w.pool.begin();i!=w.pool.end();++i)
        {
            i->join();
        }
        auto endNow = std::chrono::high_resolution_clock::now();
        std::println("mutex 组工作完成，用时：{}",std::chrono::duration<double,std::milli>(endNow-now));
        unsigned int k = 1;
        for(auto i = w.result.begin();i!=w.result.end();++i,++k)
        {
            if(k!=i->second)
            {
                std::println("result wrong 结果错误：{}={}",k,*i);
                return std::chrono::duration<double,std::milli>(endNow-now);
            }
        }
        std::println("result correct 结果正确");
        return std::chrono::duration<double,std::milli>(endNow-now);
    }
}