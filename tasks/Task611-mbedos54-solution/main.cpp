#include "mbed.h"

//Function declarations
void Function1(void const *args);
void Function2(void const *args);
void Function3(void const *args);

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

void Function1()
{
    while (true) {
        redLED = !redLED;
        Thread::wait(2000);
    }
}

void Function2()
{
    while (true) {
        yellowLED = !yellowLED;
        Thread::wait(1000);
    }
}

//Green Flashing
void Function3()
{
    while (true) {
        greenLED = !greenLED;
        Thread::wait(500);
    }
}


//Main thread
int main() {
    redLED    = 0;
    yellowLED = 0;
    greenLED  = 0;
    
    //Create and run threads
    t1.start(Function1);           
    t2.start(Function2);    
    t3.start(Function3);     
    
    while(1) {
        //Thread::wait(osWaitForever);
        Thread::wait(5000);
        printf("Awake\n");  //Should not happen
    }
}

