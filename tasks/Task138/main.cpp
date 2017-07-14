#include "mbed.h"

Serial pc(SERIAL_TX, SERIAL_RX);

char nameStr[30];           
int age = 1;    
            
void getData()
{
   pc.scanf("%s %d", &nameStr, &age);
}

void printData()
{
   pc.printf("Hello %s \n\r", nameStr);
   pc.printf("You are %d \n\r", age);
}

int main()
{
   int counter = 0;
   while (age > 0)
   {
      getData();
      printData();
      counter++;
   }
   pc.printf("You entered %d names\n", counter);
   sleep();
}
