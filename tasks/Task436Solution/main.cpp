#include "mbed.h"

//Constants
#define N 10

//Function Prototype
void doCaptureSamples();
float updateAverage(float newSample, float buffer[]);

//Global objects
Serial pc(USBTX, USBRX);

BusOut binaryOutput(D5, D6, D7);
AnalogIn POT_ADC_In(A0);
AnalogIn LDR_ADC_In(A1);
DigitalOut strobe(D8);

float xPOT[N]; //Aray of potentiometer samples
float xLDR[N]; //Aray of LDR samples

//These variables hold the next sample
static float nextLDRSample, nextPOTSample;

//The location of the oldest sample
static int indexOfOldest=(N-1);

//These variables hold the average values
float fPOTSum = 0.0;
float fLDRSum = 0.0;

//The ticker, used to sample data at a fixed rate
Ticker t;

//Main function
int main()
{
    //Set baud rate to 115200
    pc.baud(115200);

    //Print header
    pc.printf("POT,LDR,avPOT, acLDR\n\n");

    //Set up the ticker - 100Hz
    t.attach(doCaptureSamples, 0.01);

    while(1) {

        //Sleep
        sleep();

        //Update sums
        fPOTSum = fPOTSum + nextPOTSample - xPOT[indexOfOldest];
        fLDRSum = fLDRSum + nextLDRSample - xLDR[indexOfOldest];

        //Write new sample over the oldest
        xPOT[indexOfOldest] = nextPOTSample;
        xLDR[indexOfOldest] = nextLDRSample;

        //update positon of oldest sample
        indexOfOldest = (indexOfOldest==0) ? (N-1) : indexOfOldest-1;

        //Write to terminal via Serial interface
        int middle = (indexOfOldest - (N/2)) % N;
        pc.printf("%6.4f,%6.4f,%6.4f,%6.4f\n", xPOT[middle], xLDR[middle], fPOTSum/(float)N, fLDRSum/(float)N);

        //Check the threshold
        if (fLDRSum > fPOTSum) {
            binaryOutput = 0;   //Binary 000
        } else {
            binaryOutput = 7;   //Binary 111
        }

    } //end while(1)
} //end main

//ISR for the ticker - simply there to perform sampling
void doCaptureSamples()
{
    //Toggle digital out
    strobe = !strobe;

    //READ ADC - store in static globals
    //(static globals are shared with all functions in the same C or CPP file)
    nextLDRSample = LDR_ADC_In;
    nextPOTSample = POT_ADC_In;
}

