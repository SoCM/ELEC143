#include "mbed.h"

//Global objects
DigitalOut onboardLed(LED1);
DigitalOut redLED(D7);
DigitalOut yellowLED(D6);
DigitalOut greenLED(D5);

//Function prototypes (ISR)
void doRedPWM();
void doYellowPWM();
void doGreenPWM();

//One-shot timers
Timeout tRed;
Timeout tYellow;
Timeout tGreen;

//Interrupt Service Routine Flags
volatile int redISRFlag = 0;
volatile int yellowISRFlag = 0;
volatile int greenISRFlag = 0;

//ON and OFF times
// RED 9:1
float TRedON =  0.009;
float TRedOFF = 0.001;
// YELLOW 1:1
float TYellowON =  0.001; 
float TYellowOFF = 0.001;
// GREEN 1:9 - nearly off
float TGreenON =  0.001;
float TGreenOFF = 0.009;

int main() {
    //Initialise the LEDs
    redLED = 0;
    yellowLED = 0;
    greenLED = 0;

    
    //Initialise timers (oneshot)
    tRed.attach(doRedPWM, TRedOFF);
    tYellow.attach(doYellowPWM, TYellowOFF);
    tGreen.attach(doGreenPWM, TGreenOFF);
    
    while (1) {
        //Sleep and wait for an interrupt
        sleep();
        
        //Chech which timer(s) went off
        if (redISRFlag == 1) {
            redISRFlag = 0; //Reset ISR flag
            float t = (redLED==0) ? TRedOFF : TRedON;
            tRed.attach(doRedPWM, t);  //Reset timer
        }
        if (yellowISRFlag == 1) {
            yellowISRFlag = 0;
            float t = (yellowLED==0) ? TYellowOFF : TYellowON;
            tYellow.attach(doYellowPWM, t);  
        }
        if (greenISRFlag == 1) {
            greenISRFlag = 0;
            float t = (greenLED==0) ? TGreenOFF : TGreenON;
            tGreen.attach(doGreenPWM, t);  
        }                

    }
}

void doRedPWM()
{
    //Toggle LED
    redLED = !redLED;
    //Flag that interrupt has fired
    redISRFlag = 1;
    
}
void doYellowPWM()
{
    yellowLED = !yellowLED;
    yellowISRFlag = 1;
}
void doGreenPWM()
{
    greenLED = !greenLED;
    greenISRFlag = 1; 
}