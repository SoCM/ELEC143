#include "mbed.h"

#define kRED    4
#define kYELLOW 2
#define kGREEN  1

#define fMargin 0.05f
#define kGYThreshold 0.4f
#define kGYThresholdL (kGYThreshold - fMargin)
#define kGYThresholdU (kGYThreshold + fMargin)

#define kYRThreshold 0.6f
#define kYRThresholdL (kYRThreshold - fMargin)
#define kYRThresholdU (kYRThreshold + fMargin)


//Global objects
BusOut binaryOutput(D5, D6, D7);    //Outputs as an integer

DigitalIn SW1(D3);
DigitalIn SW2(D4);

AnalogIn AIN(A0);
float fVin = 0.0;
unsigned state = kGREEN;

//Main function
int main() {
    
    while(1) {
        
        //Read ADC
        fVin = AIN;
        
        //Write to terminal
        printf("Analog input = %6.4f\n", fVin);
        
        //Next State Logic
        switch (state) {
         
        case kGREEN:
            if (fVin > kGYThresholdU) {
                state = kYELLOW;    
            }
            break;
        
        case kYELLOW:
            if (fVin > kYRThresholdU) {
                state = kRED;   
            } else if (fVin < kGYThresholdL) {
                state = kGREEN;   
            }
            break;
        
        case kRED:
            if (fVin < kYRThresholdL) {
                state = kYELLOW;
            }
            break;
            
        default:
            state = kGREEN;   
        }
        
        //Output logic = f(State)
        binaryOutput = state;   
        
        //Wait
        wait(0.1);
        
    } //end while(1)
} //end main