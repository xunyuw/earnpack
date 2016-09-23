#
#pragma once
#include "../util/common.h"

using namespace std;
namespace GT06GPS {

struct Record 
{
    string tmStamp;
    float  x;
    float  y;
    UINT16    dir;
    UINT16    v;
    UINT32    route;
    UINT32    seq;
} 

}

/*

collect  \\ GPS
document \\TID_TM-SECTION
{
  "TID": "123456",
  "TM":  "yyyy-mm-dd:hh",
  "RECORD": [
        
  ]
   
}

*/

