#include "mbed.h"


#define kRED    (1 << 2)    //4
#define kYELLOW (1 << 1)    //2
#define kGREEN  (1 << 0)    //1

//Global objects
BusOut binaryOutput(D5, D6, D7);    //Outputs as an integer

DigitalIn SW1(D3);
DigitalIn SW2(D4);

AnalogIn AIN(A0);
float fVin = 0.0;

//Main function
int main() {
    
    //This represents which state we are in
    //RED or GREEN
    int state = 0;
    
    while(1) {
        
        //Read ADC
        fVin = AIN;
        
        //Write to terminal
        printf("Analog input = %6.4f\n", fVin);
        
        //Now the "state machine" - next state logic
        switch (state) {
            //The RED state
            case 0:
                //Condition to switch state
                if (fVin > 0.6f) {
                    state = 1;
                }
                break;
                
            //The GREEN state
            case 1:
                
                //Condition to switch state
                if (fVin < 0.4f) {
                    state = 0;
                }
                break;
                
            default:
                state = 0;
        }
        
        //Output logic
        switch (state) {
        case 0:
            binaryOutput = kGREEN;
            break;
        case 1:
            binaryOutput = kRED;
            break;
        default:
            binaryOutput = 0;        
        }
        
        //Wait
        wait(0.1);
        
    } //end while(1)
} //end main