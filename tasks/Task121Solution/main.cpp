#include "mbed.h"

//This is my solution, using C functions to avoid repetition.

DigitalOut myled(D7);

void flash(double t);
void dash();
void dot();
void symbGap() ;

int main() {
    while(1) {
        dot();
        dot();
        dot();
        symbGap();
        dash();
        dash();
        dash();
        symbGap();
        dot();
        dot();
        dot();
        wait(5.0);
    }
}

void dot() {
    flash(0.15);
}

void symbGap() {
    wait(0.15);
}

void dash() {
    flash(0.45);
}

void flash(double t) {
    myled = 1;
    wait(t);
    myled=0; 
    wait(0.15);   
}