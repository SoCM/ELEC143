#include "mbed.h"


//Function declarations
void Function1();
void Function2();

//I/O
DigitalOut onBoardLED(LED1);
DigitalOut redLED(D7);
DigitalOut yellowLED(D6);
DigitalOut greenLED(D5);

DigitalIn  onBoardSwitch(USER_BUTTON);
DigitalIn  SW1(D4);
DigitalIn  SW2(D3);

Thread t1, t2;

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

//Main thread
int main() {
    redLED    = 0;
    yellowLED = 0;
    
    //Create and run threads
    t1.start(Function1);           
    t2.start(Function2);    
    
    while(1) {
        //Thread::wait(osWaitForever);
        Thread::wait(5000);
        printf("Awake\n");  //Should not happen
    }
}
