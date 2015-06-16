#include<iostream>
#include<sstream>
#include<string>
#include <vector>
#include <iterator>

using namespace std;

vector<string> str_split(const string &s, char delim)      
{     
    stringstream ss(s);     
    string item;     
    vector<string> tokens;     
    while (getline(ss, item, delim))      
    {     
        tokens.push_back(item);     
    }     
    return tokens;     
} 

int main ( int argc, char **argv )
{
    if (argc != 2) return -1;
    
    int *p = NULL;
    cout  << *p << endl;
    string in(argv[1]);
    vector<string> vStr = str_split(in, '&');
    cout << vStr.size();
    std::copy(vStr.begin(), vStr.end(),
             ostream_iterator<string>(std::cout, "\n"));    
}
