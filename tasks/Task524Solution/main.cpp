#include "mbed.h"

//Global PWM object
PwmOut pwmRed(D7);

int T = 100;    //100uS
volatile int Tmark = 0; //0us
volatile int delta = 1; //1us
int n=0;

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
    //Update n
    n++;
    
    //Calculate t 
    float t = (float)n / (float)T;
    float y = sin(2.0f*3.1415926f*t); //Range -1...+1
    Tmark = (int)(0.5f*(y+1.0f)*T);   //Range 0..T
}