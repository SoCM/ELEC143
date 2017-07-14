#include "mbed.h"

//Function prototype
void doSample1Hz();

//Global objects
Serial pc(USBTX, USBRX);
AnalogIn POT_ADC_In(A0);
DigitalOut led(LED1);

//Shared variables
volatile static unsigned short sample16 = 0;

//The ticker, used to sample data at a fixed rate
Ticker t;

//Main function
int main()
{
    //Set baud rate to 115200
    pc.baud(115200);

    //Set up the ticker - 100Hz
    t.attach(doSample1Hz, 1.0);

    while(1) {

        //Sleep
        sleep();

        //Displauy the sample in HEX
        pc.printf("ADC Value: %X\n", sample16);
 
    } //end while(1)
} //end main

//ISR for the ticker - simply there to perform sampling
void doSample1Hz()
{
    //Toggle on board led
    led = !led;

    //READ ADC as an unsigned integer.
    //Shift right 4 bits (this is a 12bit ADC) & store in static global variable
    sample16 = POT_ADC_In.read_u16() >> 4;
}

