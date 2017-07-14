#include "mbed.h"

//Global objects
BusOut binaryOutput(D5, D6, D7);
DigitalIn SW1(D4);

//Function prototypes
void waitForButtonPress();

//Main function
int main() {

    //Create a variable to hold the bit pattern
    unsigned int u;

    //Flash LED's to indicate the code is running
    binaryOutput = 7;
    wait(0.5);
    binaryOutput = 0;

    while(1) {
        
        u = 0;             //Set initial value
        binaryOutput = u;  //Show binary on LED's
        waitForButtonPress(); //Call function      
  
        //Here is the first - use | to set bit 1 
        u = u | 2;      //OR with binary 010
        binaryOutput = u;   
        waitForButtonPress();   
                               
        //Modify u with the | to set bit 2 to a 1
        u = u | 4;
        binaryOutput = u;   
        waitForButtonPress();                          


        //Modify u with the | to set bit 0 to a 1
        u = u | 1;
        binaryOutput = u;   
        waitForButtonPress();   
        
    } //end while(1)
} //end main


void waitForButtonPress() {
    while (SW1 == 0);
    wait(0.25);
    while (SW1 == 1);
}