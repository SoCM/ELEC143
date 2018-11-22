/*

** ELEC143 Coursework Sample Code **

Author: Dr Nicholas Outram
Date: 09-10-2015
Version: 1.0

This sample code demonstrates the following:

- How to control the motor speed and direction
- How to measure motor speed using software polling (assuming a forward direction)

Notes:

1. This code is very monolithic. I have made no attempt to factor this into meaningful and
reuseable functions. I will be looking to see you factor your code into different functions,
and maybe even into seperate C (or CPP) files.

2. This code is only lightly commented. The commenting is sufficient to
explain the purpose of each step in the code. Do not assume this is always sufficient.

You may adapt this code for your own purposes. However, you may be questioned about any of the
code you use. Your answers will affect your grade to some extent.

*/
#include "mbed.h"

//Status LED
DigitalOut led(LED1);

//Motor PWM (speed)
PwmOut PWMA(PA_8);
PwmOut PWMB(PB_4);

//Motor Direction
DigitalOut DIRA(PA_9);
DigitalOut DIRB(PB_10);

//Hall-Effect Sensor Inputs
DigitalIn HEA1(PB_2);
DigitalIn HEA2(PB_1);
DigitalIn HEB1(PB_15);
DigitalIn HEB2(PB_14);

//On board switch
DigitalIn SW1(USER_BUTTON);

//Use the serial object so we can use higher speeds
Serial terminal(USBTX, USBRX);

//Timer used for measuring speeds
Timer timer;

//Enumerated types
enum DIRECTION   {FORWARD=0, REVERSE};
enum PULSE       {NOPULSE=0, PULSE};
enum SWITCHSTATE {PRESSED=0, RELEASED};

//Debug GPIO
DigitalOut probe(D10);

//Duty cycles
float dutyA = 1.0f; //100%
float dutyB = 1.0f; //100%

int main()
{
    //Configure the terminal to high speed
    terminal.baud(115200);

    //Set initial motor direction
    DIRA = FORWARD;
    DIRB = FORWARD;

    //Set motor period to 100Hz
    PWMA.period_ms(10);
    PWMB.period_ms(10);

    //Set initial motor speed to stop
    PWMA.write(0.0f);           //0% duty cycle
    PWMB.write(0.0f);           //0% duty cycle

    //Wait for USER button (blue pull-down switch) to start
    terminal.puts("Press USER button to start");
    led = 0;
    while (SW1 == RELEASED);
    led = 1;

    //Set initial motor speed to stop
    PWMA.write(dutyA);          //Set duty cycle (%)
    PWMB.write(dutyB);          //Set duty cycle (%)

    //Wait - give time to start running
    wait(1.0);

    //Array of sensor data
    int tA1[2];
    int tA2[2];

    //Instructions to user
    terminal.puts("Press USER button to adapt duty (to convernge on 1 rotation/s)");

    //Main polling loop
    while(1) {
        //Reset timer and Start
        timer.reset();
        timer.start();

        //Wait for HEA1 to go to LOW
        while (HEA1 == PULSE);

        //Wait for rising edge of A1 and log time
        while (HEA1 == NOPULSE);
        tA1[0] = timer.read_us();

        //Wait for rising edge of A2 and log time (30 degrees?)
        while (HEA2 == NOPULSE);
        tA2[0] = timer.read_us();


        //Wait for falling edge of A1
        while (HEA1 == PULSE);
        //Wait for falling edge of A2
        while (HEA2 == PULSE);

        //Wait for rising edge of A1 and log time
        while (HEA1 == NOPULSE);
        tA1[1] = timer.read_us();

        //Wait for rising edge of A2 and log time (30 degrees?)
        while (HEA2 == NOPULSE);
        tA2[1] = timer.read_us();

        terminal.printf("tA1(0) = %d\n", tA1[0]);
        terminal.printf("tA1(1) = %d\n", tA1[1]);
        terminal.printf("tA2(0) = %d\n", tA2[0]);
        terminal.printf("tA2(1) = %d\n", tA2[1]);

        //Calculate the frequency of rotation
        float fA1 = 1.0f/(( tA1[1]-tA1[0] )*(float)3.0E-6);
        float fA2 = 1.0f/(( tA2[1]-tA2[0] )*(float)3.0E-6);
        float fA = (fA1 + fA2)*0.5f;    //Average frequency
        terminal.printf("A1 Shaft: %6.2fHz \t Wheel: %6.2f\n", fA1, fA1/20.2f);
        terminal.printf("A2 Shaft: %6.2fHz \t Wheel: %6.2f\n", fA2, fA2/20.2f);
        terminal.printf("Average A2 Shaft: %6.2fHz \t Wheel: %6.2f\n", fA, fA/20.2f);

        //Reset timers
        timer.stop();

        //Wait for button press
        while (SW1 == 1);
        wait(0.2);
        while (SW1 == 0);
        wait(0.1);

        //Adapt duty to meet 1 revolution per second
        float wA = fA/20.2f;                    //Wheel speed
        float deltaA = 1.0f-wA;             //Error
        dutyA = dutyA + deltaA*0.1f;    //Increase duty in proportion to the error
        //The max and min values of duty and 0.0 and 1.0 respectively
        dutyA = (dutyA>1.0f) ? 1.0f : dutyA;
        dutyA = (dutyA<0.05f) ? 0.05f : dutyA;
        PWMA.write(dutyA);                  //Update duty cycle

        //Echo to the terminal
        terminal.printf("Adapting duty cycle to %6.2f\n", dutyA);

    }
}


