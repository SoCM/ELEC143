#include "mbed.h"

DigitalOut  red_led(D7);
DigitalOut  yellow_led(D6);
DigitalOut  green_led(D5);
DigitalIn   SW1(D4);
DigitalIn   SW2(D3);

//This is the solution based on the proposed flowchart.

class SwitchState {
    private:
    //Enumerated type (integer)
    enum STATE { WAITING4PRESS=1, WAITING4BOUNCE_RISING=2, WAITING4RELEASE=4, WAITING4BOUNCE_FALLING=8 };
    
    STATE state;        //Current state in the FSM
    DigitalIn& sw;      //Reference to a GPIO Input
    DigitalOut& led;    //Reference to a GPIO Output
    Timer tmr;          //Internal timer used for debounce

    //Toggle the LED state
    void toggleLed() {
        led = !led;
    }
    
    //Next state (circular pattern)
    void gotoNextState() {
        state = (state  >> 3) | ((state << 1) & 0b1111);    
    }
    
    //Constructor    
    public:
    SwitchState(DigitalIn& gpioIn, DigitalOut& gpioOut) : sw(gpioIn), led(gpioOut) {
        state = WAITING4PRESS;
        tmr.stop();
        tmr.reset();
    }
    
    //Update the state using polling
    void updateState()
    {
        switch (state) {
            
        //Waiting for switch to be pressed
        case WAITING4PRESS:

            if (sw == 1) {
                //Output: start timer
                tmr.reset();
                tmr.start();
                
                //Next state
                gotoNextState(); 
                puts("Entering state: WAITING4BOUNCE_RISING");
            } 
            break;
            
        //Waiting for 50ms to elapse
        case WAITING4BOUNCE_RISING:      
            if (tmr.read_ms() > 50) {
                //Outputs: Stop timer
                tmr.stop();
                //Next state
                gotoNextState();  
                puts("Entering state: WAITING4RELEASE");                
            }    
            break;
        
        //Waiting for switch to be released
        case WAITING4RELEASE:
            if (sw == 0) {
                //Outputs: Toggle the LED and start timer
                toggleLed();
                tmr.reset();
                tmr.start();                
                //Next state
                gotoNextState(); 
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
                gotoNextState();
                puts("Entering state: WAITING4PRESS"); 
            }
            break;
            
        default:
            //Something has gone very wrong
            tmr.stop();
            tmr.reset();
            led = 0;
            state = WAITING4PRESS;
            puts("ERROR");
            break;
        
        } //end switch        
        
    }
};


int main() {
    
    //Initial logging message
    puts("START");
    
    //Create two objects using different inputs and outputs
    SwitchState s1(SW1, red_led);
    SwitchState s2(SW2, green_led);

    //Main Polling Loop
    while (true) {
                
        s1.updateState();
        s2.updateState();
                
    } //end while

}


