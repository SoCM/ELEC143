#include "mbed.h"
#define kRED    4
#define kYELLOW 2
#define kGREEN  1


//Global objects
BusOut binaryOutput(D5, D6, D7);

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
      //3 decimal places, fieldwidth=5
      printf("Analog input = %6.4f\n", fVin);
      
      if (fVin < 0.4f)  {
         binaryOutput = kGREEN;
      } else if (fVin < 0.6f) {
         binaryOutput = kYELLOW;
      } else {
         binaryOutput = kRED;
      }
      
      //Wait
      wait(0.1);
      
   } //end while(1)
} //end main