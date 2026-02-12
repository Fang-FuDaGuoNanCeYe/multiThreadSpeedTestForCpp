#pragma once
#ifndef loops
#define loops 1
#endif
namespace Fang::Test
{
    unsigned long long doSomething(unsigned long long a)
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
}