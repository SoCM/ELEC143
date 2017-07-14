#include "mbed.h"
//#include "spi.h"

PwmOut mypwm(PWM_OUT);

//This GPIO is used for Chip Select
DigitalOut DAC_CS(D10);

//SPI Object
SPI spi(D11, D12, D13);

//Ticker for setting the output sampling rate
Ticker t;

//Prototype for the ticker ISR
void writeSample();

int main() {

    //Set speed of the SPI interface
    spi.frequency(20000);
    
    //16 bit words, mode 0 clock
    spi.format(16,0);
    
    //Write at 1000Hz    
    t.attach(writeSample, 0.001);
    
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
    unsigned int val = 2048;
    spi.write(0x7000 | val);
    
    //Disable the selected slave device (and update the output)
    DAC_CS = 1;
}