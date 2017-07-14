#include "mbed.h"
#include "rtos.h"
#include "string.h"
#include <stdio.h>
#include <ctype.h>

#define SWITCH1_RELEASE 1

void thread1();
void thread2();
void switchISR();

//Digital outputs
DigitalOut onBoardLED(LED1);
DigitalOut redLED(D7);
DigitalOut yellowLED(D6);
DigitalOut greenLED(D5);

//Serial Interface
Serial pc(USBTX, USBRX);

//Digital inputs
DigitalIn  onBoardSwitch(USER_BUTTON);
DigitalIn  sw1(D4);                         //CONSIDER CHANGING THIS TO AN INTERRUPT
DigitalIn  sw2(D3);

//Threads
Thread *t1;
Thread *t2;

//Thread ID for the Main function (CMSIS API)
osThreadId tidMain;
osThreadId tid1;
osThreadId tid2;


//TBD: Call this on the falling edge of SW1
void switchISR() {
     //TBD
}

//High priority thread
void thread1() 
{
    redLED = 1;
    while (true) {
         
         // THIS IS BAD BAD BAD BAD BAD BAD
         // BLOCK ON FALLING EDGE OF SW1 BY RAPID POLLING (SPINNING)
         // Thread is blocked in the RUNNING state 
         while (sw1 == 0);
         wait_ms(200);      //Wait for debounce
         while (sw1 == 1);
         // TODO: FIX THIS! GET THE INTERRUPT TO SIGNAL THIS THREAD
         
         redLED = !redLED;
         Thread::wait(1000);    //Thread in WAITING state
         redLED = !redLED;
         Thread::wait(1000);    //Thread in WAITING state
    }
}

// This thread has normal priority 
// It is supposed to flash the green LED every second.
// THIS IS NOT WORKING because it is currently being starved by thread1 (while polling the switch)
void thread2() 
{
    greenLED = 1; 
    while (true) {   
        Thread::wait(500);
        greenLED = !greenLED;
    }
}


// Main thread
int main() {
    redLED    = 0;
    yellowLED = 0;
    greenLED  = 0;
           
    //Threads - created dynamically with new
    t1 = new Thread(osPriorityRealtime);    //HIGH PRIORITY
    t2 = new Thread(osPriorityNormal);              
         
    //Note t1 and t2 are pointers (32 bit addresses), so the arrow -> is used
    t1->start(thread1);
    t2->start(thread2);    
          
    // Thread IDs
    tidMain = Thread::gettid();  
    tid1    = t1->gettid();
    tid2    = t2->gettid();
    
    //TBD: Hook up interrupt    
        
    pc.printf("Main Thread\n");
    while (true) {
        Thread::wait(osWaitForever);
    }
}

   
