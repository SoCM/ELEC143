#include "mbed.h"

//Global objects
BusOut binaryOutput(D5, D6, D7);


//Main function
int main() {

    //Create a variable to hold the bit pattern
    unsigned int u;

    while(1) {
        
        u = 1;             //Set initial value 0
        int count = 0;
        while (count++ < 3) {
           binaryOutput = u;    //Write to LEDs
           u = u << 1;          //Shift left 1 bit
           wait(0.25);          //Wait
        }       
        
        //TODO: Make the pattern shift in the opposite direction
        // (also known as the knight rider pattern)
        
        
    } //end while(1)
} //end main
