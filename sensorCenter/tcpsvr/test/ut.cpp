#include <iostream>
#include <cstring>
#include "stdio.h"

typedef unsigned char  BYTE;
typedef unsigned short WORD; 
typedef unsigned int   DWORD; 
typedef unsigned int   UINT;

void swapEndian(WORD& v)    
{                            
    BYTE* p = (BYTE*)&v;             
    BYTE t = *(p);       
    *(p) = *(p+1);
    *(p+1) = t;
} 

using namespace std;
int main()
{

    WORD w = (WORD)0xFFA0;
    swapEndian(w);
    printf("new : 0x%x\n", (WORD)w);
    char sentence[]="This is a sentence with 7 tokens";
    cout << "The string to be tokenized is:\n" << sentence << "\n\nThe tokens are:\n\n";
    char *tokenPtr=strtok(sentence, " ");
    while(tokenPtr!=NULL) {
        cout<<tokenPtr<<'\n';
        tokenPtr=strtok(NULL," ");
    }
    //cout << "After strtok,sentence=" << tokenPtr<<endl;

    cout << "sen: " <<  sentence << " \n";
    return 0;
}
