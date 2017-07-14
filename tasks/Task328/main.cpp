#include "mbed.h"
#define N 100000
#define RELEASED 1
#define PRESSED  0
DigitalOut red_led(D7);     //CountUp is in its critical section
DigitalOut yellow_led(D6);  //CountDown is in its critical section
DigitalOut green_led(D5);   //counter != 0
DigitalIn button(USER_BUTTON);

//Shared mutable state
volatile long long counter = 0; //Volatile means it must be stored in memory

//Increment the shared variable 
void countUp()
{
    //RED MEANS THE COUNT UP FUNCTION IS IN ITS CRITICAL SECTION
    red_led = 1;
    for (int n=0; n<N; n++) {
        counter++; 
        counter++;
        counter++;
        counter++;
        counter++;
        counter++;
        counter++;
        counter++;
        counter++;
        counter++;           
    }  
    red_led = 0; 
    
    //Last to finish turnes out the green light
    if (counter == 0) {
        green_led = 0;   
    }
}

//Decrement the shared variable
void countDown()
{
    //YELLOW MEANS THE COUNT DOWN FUNCTION IS IN ITS CRITICAL SECTION
    yellow_led = 1;
    for (int n=0; n<N; n++) {
        counter--;
        counter--;
        counter--;
        counter--;
        counter--;
        counter--;
        counter--;
        counter--;
        counter--;
        counter--;           
    }
    yellow_led = 0;
      
    //Last to finish turns out the green light  
    if (counter == 0) {
        green_led = 0;   
    }     
}
int main() {
    
    green_led = 1;
    Timeout t1;
    
    // TRY EACH OF THESE LINES IN TURN.
    // ALL IT DOES IS CHANGE THE TIMING OF THE ISR, NOT THE FUNCTION

    if (button == PRESSED) {
        //VERSION 2: short delay allowing main to be preempted - you might want to tweak this value
        t1.attach_us(&countDown, 15);
    } else {
        //VERSION 1: 2s - ENOUGH TIME FOR COUNTUP TO FINISH
        t1.attach(&countDown, 2);                   
    }
    
    //Run count up on the main thread
    countUp();
    
    //Now spin-lock for ever
    while(1) { 
        asm("nop");
    };
}

