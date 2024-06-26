#include "showing.hpp"
#include <iostream>

int getCountLetters(const std::string& s) {
    const int ex0 = 0xffffffcc, ex1 = 0xffffffa9;
    int count = 0;
    for( auto ch:s) {
        if(((ch&0xC0) == 0x000000C0)  || (ch&0x00000080) != 0x80 )
            count++;
        if(int(ch) == ex1 ){
            count--;
        }
    }
    return  count;
}
