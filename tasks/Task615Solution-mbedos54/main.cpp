#include "mbed.h"

#define RED_DONE 1
#define YELLOW_DONE 2


//Function declarations
void countUP();
void countDOWN();

//Digital outputs
DigitalOut onBoardLED(LED1);
DigitalOut redLED(D7);
DigitalOut yellowLED(D6);
DigitalOut greenLED(D5);

//Digital inputs
DigitalIn  onBoardSwitch(USER_BUTTON);
DigitalIn  SW1(D4);
DigitalIn  SW2(D3);

//MUTEX Lock
Mutex *countLock;

//Thread ID for the Main function (CMSIS API)
osThreadId tidMain;

//Stared mutable state
volatile long long count = 0;

//Threads
void countUP()
{
    redLED = 1;
    
    for (unsigned int n=0; n<10000; n++) {
        //Take lock
        countLock->lock();
        
        //Critical section(s)
        count++;
        count++;        
        count++;
        count++;    
        count++;
        count++; 
        count++;
        count++;
        count++;
        count++;   
        
        //Release lock
        countLock->unlock();
    }
    
    redLED = 0;
    osSignalSet(tidMain, RED_DONE);  //Signal main thread we are done
}

void countDOWN()
{
    yellowLED = 1;
    
    for (unsigned int n=0; n<10000; n++) {
        //Take lock
        countLock->lock();
        
        //Critical section(s)        
        count--;
        count--;        
        count--;
        count--;    
        count--;
        count--; 
        count--;
        count--;
        count--;
        count--;    
        
        //Release lock
        countLock->unlock();                   
    }   
    
    osSignalSet(tidMain, YELLOW_DONE); //Signal main thread we are done
    yellowLED = 0;
}


//Main thread
int main() {
    Thread t1;           
    Thread t2;        
    
    redLED    = 0;
    yellowLED = 0;
    greenLED  = 1;
    
    //Main thread ID
    tidMain = Thread::gettid();  
    
    //Create lock
    countLock = new Mutex();
    
    //Press the switch to run concurrently
    if (onBoardSwitch == 1) {
        printf("Running sequntially\n");
        countUP();
        countDOWN();        
    } else {
        printf("Running concurrently\n");
        t1.start(countUP);           
        t2.start(countDOWN);    
  
        //Wait for the ALL_ON signal
        Thread::signal_wait(RED_DONE,osWaitForever);
        Thread::signal_wait(YELLOW_DONE,osWaitForever);        
    }

    printf("Final result = %lld\n", count);
    if (count == 0) {
        greenLED = 0;
    }
    
    //Tidy
    delete countLock;
    
    while(true);
}
