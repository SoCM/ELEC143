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

        //Calculate averages
        float fPOTAverage = updateAverage(POT_ADC_In, xPOT);
        float fLDRAverage = updateAverage(LDR_ADC_In, xLDR);
        
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


/** Update and calculate the average. This has not been "hand optimised" yet 
 *
 * @param newSample The new sample to be added to the buffer
 * @param buffer[] The array used to store N samples
 * @returns
 *   average of the samples in the array buffer
 */
float updateAverage(float newSample, float buffer[])
{
    //Local variable - remember to initialise!
    float average = 0.0;
    
    //Move all samples to the right
    for (unsigned int n=(N-1); n>0; n--) {
        buffer[n] = buffer[n-1];    
    }    
    
    //Insert new sample
    buffer[0] = newSample;
    
    //Calculate average
    for (unsigned int n=0; n<N; n++) {
        average += buffer[n];    
    }
    average /= (float)N;
    
    //Return the result
    return average;
}