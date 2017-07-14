#include "mbed.h"

DigitalOut  red_led(D7);
DigitalOut  green_led(D5);
DigitalIn   SW1(D4);
DigitalIn   SW2(D3);
//This is the solution based on the proposed flowchart.
//The precise delay required may need adjusting

Timer tmr1;

#define WAITING4PRESS 0
#define WAITING4BOUNCE_RISING 1
#define WAITING4RELEASE 2
#define WAITING4BOUNCE_FALLING 4

int main() {
    //Initial logging message
    puts("START");
    
    //Initial state
    red_led    = 0; //Set RED LED to OFF
    green_led  = 1;
    
    //Switch state
    int sw1State = 0;
    
    //Timers
    tmr1.stop();
    tmr1.reset();

    //Initial logging message
    puts("Entering state WAITING4PRESS");

    //Main Polling Loop
    while (true) {
        
        //Poll inputs (without blocking)
        int sw1 = SW1;
        
        //Poll Timers
        int tmr1Count = tmr1.read_ms();
        
        //LED1
        switch (sw1State) {
            
        //Waiting for switch to be pressed
        case WAITING4PRESS:

            if (sw1 == 1) {
                //Output: start timer
                tmr1.reset();
                tmr1.start();
                
                //Next state
                sw1State =  WAITING4BOUNCE_RISING;  
                puts("Entering state: WAITING4BOUNCE_RISING");
            } 
            break;
            
        //Waiting for 50ms to elapse
        case WAITING4BOUNCE_RISING:      
            if (tmr1Count > 50) {
                //Outputs: Stop timer
                tmr1.stop();
                tmr1.reset();
                //Next state
                sw1State =  WAITING4RELEASE;  
                puts("Entering state: WAITING4RELEASE");                
            }    
            break;
        
        //Waiting for switch to be released
        case WAITING4RELEASE:
            if (sw1 == 0) {
                //Outputs: Toggle the LED and start timer
                red_led = !red_led;
                tmr1.start();                
                //Next state
                sw1State =  WAITING4BOUNCE_FALLING;  
                puts("Entering state: WAITING4BOUNCE_FALLING");  
            } 
            break;
            
        //Waiting 50ms for switch bounce         
        case WAITING4BOUNCE_FALLING:
            if (tmr1Count > 50) {
                //Outputs: Reset timer 1
                tmr1.stop();
                tmr1.reset();
                
                //Next state:
                sw1State = WAITING4PRESS;
                puts("Entering state: WAITING4PRESS"); 
            }
            break;
            
        default:
            //Something has gone very wrong
            tmr1.stop();
            tmr1.reset();
            red_led = 0;
            sw1State = WAITING4PRESS;
            puts("ERROR");
            break;
        
        } //end switch
        
        
    } //end while

}