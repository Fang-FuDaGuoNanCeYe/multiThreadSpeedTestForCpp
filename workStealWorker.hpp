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
#define tasknum 30000000ULL
#endif
#include"timeWaster.h"
#include<mutex>
#include<chrono>
#include<print>
#include<deque>
#include<array>
namespace Fang::Test
{
    class work4
    {
        public:
        std::list<std::jthread> pool; 
        std::mutex write;
        std::multimap<long long, unsigned long long> result;
        std::vector<std::deque<std::pair<unsigned long long , unsigned long long>>> tasks;
        std::array<std::mutex,threadnum> stealLock;
        work4()
        {
            for(unsigned long long i = 0;i<threadnum;++i)
            {
                tasks.emplace_back();
            }
            for(unsigned long long i = 0 ;i<tasknum;++i)
            {
                tasks[i%threadnum].emplace_back(std::pair<unsigned long long , unsigned long long>(i,i+1));
            }
        }
    };

    void doWork4(work4 & w,unsigned long long threadId)
    {
        std::multimap<long long,unsigned long long> resultLocal;
        std::pair<unsigned long long,unsigned long long> tmp {w.tasks[threadId].begin()->first,w.tasks[threadId].begin()->second};
        while(w.tasks[threadId].size()>0ULL)
        {
            w.stealLock[threadId].lock();
            if(w.tasks[threadId].size()>0)
            {
                std::pair<unsigned long long,unsigned long long> tmp {w.tasks[threadId].begin()->first,w.tasks[threadId].begin()->second};
                w.tasks[threadId].pop_front();
                w.stealLock[threadId].unlock();
                resultLocal.emplace(tmp.first,Fang::Test::doSomething(tmp.second));
            }
            else  
            {
                w.stealLock[threadId].unlock();
            }
        }
        
        for(unsigned long long i = threadId+1;i<threadId+threadnum;++i)
        {
            while(w.tasks[i%threadnum].size()>10ULL)
            {
                w.stealLock[i%threadnum].lock();
                if(w.tasks[i%threadnum].size()>0ULL)
                {
                    
                    auto i_burg {w.tasks[i%threadnum].end()};
                    --i_burg;
                    std::pair<unsigned long long,unsigned long long> stolen = {i_burg->first,i_burg->second};
                    w.tasks[i%threadnum].pop_back();
                    w.stealLock[i%threadnum].unlock();
                    resultLocal.emplace(stolen.first,Fang::Test::doSomething(stolen.second));
                }
                else  
                {
                    w.stealLock[i%threadnum].unlock();
                }
            }
        }

        std::unique_lock<std::mutex> lock(w.write);
        w.result.insert(resultLocal.begin(),resultLocal.end());
    }

    auto work4Start(work4 & w)
    {
        auto now = std::chrono::high_resolution_clock::now();
        for(unsigned int i = 0;i<threadnum;++i)
        {
            w.pool.emplace_back(doWork4,std::ref(w),static_cast<unsigned long long>(i));
        }
        for(auto i = w.pool.begin();i!=w.pool.end();++i)
        {
            i->join();
        }
        auto endNow = std::chrono::high_resolution_clock::now();
        std::println("steal 组工作完成，用时：{}",std::chrono::duration<double,std::milli>(endNow-now));
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