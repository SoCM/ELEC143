#include "mbed.h"

#define kRED    (1 << 2)    //4
#define kYELLOW (1 << 1)    //2
#define kGREEN  (1 << 0)    //1
#define kALL (kRED | kYELLOW | kGREEN)

//Global objects
BusOut binaryOutput(D5, D6, D7);    
DigitalIn SW1(D3);
DigitalIn SW2(D4);

AnalogIn POT_ADC_In(A0);
AnalogIn LDD_ADC_In(A1);

float fPOT, fLDR = 0.0;

//Main function
int main() {
    
    while(1) {
        
        //Read ADC
        fPOT = POT_ADC_In;
        fLDR = LDD_ADC_In;
        
        //TODO:
        //Calculate the average of both fPOT and fLDR, then write to the terminal
        
        //Write to terminal
        printf("POT = %6.4f\tLDR = %6.4f\n", fPOT, fLDR);
        
        if (fLDR > fPOT) {
            binaryOutput = 0;      //Binary 000    
        } else {
            binaryOutput = kALL;   //Binary 111
        }
        
        //Wait
        wait(0.1);
        
    } //end while(1)
} //end main