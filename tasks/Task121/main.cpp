//This is known as a “header file”
//In short, this copies and pastes the text file
//mbed.h into this code
#include "mbed.h"

//Create a DigitalOut “object” called myled
//Pass constant D7 as a “parameter”
DigitalOut myled(D7);
//On the F401 board, you can also change this to LED1 if you wish to use the on-board LED

//The main function - all executable C / C++
//applications have a main function. This is
//out entry point in the software
int main() {

// ALL the code is contained in a 
// “while loop”
    while(1) 
    {
    //The code between the { curly braces }
    //is the code that is repeated  
        myled = 1; // External LED is ON
        wait(1.0); // 1 second
        myled = 0; // LED is OFF
        wait(1.0); // External 1 second
    }
}

//updated for mbed os 5.4

