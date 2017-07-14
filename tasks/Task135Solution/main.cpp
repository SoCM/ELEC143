//This is known as a “header file”
//In short, this copies and pastes the text file
//mbed.h into this code
#include "mbed.h"

#define kRED    1  
#define kYELLOW 2
#define kGREEN  4

BusOut binaryOutput(D7, D6, D5);    //Outputs as an integer

unsigned int state = 0;
void toggleYellow()
{
    if (state == 0) {
        binaryOutput = kYELLOW;
        state = 1;
    } else {
        binaryOutput = 0;
        state = 0;
    }
}

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
        wait(4.0);
        
        //STATE 2 (RA)
        binaryOutput = kRED + kYELLOW;
        wait(4.0);
        
        //STATE 3 (G)
        binaryOutput = kGREEN;
        wait(4.0);
        
        //STATE 4 (Flashing A)
        T.attach(toggleYellow, 0.25);
        wait(4.0);
        T.detach();
                  
    }
}