//This is known as a “header file”
//In short, this copies and pastes the text file
//mbed.h into this code
#include "mbed.h"

//Create a DigitalOut “object” called myled
//Pass constant D7 as a “parameter”
DigitalOut redLED(D7);
DigitalOut yellowLED(D6);
DigitalOut greenLED(D5);

//The main function - all executable C / C++
//applications have a main function. This is
//out entry point in the software
int main() {

    redLED = 0;
    yellowLED = 0;
    greenLED = 0;

// ALL the code is contained in a 
// “while loop"


    while(1) 
    {
    //The code between the { curly braces }
    //is the code that is repeated
    
        //STATE 1 (R) 
        redLED = 1;
        yellowLED = 0;
        greenLED = 0;       
        wait(1.0);
        
        //STATE 2 (RA)
        yellowLED = 1;
        wait(1.0);
        
        //STATE 3 (G)
        redLED    = 0;
        yellowLED = 0;
        greenLED  = 1;
        wait(1.0);
        
        //STATE 4 (A)
        yellowLED = 1;
        greenLED  = 0;
        wait(1.0);        
    }
}