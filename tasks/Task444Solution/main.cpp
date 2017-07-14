#include "mbed.h"

//Function prototype
void doSample1Hz();

//Global objects
Serial pc(USBTX, USBRX);
AnalogIn POT_ADC_In(A0);
DigitalOut led(LED1);

#define N 16 

//Shared variables
volatile static unsigned short sample16 = 0;

unsigned int sum = 0;
unsigned int count = 0;

//The ticker, used to sample data at a fixed rate
Ticker t;

//Main function
int main()
{
    //Set baud rate to 115200
    pc.baud(115200);

    //Set up the ticker - 100Hz
    t.attach(doSample1Hz, 0.01);

    while(1) {

        //Sleep
        sleep();
        
        //Add new sample to the sum
        sum += sample16;
        
        //Increment the count
        count++;
        
        //Enough to calculate the average?
        if (count == N) {
            //Divide sum by 16
            unsigned short mean = (sum >> 4);   
            
            //Display
            pc.printf("Mean = %hu\n", mean);
            
            //Reset sum and count
            count = sum = 0;
        }
 
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


