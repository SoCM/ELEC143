#include "mbed.h"

//Create an instance of a Serial object called pc
//Transmit and receive pins have names USBTX and USBRX
Serial pc(USBTX, USBRX);

int main() {

    //Set the baud rate property (bits persecond)
    pc.baud(9600);

    //Run in an infinite loop
    while(1) {
        
        //Call the printf method on pc
        pc.printf("Hello World\n");
        
        //Wait for 1s
        wait(1.0);
    }
}
