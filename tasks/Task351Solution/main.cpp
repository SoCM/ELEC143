#include "mbed.h"

BusOut binaryOutput(D5, D6, D7);

/*
 ***************************************************
Solution - use the post-decrement operator on iCount
 ***************************************************
 */

int main() {
    
    int iCount = 7;
    
    //Repeat this program forever
    while(1) {
        
        do {
            binaryOutput = iCount--;  //Write decimal to the output and decrement
            wait(1.00);             //Delay for 500ms
        } while (iCount >= 0);      //Condition to repeat
        
        //Reset the count
        iCount = 7;
    }
}