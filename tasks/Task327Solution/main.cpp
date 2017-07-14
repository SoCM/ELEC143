#include "mbed.h"

// There is a LOT of code repetition here.
// Following bad practise, you can almost copy and paste the code for the red LED to add the greedn
// Further task: Consider ways to reduce this to make it simpler to maintain.

void sw1TimeOutHandler();
void sw1RisingEdge();
void sw1FallingEdge();
void sw2TimeOutHandler();
void sw2RisingEdge();
void sw2FallingEdge();

#define EDGE_RISEN 1
#define EDGE_FALLEN 0

//Global Objects
DigitalOut  red_led(D7);
DigitalOut  green_led(D5);
InterruptIn   sw1(D4);
InterruptIn   sw2(D3);

Timeout sw1TimeOut;             //Used to prevent switch bounce
Timeout sw2TimeOut;
int sw1State = EDGE_FALLEN;     //Initial state for switch 1
int sw2State = EDGE_FALLEN;

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

void sw2TimeOutHandler() {
    sw2TimeOut.detach();            //Stop the timeout counter firing

    //Which event does this follow?
    switch (sw2State) {
    case EDGE_RISEN:    
        sw2.fall(&sw2FallingEdge);  //Now wait for a falling edge
        break;
    case EDGE_FALLEN:
        sw2.rise(&sw2RisingEdge);   //Now wait for a rising edge
        break;
    } //end switch 
}

//Interrupt service routine for a rising edge (press)
void sw2RisingEdge() {
    sw2.rise(NULL);             //Disable detecting more rising edges
    sw2State = EDGE_RISEN;      //Flag state
    sw2TimeOut.attach(&sw2TimeOutHandler, 0.2);    //Start timeout timer
}

//Interrupt service routive for SW1 falling edge (release)
void sw2FallingEdge() {
    sw2.fall(NULL);                         //Disable this interrupt
    green_led = !green_led;                //Toggle LED    
    sw2State = EDGE_FALLEN;                 //Flag state
    sw2TimeOut.attach(&sw2TimeOutHandler, 0.2);    //Start timeout counter    
}

//Main - only has to initialise and sleep
int main() {
    //Initial logging message
    puts("START");
    
    //Initial state
    red_led     = 0; //Set RED LED to OFF
    green_led   = 0;
    
    //Configure interrupts, wait for first rising edge
    sw1.rise(&sw1RisingEdge);
    sw2.rise(&sw2RisingEdge);
    
    //Main Polling Loop
    while (true) {
        
        //Put CPU back to sleep
        sleep();
        
        //You can ONLY reach this point if an ISR wakes the CPU
        puts("ISR just woke the MPU");
        
    } //end while

}
