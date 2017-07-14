#include "mbed.h"

//Function prototype
void doSample1Hz();
void printBinary12(unsigned short u16);

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
        printBinary12(sample16);
 
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

//Function to print a 12-bit number in the binary format
//(I've kept this simple to avoid confusion)
void printBinary12(unsigned short u16)
{
    unsigned int mask = 0x800; //Binary 1000000000000000
    for (unsigned int n=0; n<12; n++) {
        //Test to see if the bit is set
        if (u16 & mask) {
            pc.printf("1");
        } else {
            pc.printf("0");   
        }
        //Shift mask 1 bit right
        mask >>= 1;
    }
    pc.printf("\n"); 
}
