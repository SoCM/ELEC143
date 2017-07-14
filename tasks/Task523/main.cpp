#include "mbed.h"

PwmOut pwmRed(D7);

int T = 10;
int Tmark = 1;

int main() {
    
    pwmRed.period_us(T);
    pwmRed.pulsewidth_us(Tmark);
    
    while(1) {
        sleep();
    }
}
