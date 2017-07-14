#include "mbed.h"

#define kRED    4
#define kYELLOW 2
#define kGREEN  1

//Global objects
BusOut binaryOutput(D5, D6, D7);    //Outputs as an integer

DigitalIn SW1(D3);
DigitalIn SW2(D4);

AnalogIn AIN(A0);
float fVin = 0.0;

//Main function
int main() {


    while(1) {
        
        //Read ADC         
        fVin = AIN;
        
        //Write to terminal
        printf("Analog input = %6.4f\n", fVin); //3 decimal places, fieldwidth=5
        
        if (fVin < 0.5f)  {
            binaryOutput = kGREEN;    
        } else {
            binaryOutput = kRED;    
        }
        
        //Wait
        wait(0.5);
            
    } //end while(1)
} //end main