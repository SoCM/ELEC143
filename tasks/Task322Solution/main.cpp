#include "mbed.h"

DigitalOut  red_led(D7);
DigitalOut  yellow_led(D6);
DigitalOut  green_led(D5);
DigitalIn   SW1(D4);

//This is the solution based on the proposed flowchart.
//It is flawed however (see next exercise) due to
//switch bounce.

int main() {

    //Switch on Yellow and Green to indicate
    //that the code is running
    yellow_led = 1;
    green_led = 1;
    red_led = 0; //Set RED LED to OFF
    
    // Wait for SW1 to be pressed
    while (SW1 == 0) { }
    
    // Wait for SW1 to be released
    while (SW1 == 1) { }
    
    red_led = 1;    //Turn ON LED
    
    while (1) { }       //Repeat forever
}