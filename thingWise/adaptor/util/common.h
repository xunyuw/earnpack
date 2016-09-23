//


#pragma once

#if TRACE
  #undef TRACE
  #define TRACE(a) std::cout << (a) << std::endl;
#else
  #define TRACE(a)
#endif


typedef char INT8;
typedef short INT16;
typedef unsigned short UINT16;
typedef int INT32;
typedef unsigned int UINT32;

typedef bool BOOL;
#define TRUE 1
#define FALSE 0 

