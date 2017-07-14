#include "mbed.h"

// This uses INTERRUPTS to detect a falling edge of the switch input
// However, pressing and releasing the switch can result in spurious falling edges
// which trigger the routine 

//Declare functions
void sw1FallingEdge();

//Global Objects
DigitalOut  red_led(D7);
DigitalOut  green_led(D5);
InterruptIn   sw1(D4);

//Interrupt service routine for a rising edge (press)
void sw1FallingEdge() {
    red_led = !red_led;     //Toggle the LED
}

//Main - only has to initialise and sleep
int main() {
    //Initial logging message
    puts("START");
    red_led   = 0;
    green_led = 1;
    
    //Configure interrupts, wait for first rising edge
    sw1.fall(&sw1FallingEdge);
    
    //Main Polling Loop
    while (true) {
        
        //Put CPU back to sleep
        sleep();
        
        //You can ONLY reach this point if an ISR wakes the CPU
        puts("ISR just woke the MPU");
        
    } //end while

}
