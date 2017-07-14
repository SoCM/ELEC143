//This is known as a “header file”
//In short, this copies and pastes the text file
//mbed.h into this code
#include "mbed.h"

#define kRED    1  
#define kYELLOW 2
#define kGREEN  4

BusOut binaryOutput(D7, D6, D5);    //Outputs as an integer

//The main function - all executable C / C++
//applications have a main function. This is
//out entry point in the software
Ticker T;

int main() {

    binaryOutput = 0;

// ALL the code is contained in a 
// “while loop"

// THIS IS NOT AN IDEAL SOLUTION. HOWEVER IT IS SIMPLE


    while(1) 
    {
    //The code between the { curly braces }
    //is the code that is repeated
    
        //STATE 1 (R) 
        binaryOutput = kRED;
        wait(1.0);
        
        //STATE 2 (RA)
        binaryOutput = kRED + kYELLOW;
        wait(1.0);
        
        //STATE 3 (G)
        binaryOutput = kGREEN;
        wait(1.0);
        
        //STATES 4-7 (Flashing A)
        binaryOutput = kYELLOW;
        wait(0.25);
        binaryOutput = 0;
        wait(0.25);
        binaryOutput = kYELLOW;
        wait(0.25);
        binaryOutput = 0;
        wait(0.25);
                  
    }
}