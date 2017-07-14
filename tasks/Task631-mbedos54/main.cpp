#include "mbed.h"
#include "rtos.h"
#include "string.h"
#include <stdio.h>
#include <ctype.h>

#define SWITCH1_RELEASE 1

void thread1();
void thread2();
void switchISR();

//Analogue inputs
AnalogIn adcIn(A0);

//Digital outputs
DigitalOut onBoardLED(LED1);
DigitalOut redLED(D7);
DigitalOut yellowLED(D6);
DigitalOut greenLED(D5);

//Digital inputs
DigitalIn  onBoardSwitch(USER_BUTTON);
DigitalIn  sw1(D4);                         //CONSIDER CHANGING THIS TO AN INTERRUPT
DigitalIn  sw2(D3);

//Threads
Thread *t1;

//Queues - "A message can be a integer or pointer value  to a certain type T that is sent to a thread or interrupt service routine."
Queue<uint32_t, 5> *queue;


// Call this on precise intervals
void adcISR() {
    
    //Option to starve the queue
    if (sw2 == 1) return;
    
    //Read sample - make a copy
    uint32_t sample = (uint32_t)(4095*adcIn.read());
    
    //Write to queue
    osStatus stat = queue->put((uint32_t*)sample);
    
    //Check if succesful
    if (stat == osErrorResource) {
        redLED = 1; 
        printf("queue->put() Error code: %4Xh, Resource not available\r\n", stat);   
    }
    
}

//Normal priority thread (consumer)
void thread1() 
{    
    while (true) {
        //Read queue - block (with timeout)
        osEvent evt = queue->get(5000);     //With timeout
        
        //Check status of get()
        switch (evt.status) { 
            case osEventMessage:
                //Normal status
                printf("value = %d\n\r", evt.value.v);
                greenLED = !greenLED;
                break;
            case osEventTimeout:
                //Timeout
                printf("queue->get() returned %02x status (timeout)\n\r", evt.status);
                break;
            default:
                //All other errors (see cmsis_os.h for meaning of error code)
                printf("queue->get() returned %02x status\n\r", evt.status);
                break;
        }
                
        //Block up consumer if switch is held down
        //Will fill the queue if held long enough
        while (sw1 == 1);
        
    } //end while
}


// Main thread
int main() {
    redLED    = 0;
    yellowLED = 0;
    greenLED  = 0;
           
    //Start message
    printf("Welcome\n");
           
    //Queue
    queue = new Queue<uint32_t,5>();

    //Hook up timer interrupt   
    Ticker timer; 
    timer.attach(&adcISR, 1.0);
               
    //Threads
    t1 = new Thread(); 
    t1->start(thread1);
    
    printf("Main Thread\n");
    while (true) {
        Thread::wait(5000);
        puts("Main Thread Alive");
    }
}

   
