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

//Thread sychronisation primatives
Mutex lock1;
Mutex lock2;
unsigned long sw1Count = 0;
unsigned long sw2Count = 0;

void thread1() 
{
    pc.printf("Entering thread 1\n");
    while (true) {
        yellowLED = 1;
        
        //Start critical section
        lock1.lock();
 
        sw1Count++;
        printf("\nCount1 = %lu", sw1Count);
        
        //Thread::wait(1); //1ms
        
        if (SW1 == 1) {
            lock2.lock();
            sw2Count--;
            lock2.unlock();   
        }
        
        //End critical section
        lock1.unlock();
        
        yellowLED = 0;
        Thread::wait(DELAY);       
    }
}

void thread2() 
{
    pc.printf("Entering thread 2\n");  
    while (true) {
        redLED = 1; 
        
        //Start critical section
        lock2.lock();
        
        sw2Count++;
        printf("\nCount2 = %lu", sw2Count);
        
        //Thread::wait(1);  //1ms
        
        if (SW2 == 1) {
            lock1.lock();
            sw1Count--;
            lock1.unlock();  
        }  
        //End critical section
        lock2.unlock();
        
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
    
    //Threads
    Thread t1, t2;
        
    t1.start(thread1);
    t2.start(thread2);
    
    pc.printf("Main Thread\n");
    while (true) {
        Thread::wait(osWaitForever);
    }

}

   
