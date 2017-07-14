#include "mbed.h"
#include "rtos.h"
#include "string.h"
#include <stdio.h>
#include <ctype.h>

#define DELAY 200

//Digital outputs
DigitalOut onBoardLED(LED1);
DigitalOut redLED(D7);
DigitalOut yellowLED(D6);
DigitalOut greenLED(D5);

//Serial Interface
Serial pc(USBTX, USBRX);

//Digital inputs
DigitalIn  onBoardSwitch(USER_BUTTON);
DigitalIn  SW1(D4);
DigitalIn  SW2(D3);

//Thread ID for the Main function (CMSIS API)
osThreadId tidMain;

void thread1() 
{
    pc.printf("Entering thread 1\n");
    while (true) {
        yellowLED = 1;
        Thread::wait(DELAY);   
        yellowLED = 0;
        Thread::wait(DELAY);               
    }
}

//This thread has higher priority
void thread2() 
{
    pc.printf("Entering thread 2\n");  
    while (true) {
        redLED = 1;
        if (SW1 == 1) {
            
            // 1) Select the 'type' of wait
            
            //wait_ms(osWaitForever);
            Thread::wait(osWaitForever); 
        } else {
            Thread::wait(DELAY);    
        }
        redLED = 0;
        Thread::wait(DELAY);               
    }
}


//Main thread
int main() {
    redLED    = 0;
    yellowLED = 0;
    greenLED  = 0;
                
    //Main thread ID
    tidMain = Thread::gettid();  
    
    Thread t1(osPriorityNormal);
    t1.start(thread1);
    
    // 2) Select the Thread Priority
    
    //Thread t2(thread2, NULL, osPriorityNormal);
    Thread t2(osPriorityAboveNormal);
    t2.start(thread2);
    
    pc.printf("Main Thread\n");
    while (true) {
        Thread::wait(osWaitForever);
    }

}

   
