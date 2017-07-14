#include "mbed.h"

//Time period
#define T 0.01

//Mark-Space Ratio
#define R 0.1

//Mark and Space times
#define Tmark (R*T)
#define Tspace ((1.0-R)*T)

DigitalOut onboardLed(LED1);
DigitalOut redLED(D7);

int main() {
    printf("\nWelcome to ELEC143\n");
    
        while (1) {
                redLED = 0; //Space
                wait(Tspace);
                redLED = 1; //Mark
                wait(Tmark);
        }
}
