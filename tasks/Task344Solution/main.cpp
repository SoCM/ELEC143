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
        
        //At this point, the output is binary 100
        //Change to 010
        binaryOutput = 2;   //The missing yellow state
        wait(0.25); 
        
        //The sequence can now repeat
    } //end while(1)
} //end main
