#pragma once

#ifndef threadnum
#define threadnum 16U
#endif
#ifndef tasknum
#define tasknum 30000000ULL
#endif
#ifndef loops
#define loops 1
#endif
#include<array>
#include<chrono>
#include<iostream>
//#include<print> std cpp 23 才有print，这是cpp17
namespace Fang
{
    namespace Test
    {

        
        class work6
        {
            public:
            unsigned long long tasks[tasknum];
            unsigned long long result[tasknum];
            
            work6()
            {
                for(unsigned long long i = 0;i<tasknum;++i)
                {
                    tasks[i] = i+1;
                }
                
            }
            
        };

        __device__ auto doesWork6(unsigned long long a)
        {
            unsigned long long a_ = a;
            for(int i = 0;i<loops;++i)
            {
                a=a_;
            
                while(a>1)
                {
                    if(a%2==0)
                    {
                        a/=2;
                    }
                    else
                    {
                        a=a*3+1;
                    }
                }
            }
            return a*a_;
        
        }

        __global__ void doWork6(unsigned long long * tasks, unsigned long long * results, unsigned long long length)
        {
            for(unsigned long long i = blockDim.x * blockIdx.x + threadIdx.x ; i<length ; i+=1024)
            {
                results[i] = doesWork6(tasks[i]);
            }
        }

        auto work6Start(work6 & w)
        {
            unsigned long long *tasks, *results;
            auto now = std::chrono::high_resolution_clock::now();
            cudaMalloc((void **)& tasks,sizeof(unsigned long long) * tasknum);
            cudaMalloc((void **)& results,sizeof(unsigned long long) * tasknum);
            cudaMemcpy(tasks,&(w.tasks),sizeof(unsigned long long) * tasknum,cudaMemcpyHostToDevice);
            doWork6<<<64,1024/64>>>(tasks, results, tasknum);
            cudaMemcpy(&(w.result),results,sizeof(unsigned long long) * tasknum,cudaMemcpyDeviceToHost);
            cudaFree(tasks);
            cudaFree(results);
            auto endNow = std::chrono::high_resolution_clock::now();
            std::cout<<"cuda completed in "<< std::chrono::duration<double,std::milli>(endNow-now).count()<<"ms"<<"\n";
            
            for(unsigned int k = 1,i=0;i!=tasknum;++i,++k)
            {
                if(k!=w.result[i])
                {
                    std::cout<<"result wrong 错误："<<k<<"="<<w.result[i]<<"\n";
                    return std::chrono::duration<double,std::milli>(endNow-now);
                }
            }
            std::cout<<"result correct 正确"<<"\n";
            return std::chrono::duration<double,std::milli>(endNow-now);
        }
    }
}

