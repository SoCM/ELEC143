#include "mbed.h"

//Constants
#define N 10

//Function Prototype
float updateAverage(float newSample, float buffer[]);

//Global objects
Serial pc(USBTX, USBRX);

BusOut binaryOutput(D5, D6, D7);
AnalogIn POT_ADC_In(A0);
AnalogIn LDR_ADC_In(A1);

float xPOT[N]; //Aray of potentiometer samples
float xLDR[N]; //Aray of LDR samples

//Main function
int main()
{

    //Set baud rate to 115200
    pc.baud(115200);

    //Print header
    pc.printf("POT,LDR,avPOT, acLDR\n\n");

    while(1) {

        //Move the samples in both arrays to the right
        for (unsigned int n=(N-1); n>0; n--) {
            xPOT[n] = xPOT[n-1];
            xLDR[n] = xLDR[n-1];
        }

        //Insert the new sample at position 0
        xPOT[0] = POT_ADC_In;
        xLDR[0] = LDR_ADC_In;

        //Calculate average for POT
        float fSum = 0.0;
        for (unsigned int n=0; n<N; n++) {
            fSum += xPOT[n];
        }
        float fPOTAverage = fSum / (float)N;

        //Calculate average for LDR
        fSum = 0.0;
        for (unsigned int n=0; n<N; n++) {
            fSum += xLDR[n];
        }
        float fLDRAverage = fSum / (float)N;

        //Write to terminal via Serial interface
        pc.printf("%6.4f,%6.4f,%6.4f,%6.4f\n", xPOT[N/2], xLDR[N/2], fPOTAverage, fLDRAverage);

        //Check the threshold
        if (fLDRAverage > fPOTAverage) {
            binaryOutput = 0;   //Binary 000
        } else {
            binaryOutput = 7;   //Binary 111
        }

        //Wait 1/100th second
        wait(0.01);


    } //end while(1)
} //end main


/*
    First version of the updateAverage function
    TODO
*/
float updateAverage(float newSample, float buffer[])
{
    //Local variable - remember to initialise!
    float average = 0.0;
    
    //TODO: Move all samples to the right
 
    
    //TODO: Insert new sample

    
    //TODO: Calculate average
    
    //Return the result
    return average;
}