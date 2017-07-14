#include "mbed.h"

#define RED_TOGGLE    1
#define YELLOW_TOGGLE 2
#define GREEN_TOGGLE  4

//Function declarations
void FunctionRed();
void FunctionYellow();
void FunctionGreen();

//I/O
DigitalOut onBoardLED(LED1);
DigitalOut redLED(D7);
DigitalOut yellowLED(D6);
DigitalOut greenLED(D5);

DigitalIn  onBoardSwitch(USER_BUTTON);
DigitalIn  SW1(D4);
DigitalIn  SW2(D3);


//Each of the following 3 functions is listening for a signal
void FunctionRed()
{
    while (true) {
        Thread::signal_wait(RED_TOGGLE);  
        redLED = !redLED;                   
    }
}

void FunctionYellow()
{
    while (true) {
        Thread::signal_wait(YELLOW_TOGGLE);
        yellowLED = !yellowLED;
    }
}

void FunctionGreen()
{
    while (true) {
        Thread::signal_wait(GREEN_TOGGLE);
        greenLED = !greenLED;
    }
}

//Main thread
int main() {
    redLED    = 0;
    yellowLED = 0;
    greenLED  = 0;
        
    //Create threads
    Thread t1;           
    Thread t2;    
    Thread t3;   
    
    //Start threads
    t1.start(FunctionRed);           
    t2.start(FunctionYellow);    
    t3.start(FunctionGreen);      
    
    //Main loop
    while(1) {
        
        //Read stdin (serial port)
        int selection, hits;
        char strInput[64];
        do {
            puts("Please choose:");
            puts("1 - Red");
            puts("2 - Yellow");
            puts("3 - Green");            
            scanf("%64s", strInput);                    //Read a string
            hits = sscanf(strInput, "%d", &selection);  //Look for an integer
        } while (hits != 1);                            //Repeat if not found
        
        //Signal the thread
        switch (selection) {
        case 1:
            //Signal thread 1
            t1.signal_set(RED_TOGGLE);
            break;
        case 2:
            //Signal thread 2
            t2.signal_set(YELLOW_TOGGLE);
            break;
        case 3:
            //Signal thread 3
            t3.signal_set(GREEN_TOGGLE);
            break;
        default:
            puts("Invalid option");
            break;
        } //end switch
        
    } //end while
} //end main
