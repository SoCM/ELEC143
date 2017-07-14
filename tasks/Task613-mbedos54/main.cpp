#include "mbed.h"

#define RED_ON 1
#define YELLOW_ON 2
#define GREEN_ON 4
#define ALL_ON 7

//Function declarations
void Function1();
void Function2();
void Function3();
void Function4();

//I/O
DigitalOut onBoardLED(LED1);
DigitalOut redLED(D7);
DigitalOut yellowLED(D6);
DigitalOut greenLED(D5);

DigitalIn  onBoardSwitch(USER_BUTTON);
DigitalIn  SW1(D4);
DigitalIn  SW2(D3);

Thread t1;
Thread t2;
Thread t3;
Thread t4;

//Thread ID
osThreadId idMain;
osThreadId id1;
osThreadId id2;
osThreadId id3;
osThreadId id4;

void Function1()
{
    while (true) {
        redLED = !redLED;
        if (redLED == 1) {
            t4.signal_set(RED_ON);
        }
        
        Thread::wait(5100);
    }
}

void Function2()
{
    while (true) {
        yellowLED = !yellowLED;
        if (yellowLED == 1) {
            t4.signal_set(YELLOW_ON);
        }
        Thread::wait(1900);
    }
}

//Green Flashing
void Function3()
{
    while (true) {
        greenLED = !greenLED;
        if (greenLED == 1) {
            t4.signal_set(GREEN_ON);
        }       
        Thread::wait(700);
    }
}

//This function waits for signals from all other threads
void Function4()
{
    while (true) {
        
        Thread::signal_wait(RED_ON);
        printf("Red\n");
        Thread::signal_wait(YELLOW_ON);
        printf("Yellow\n"); 
        Thread::signal_wait(GREEN_ON);
        printf("Green\n");
        //Signal main thread       
        osSignalSet(idMain, ALL_ON);              
    }
}

//Main thread
int main() {
    redLED    = 1;
    yellowLED = 0;
    greenLED  = 0;
    
    //Main thread ID
    idMain = osThreadGetId();   //CMSIS RTOS call
    
    //Run threads
    t4.start(Function4);
    t1.start(Function1);           
    t2.start(Function2);    
    t3.start(Function3);     

    
    //Thread ID
    id1 = t1.gettid();
    id2 = t2.gettid();
    id3 = t3.gettid();
    id4 = t4.gettid();
    
    while(1) {
        //Wait for the ALL_ON signal
        osSignalWait(ALL_ON,osWaitForever);
        printf("ALL\n");
    }
}
