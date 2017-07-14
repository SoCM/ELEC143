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
InterruptIn  sw1(D4);
DigitalIn    sw2(D3);

//Threads
Thread *t1;
Thread *t2;

//Thread ID for the Main function (CMSIS API)
osThreadId tidMain;
osThreadId tid1;
osThreadId tid2;


//Called on the falling edge of a switch
void switchISR() {
     t1->signal_set(SWITCH1_RELEASE);    //Very short
}

//High priority thread
void thread1() 
{
    redLED = 1;
    while (true) {
         Thread::signal_wait(SWITCH1_RELEASE);
         redLED = !redLED;
         Thread::wait(1000);
         redLED = !redLED;
         Thread::wait(1000);  
         t1->signal_clr(SWITCH1_RELEASE);   //Debounce
    }
}

//This thread has normal priority
void thread2() 
{
    greenLED = 1; 
    while (true) {
        //Thread::wait(2000);    
        Thread::wait(500);          // WAIT FOR 0.5 SECONDS
        greenLED = !greenLED;
    }
}


// Main thread
int main() {
    redLED    = 0;
    yellowLED = 0;
    greenLED  = 0;
           
    //Threads
    t1 = new Thread(osPriorityRealtime);
    t2 = new Thread(osPriorityNormal);              
       
    t1->start(thread1);
    t2->start(thread2);
           
    // Thread IDs
    tidMain = Thread::gettid();  
    tid1    = t1->gettid();
    tid2    = t2->gettid();
    
    //Hook up interrupt
    sw1.fall(switchISR);      
        
    pc.printf("Main Thread\n");
    while (true) {
        Thread::wait(osWaitForever);
    }

}

   
