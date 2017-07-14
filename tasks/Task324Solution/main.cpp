#include "mbed.h"

void updateState(int &sw, int &swState, DigitalOut &led, Timer &tmr) ;

DigitalOut  red_led(D7);
DigitalOut  green_led(D5);
DigitalIn   SW1(D4);
DigitalIn   SW2(D3);
//This is the solution based on the proposed flowchart.
//The precise delay required may need adjusting

Timer tmr1;
Timer tmr2;

#define WAITING4PRESS 0
#define WAITING4BOUNCE_RISING 1
#define WAITING4RELEASE 2
#define WAITING4BOUNCE_FALLING 4

int main() {
    //Initial logging message
    puts("START");
    
    //Initial state
    red_led    = 0; //Set RED LED to OFF
    green_led  = 0;
    
    //Switch state
    int sw1State = 0;
    int sw2State = 0;
    
    //Timers
    tmr1.stop();
    tmr1.reset();
    tmr2.stop();
    tmr2.reset();

    //Initial logging message
    puts("Entering state WAITING4PRESS");

    //Main Polling Loop
    while (true) {
        
        //Poll inputs (without blocking)
        int sw1 = SW1;
        int sw2 = SW2;
        
        updateState(sw1, sw1State, red_led,   tmr1);
        updateState(sw2, sw2State, green_led, tmr2);
                
    } //end while

}

// Update the state for a given switch and LED pair
// Parameters:
// sw is the switch input, 0 for released and 1 for pressed, passed by value
// swState is an integer that keeps a record of state, passed by reference (updated by the function)
// led is of type DigitalOut, connected to a LED
// tmr is of type Timer, a reference to  unique instance of a timer
// This function cannot be called by an ISR
void updateState(int sw, int &swState, DigitalOut &led, Timer &tmr) 
{
        //LED
        switch (swState) {
            
        //Waiting for switch to be pressed
        case WAITING4PRESS:

            if (sw == 1) {
                //Output: start timer
                tmr.reset();
                tmr.start();
                
                //Next state
                swState =  WAITING4BOUNCE_RISING;  
                puts("Entering state: WAITING4BOUNCE_RISING");
            } 
            break;
            
        //Waiting for 50ms to elapse
        case WAITING4BOUNCE_RISING:      
            if (tmr.read_ms() > 50) {
                //Outputs: Stop timer
                tmr.stop();
                //Next state
                swState =  WAITING4RELEASE;  
                puts("Entering state: WAITING4RELEASE");                
            }    
            break;
        
        //Waiting for switch to be released
        case WAITING4RELEASE:
            if (sw == 0) {
                //Outputs: Toggle the LED and start timer
                led = !led;
                tmr.reset();
                tmr.start();                
                //Next state
                swState =  WAITING4BOUNCE_FALLING;  
                puts("Entering state: WAITING4BOUNCE_FALLING");  
            } 
            break;
            
        //Waiting 50ms for switch bounce         
        case WAITING4BOUNCE_FALLING:
            if (tmr.read_ms() > 50) {
                //Outputs: Reset timer 1
                tmr.stop();
                tmr.reset();
                
                //Next state:
                swState = WAITING4PRESS;
                puts("Entering state: WAITING4PRESS"); 
            }
            break;
            
        default:
            //Something has gone very wrong
            tmr.stop();
            tmr.reset();
            led = 0;
            swState = WAITING4PRESS;
            puts("ERROR");
            break;
        
        } //end switch
    } //end function