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
        
        u = 7;             //Set initial value 111
        binaryOutput = u;  //Show binary on LED's
        waitForButtonPress(); //Call function      
  
        //Use & to reset bit 1 
        u = u & 5;          //AND with binary 101
        binaryOutput = u;   
        waitForButtonPress();   
                               
        //Modify u with & to reset bit 2 to a 0
        //WRITE CODE HERE
        binaryOutput = u;   
        waitForButtonPress();                          


        //Modify u with & to reset bit 0 to a 0
        //WRITE CODE HERE
        binaryOutput = u;   
        waitForButtonPress();   
        
    } //end while(1)
} //end main


void waitForButtonPress() {
    while (SW1 == 0);
    wait(0.25);
    while (SW1 == 1);
}