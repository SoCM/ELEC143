#include "mbed.h"

#define Fs 1000
#define T (1.0/Fs)
#define PI 3.1415926541

PwmOut mypwm(PWM_OUT);

//This GPIO is used for Chip Select
DigitalOut DAC_CS(D10);

//SPI Object
SPI spi(D11, D12, D13);

//Ticker for setting the output sampling rate
Ticker t;

//Discrete time
unsigned int n=0;

//Frequency f
double f = 10.0;

//Prototype for the ticker ISR
void writeSample();

int main() {

    //Set speed of the SPI interface
    spi.frequency(20000);
    
    //16 bit words, mode 0 clock
    spi.format(16,0);
    
    //Write at Fs Hz    
    t.attach(writeSample, T);
    
    while(1) {
        sleep();
    }
}

//ISR for ticker
void writeSample()
{
    //Enable the selected slave device
    DAC_CS = 0;
    
    //Write a header (top 4 bits) and value (bottom 12 bits) 
    double y = cos(2.0*PI*n*f*T);
    unsigned int val = (unsigned int)(2048.0 * (y + 1.0));
    spi.write(0x7000 | val);
    
    //Disable the selected slave device (and update the output)
    DAC_CS = 1;
    
    n++;
}