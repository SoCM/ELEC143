#include "mbed.h"

#define kRED    (1 << 2)    //4
#define kYELLOW (1 << 1)    //2
#define kGREEN  (1 << 0)    //1

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
        unsigned op = 0u;
        
        //Read ADC
        fPOT = POT_ADC_In;  //Threshold
        fLDR = LDD_ADC_In;  //Light reading
        
        //Write to terminal
        printf("POT = %6.4f\tLDR = %6.4f\n", fPOT, fLDR);
        
        if (fLDR < 0.1f) {
            op = kRED | kYELLOW | kGREEN;    
        } else if (fLDR < 0.15f) {
            op = kYELLOW | kRED;     
        } else if (fLDR < 0.2f) {
            op = kRED;
        } else {
            op = 0;    
        }

        //Write to port
        binaryOutput = op;
        
        //Wait
        wait(0.1);
        
    } //end while(1)
} //end main