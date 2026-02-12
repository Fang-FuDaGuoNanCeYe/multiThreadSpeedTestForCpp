#pragma once
#include<vector>
#include<map>
#include<list>
#include<thread>
#include<atomic>
#ifndef threadnum
#define threadnum 16U
#endif
#ifndef tasknum
#define tasknum 3000000000ULL
#endif
#include"timeWaster.h"
#include<mutex>
#include<chrono>
#include<print>
namespace Fang::Test
{
    class work2
    {
        public:
        std::list<std::jthread> pool; 
        std::mutex write;
        std::atomic<long long> process;
        std::multimap<long long,unsigned long long> result;
        std::vector<unsigned long long> tasks;
        work2()
        {
            for(unsigned long long i = 0 ;i<tasknum;++i)
            {
                tasks.emplace_back(i+1);
            }
            process.store((long long)(tasks.size())-1);
        }
    };

    void doWork2(work2 & w)
    {
        long long nowWorks;
        std::multimap<long long,unsigned long long> resultLocal;
        for( nowWorks=w.process.fetch_sub(1LL);nowWorks>=0;nowWorks=w.process.fetch_sub(1LL))
        {
            //std::println("{}",nowWorks);
            resultLocal.emplace(nowWorks,Fang::Test::doSomething(w.tasks[nowWorks]));
        }
        std::unique_lock<std::mutex> lock(w.write);
        w.result.insert(resultLocal.begin(),resultLocal.end());
    }

    auto work2Start(work2 & w)
    {
        auto now = std::chrono::high_resolution_clock::now();
        for(unsigned int i = 0;i<threadnum;++i)
        {
            w.pool.emplace_back(doWork2,std::ref(w));
        }
        for(auto i = w.pool.begin();i!=w.pool.end();++i)
        {
            i->join();
        }
        auto endNow = std::chrono::high_resolution_clock::now();
        std::println("atomic 组工作完成，用时：{}",std::chrono::duration<double,std::milli>(endNow-now));
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