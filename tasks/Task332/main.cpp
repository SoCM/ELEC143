#include "mbed.h"

BusOut binaryOutput(D5, D6, D7);
//DigitalIn SW1(D4);

int main() {

    unsigned int uiCount = 0;

    while(1) {
        
        do {
            //Write decimal to the output
            binaryOutput = uiCount;
            
            //Increment the count
            uiCount = uiCount + 1;  
            wait(1.0);              //Delay for 1s

        //Condition to repeat
        } while (uiCount < 8);    
                            
        //Reset the count
        uiCount = 0;
        
    } //end while(1)
} //end main
