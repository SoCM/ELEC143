#include "mbed.h"

Serial pc(SERIAL_TX, SERIAL_RX);

char nameStr[30]; // array of chars (string)
int age;          // integer to hold your age

void getData() 
{
   pc.scanf("%s %d", &nameStr, &age);
}

int main()
{
   getData();
   
   pc.printf("Hello %s \n\r", nameStr);
   pc.printf("You are %d \n", age);
}