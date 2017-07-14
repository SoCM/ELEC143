#include "mbed.h"

//Global objects
BusOut binaryOutput(D5, D6, D7);    
AnalogIn POT_ADC_In(A0);
AnalogIn LDD_ADC_In(A1);

float fPOT, fLDR = 0.0;
float fPOTAverage = 0.0, fLDRAverage = 0.0;
unsigned int n = 0;
unsigned int N = 10;

//Main function
int main() {
    
    while(1) {
        
        //Read ADC
        fPOT = POT_ADC_In;
        fLDR = LDD_ADC_In;
        
        //Calculate the average of both fPOT and fLDR
        fPOTAverage += fPOT;
        fLDRAverage += fLDR;
        
        //Update the sample count
        n++;
        
        //Wait
        wait(0.01);        
        
        //Do we have N samples?
        if (n == N) {
            
            //Calculate average
            fPOTAverage /= (float)N;
            fLDRAverage /= (float)N;
            
            //Write to terminal
            printf("POT = %6.4f\tLDR = %6.4f\n", fPOTAverage, fLDRAverage);
        
            //Check the threshold
            if (fLDRAverage > fPOTAverage) {
                binaryOutput = 0;   //Binary 000    
            } else {
                binaryOutput = 7;   //Binary 111
            }  
            
            //RESET averages and sample count
            n = 0;
            fPOTAverage = 0.0;
            fLDRAverage = 0.0;        
        }

        
    } //end while(1)
} //end main