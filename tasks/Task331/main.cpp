#include "mbed.h"

//lsb first
BusOut binaryOutput(D5, D6, D7);

int main() {

    //Try different values
    binaryOutput = 3;
    
    while (1) { }
}