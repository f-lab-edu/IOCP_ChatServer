#pragma once

#ifdef _DEBUG
#define Assert(condition) if(condition){ \
    int* a = nullptr; \
    *a = 1; \
        }
#else
    #define Assert(condition)
#endif 
    
        