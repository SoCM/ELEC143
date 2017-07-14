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
        
        u = 0;             //Set initial value 0
        binaryOutput = u;  //Show binary on LED's
        waitForButtonPress(); //Call function      
  
        //Use & to toggle bit 0 
        u = u ^ 1;          //XOR with binary 001
        binaryOutput = u;   
        waitForButtonPress();   
                               
        //Toggle bit 1
        //WRITE CODE HERE
        binaryOutput = u;   
        waitForButtonPress();                          


        //Toggle bit 2
        //WRITE CODE HERE
        binaryOutput = u;   
        waitForButtonPress();   
        
        //Toggle bits 0 and 2
        //WRITE CODE HERE
        binaryOutput = u;   
        waitForButtonPress();  
        
        //Toggle all bits
        //WRITE CODE HERE
        binaryOutput = u;   
        waitForButtonPress();
        
        //Toggle all bits
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