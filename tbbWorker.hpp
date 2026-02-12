#pragma once
#include<map>
#include<thread>
#include<vector>
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
#include<tbb/parallel_for.h>
namespace Fang::Test
{
    class work5
    {
        public:
        std::vector<std::pair<unsigned long long , unsigned long long>> tasks;
        std::multimap<unsigned long long , unsigned long long> result;
        std::mutex write;
        work5()
        {
            for(unsigned long long i=0;i<tasknum;++i)
            {
                tasks.emplace_back(i,i+1);
            }
        }
    };
    void doWork5(work5 & w)
    {
        tbb::parallel_for(tbb::blocked_range<size_t>(0,w.tasks.size()),
        [&w](tbb::blocked_range<size_t>& sizeHere)
        {
            std::multimap<unsigned long long , unsigned long long> resultLocal;
            for(auto i {sizeHere.begin()};i<sizeHere.end();++i)
            {
                resultLocal.emplace(w.tasks[i].first,Fang::Test::doSomething(w.tasks[i].second));
            }
            std::unique_lock<std::mutex> locker(w.write);
            w.result.insert(resultLocal.begin(),resultLocal.end());
        },tbb::auto_partitioner());
    }

    auto work5Start(work5 & w)
    {
        auto now = std::chrono::high_resolution_clock::now();
        doWork5(std::ref(w));
        auto endNow = std::chrono::high_resolution_clock::now();
        std::println("intel (one)TBB 组工作完成，用时：{}",std::chrono::duration<double,std::milli>(endNow-now));
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