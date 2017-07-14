#include "mbed.h"

BusOut binaryOutput(D5, D6, D7);
//DigitalIn SW1(D4);

int main() {

    int iCount = 7;

    while(1) {
        
        do {
            //Write decimal to the output
            binaryOutput = iCount;
            
            //Increment the count
            iCount = iCount - 1;  
            wait(1.0);              //Delay for 1s

        //Condition to repeat
        } while (iCount >= 0);    
                            
        //Reset the count
        iCount = 7;
        
    } //end while(1)
} //end main
