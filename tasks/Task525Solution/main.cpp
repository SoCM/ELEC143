#include "mbed.h"

//Global PWM object
PwmOut pwmRed(D7);

InterruptIn SWOn(D4);    //Fade ON
InterruptIn SWOff(D3);    //Fade OFF

int T = 100;    //100uS
volatile int Tmark = 0; //0us
volatile int delta = 1; //1us

//Timer
Ticker t;

//Function prototype 
void doTwinkle();
void doSwitchON();
void doSwitchOFF();

//Flags
volatile int switchONFlag=0;
volatile int switchOFFFlag=0;
volatile int tickerFlag = 0;

int main() {
    //Setup switch interrupts
    SWOn.rise(doSwitchON);
    SWOff.rise(doSwitchOFF);
    
    //Initial PWM state
    pwmRed.period_us(T);
    pwmRed.pulsewidth_us(Tmark);
    
    
    printf("Ready\n");
    
    while(1) {
        sleep();
        
        //Which interrupt occured?
        if (switchONFlag == 1) {
            switchONFlag = 0;   
            delta = 5; 
            t.attach(doTwinkle, 0.1);
            printf("ON button pressed\n");
        }
        else if (switchOFFFlag == 1) {
            switchOFFFlag = 0;
            delta = -5;
            t.attach(doTwinkle, 0.1);
            printf("OFF button pressed\n");
        }
        
        //Has ticker hit the end stop?
        if (tickerFlag == 1) {
            tickerFlag = 0;
            delta = 0;      //Pedantic
            t.detach();     //Turn off ticker
            printf("Timer finished\n"); 
        }
        //Update PWM
        pwmRed.pulsewidth_us(Tmark);
        
        printf("Tmark = %d\n", Tmark);    //Debug
    }
}

void doSwitchON()
{
    switchONFlag=1;  
}

void doSwitchOFF()
{
    switchOFFFlag=1;
}

//ISR for Timer
void doTwinkle()
{
    //Add on delta
    Tmark += delta;
    
    //Cap at extremes
    if (Tmark > 100) {
        Tmark = 100;        
    }
    else if (Tmark < 0) {
        Tmark = 0;
    }
    
    //Check bounds 
    if ((Tmark == 100) || (Tmark == 0)) {
        tickerFlag = 1;
    } 
}