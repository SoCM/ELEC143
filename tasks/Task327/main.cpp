#include "mbed.h"

void sw1TimeOutHandler();
void sw1RisingEdge();
void sw1FallingEdge();


#define EDGE_RISEN 1
#define EDGE_FALLEN 0

//Global Objects
DigitalOut  red_led(D7);
DigitalOut  green_led(D5);
InterruptIn   sw1(D4);
InterruptIn   sw2(D3);

Timeout sw1TimeOut;             //Used to prevent switch bounce
int sw1State = EDGE_FALLEN;     //Initial state for switch 1

//Interrupt service routine for handling the timeout
void sw1TimeOutHandler() {
    sw1TimeOut.detach();            //Stop the timeout counter firing

    //Which event does this follow?
    switch (sw1State) {
    case EDGE_RISEN:    
        sw1.fall(&sw1FallingEdge);  //Now wait for a falling edge
        break;
    case EDGE_FALLEN:
        sw1.rise(&sw1RisingEdge);   //Now wait for a rising edge
        break;
    } //end switch 
}

//Interrupt service routine for a rising edge (press)
void sw1RisingEdge() {
    sw1.rise(NULL);             //Disable detecting more rising edges
    sw1State = EDGE_RISEN;      //Flag state
    sw1TimeOut.attach(&sw1TimeOutHandler, 0.2);    //Start timeout timer
}

//Interrupt service routive for SW1 falling edge (release)
void sw1FallingEdge() {
    sw1.fall(NULL);                         //Disable this interrupt
    red_led = !red_led;                     //Toggle LED    
    sw1State = EDGE_FALLEN;                 //Flag state
    sw1TimeOut.attach(&sw1TimeOutHandler, 0.2);    //Start timeout counter    
}

//Main - only has to initialise and sleep
int main() {
    //Initial logging message
    puts("START");
    
    //Initial state
    red_led    = 0; //Set RED LED to OFF
    green_led   = 1;
    
    //Configure interrupts, wait for first rising edge
    sw1.rise(&sw1RisingEdge);
    
    //Main Polling Loop
    while (true) {
        
        //Put CPU back to sleep
        sleep();
        
        //You can ONLY reach this point if an ISR wakes the CPU
        puts("ISR just woke the MPU");
        
    } //end while

}
