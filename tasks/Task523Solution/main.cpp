#include "mbed.h"

//Global PWM object
PwmOut pwmRed(D7);

int T = 100;    //100uS
volatile int Tmark = 0; //0us
volatile int delta = 1; //1us

//Timer
Ticker t;

//Function prototype 
void doTwinkle();

int main() {
    
    //Initial PWM state
    pwmRed.period_us(T);
    pwmRed.pulsewidth_us(Tmark);
    
    //Start timer
    t.attach(doTwinkle, 0.01);
    
    while(1) {
        sleep();
        
        //Update PWM
        pwmRed.pulsewidth_us(Tmark);
        
        //printf("Tmark = %d\n", Tmark);    //Debug
    }
}

//ISR for Timer
void doTwinkle()
{
    //Update mark time
    Tmark += delta;
    
    //Check bounds - and swap direction
    if ((Tmark >= 99) || (Tmark <= 0)) {
        delta = -delta;
    }
}