#include "mbed.h"

#include "string.h"
#include <stdio.h>
#include <ctype.h>

#define RED_DONE 1
#define YELLOW_DONE 2

//Delays in ms
#define TUNIT 250
#define TDOT  TUNIT
#define TDASH (3*TUNIT)
#define TGAP TUNIT
#define TLETTER (3*TUNIT)
#define TWORD (7*TUNIT)

//Size of the morse character buffer
#define BUFFERSIZE 100


/* 
**** Morse Code ***

.               1 unit
-               3 units
inter-space     1 unit

between letters 3 units
between words   7 units
*/

//Morse code
const char* const morseAlpha[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--.."  //Z
};

const char* const morseNumeric[] = {
    "-----",    //0
    ".----",    //1
    "..---",    //2
    "...--",    //3
    "....-",    //4
    ".....",    //5
    "-....",    //6
    "--...",    //7
    "---..",    //8
    "----."     //9    
};

//Digital outputs
DigitalOut onBoardLED(LED1);
DigitalOut redLED(D7);
DigitalOut yellowLED(D6);
DigitalOut greenLED(D5);

//Serial Interface
Serial pc(USBTX, USBRX);

//Digital inputs
DigitalIn  onBoardSwitch(USER_BUTTON);
DigitalIn  SW1(D4);
DigitalIn  SW2(D3);

//Thread ID for the Main function (CMSIS API)
osThreadId tidMain;

//Thread sychronisation primatives
Semaphore *spaceAvailable;
Semaphore *samplesInBuffer;
Mutex *bufferLock;

//Output buffer
char buffer[BUFFERSIZE];
unsigned int newestIndex = BUFFERSIZE-1;    //First time it is incremented, it will be 0
unsigned int oldestIndex = BUFFERSIZE-1;          


//Producer
void addCharacterToQueue(const char c)
{    
    //Is there space?
    int32_t Nspaces = spaceAvailable->wait();
             
    //Ok, there is space - take the lock
    bufferLock->lock();
    redLED = 1;       
        
    //Update buffer
    newestIndex = (newestIndex+1) % BUFFERSIZE;  
    buffer[newestIndex] = c;
    pc.printf("\tAdded ASCII Character: %2Xh (%c) to buffer, %d spaces available\n", c, c, Nspaces-1);
    
    //Release lock
    bufferLock->unlock();
    redLED = 0;
    
    //Signal that a sample has been added
    samplesInBuffer->release();
}

//Consumer
char takeCharacterFromQueue()
{    
    //Are thre any samples in the buffer
    int32_t Nsamples = samplesInBuffer->wait();
        
    //Ok, there are samples - take the lock
    bufferLock->lock();   
    yellowLED = 1;
    
    //Update buffer - remove oldest
    oldestIndex = (oldestIndex+1) % BUFFERSIZE;
    char cc = buffer[oldestIndex];
    pc.printf("\t\tTaking ASCII Character: %2Xh (%c) from buffer, %d bytes remaining\n", cc, cc, Nsamples-1);
    
    //Release lock
    bufferLock->unlock();
    yellowLED = 0;
    
    //Signal there is space in the buffer
    spaceAvailable->release();
    
    //return a copy of the result
    return cc;
}

void morseGenerator() 
{
    while (true) {
        
        //Are there samples available?
        char nextChar = takeCharacterFromQueue();  
        nextChar = tolower(nextChar);
        
        //Look up morse code
        
        //Space between words - assumes only one
        if (nextChar == ' ') {
            Thread::wait(TWORD-TLETTER);
            continue;   
        }
        
        //Number?
        const char *nextString;
        if ((nextChar >= '0') && (nextChar <= '9')) {
            nextString = morseNumeric[nextChar - '0'];   
        }
        //Character?
        else if ((nextChar >='a') && (nextChar <= 'z')) {
            nextString = morseAlpha[nextChar - 'a'];   
        } else {
            //Invalid
            continue;   
        }
        
        //Flash morse for this letter
        for (unsigned int n=0; n<strlen(nextString); n++) {
            char symb = nextString[n];
            greenLED = 1;
            switch (symb) {
                case '.':
                    Thread::wait(TDOT);
                    break;
                case '-':
                    Thread::wait(TDASH);
                    break;
                default:
                    break;  
            }
            greenLED = 0;
            
            //A gap between symbols
            Thread::wait(TGAP);    
        } 
        
        //Gap between letters
        Thread::wait(TLETTER-TGAP);
        
    }      
}

//Main thread
int main() {
    Thread writer;
    
    redLED    = 0;
    yellowLED = 0;
    greenLED  = 0;
        
    //Semaphores
    bufferLock = new Mutex();
    spaceAvailable = new Semaphore(BUFFERSIZE);
    samplesInBuffer = new Semaphore(0);
        
    
    //Main thread ID
    tidMain = Thread::gettid();  
    
    
    //Thread for outputting mors
    writer.start(morseGenerator);
    
    pc.printf("Type characters to send\n");
    while (true) {
        //Read keyboard (serial port)
        char c = pc.getc();
        addCharacterToQueue(c);
    }

}

   
