#include "mbed.h"

//Global objects
BusOut binaryOutput(D5, D6, D7);    //Outputs as an integer
BusIn binaryInput(D3, D4);          //Inputs as an integer


//Main function
int main() {

    //Create a variable to hold the bit pattern
    unsigned int u = 7;

    while(1) {
                 
        binaryOutput = u;    //Write to LEDs
           
        //TOGGLE all 3 bits in u
        u = u ^ 7;
           
        //Calculate the delay     
        double delay = (double)(binaryInput+1);
        wait(delay * 0.25);          //Wait
            
    } //end while(1)
} //end main
