#pragma once

#ifdef _DEBUG
    #define xassert(except) if(except) { \
        std::abort(); \
        }
#else
    #define xassert(except)  
#endif

#define xstop() int* exception = nullptr \
    *exception = 1;


