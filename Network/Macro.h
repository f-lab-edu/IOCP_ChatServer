#pragma once
#include <assert.h>

#define xstop() int* exception = nullptr \
    *exception = 1;
