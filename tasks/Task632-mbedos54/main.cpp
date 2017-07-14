#include "mbed.h"
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

//Class type
class message_t {
public:    
    float    adcValue;    
    int sw1State;
    int sw2State;
    
    //Constructor
    message_t(float f, int s1, int s2) {
        adcValue = f;
        sw1State = s1;
        sw2State = s2;    
    }
};
 
//Memory Pool - with capacity for 16 message_t types
MemoryPool<message_t, 16> mpool;

//Message queue - matched to the memory pool
Queue<message_t, 16> queue;

// Call this on precise intervals
void adcISR() {
    
    //Read sample - make a copy
    float sample = adcIn;
    //Grab switch state
    uint32_t switch1State = sw1;
    uint32_t switch2State = sw2;
    
    //Allocate a block from the memory pool
    message_t *message = mpool.alloc();
    if (message == NULL) {
        //Out of memory
        redLED = 1;
        return;   
    }
    
    //Fill in the data
    message->adcValue = sample;
    message->sw1State = switch1State;
    message->sw2State = switch2State;
    
    //Write to queue
    osStatus stat = queue.put(message);    //Note we are sending the "pointer"
    
    //Check if succesful
    if (stat == osErrorResource) {
        redLED = 1; 
        printf("queue->put() Error code: %4Xh, Resource not available\r\n", stat);   
        mpool.free(message);
        return;
    }
}

//Normal priority thread (consumer)
void thread1() 
{      
    while (true) {
        //Block on the queue
        osEvent evt = queue.get();
        
        //Check status
        if (evt.status == osEventMessage) {
            message_t *pMessage = (message_t*)evt.value.p;  //This is the pointer (address)
            //Make a copy
            message_t msg(pMessage->adcValue, pMessage->sw1State, pMessage->sw2State);
            //We are done with this, so give back the memory to the pool
            mpool.free(pMessage);
            
            //Echo to the terminal
            printf("ADC Value: %.2f\t",    msg.adcValue);
            printf("SW1: %u\t",             msg.sw1State);
            printf("SW2: %u\n\r",             msg.sw2State);
        }

             
    } //end while
}


// Main thread
int main() {
    redLED    = 0;
    yellowLED = 0;
    greenLED  = 0;
           
    //Start message
    printf("Welcome\n");           
   
    //Hook up timer interrupt   
    Ticker timer; 
    timer.attach(&adcISR, 0.1);
               
    //Threads
    t1 = new Thread(); 
    t1->start(thread1);
    
    printf("Main Thread\n");
    while (true) {
        Thread::wait(5000);
        puts("Main Thread Alive");
    }
}

   
