#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>

#define	PIN_1A	11
#define	PIN_1B	12
#define PIN_2A	13
#define PIN_2B	14
#define DELAY 50

//one full step equals 1.8°, 360° = 200 steps
int step_count = 0;  //our halfstep = 0.9°,  360° = 400 steps  
int substep_num = 0;

int clockwise = 1;
int degree = 0;

//0x7 = 0b1001
//0x3 = 0b0011
//0x2 = 0b0010
//0x6 = 0b0110
//0x4 = 0b0100
//0x8 = 0b1000
int shift_vals[7][2] = {
    {PIN_1A, PIN_2B},
    {PIN_1A, PIN_1B},
    {PIN_1B,0},
    {PIN_1B, PIN_2A},
    {PIN_2A,0},
    {PIN_2A, PIN_2B},
    {PIN_2B, 0}
};

void ResetPins(){
    digitalWrite (PIN_1A, 0);
    digitalWrite (PIN_2A, 0);
    digitalWrite (PIN_1B, 0);
    digitalWrite (PIN_2B, 0);
}   
 
void Rotate()
{
    //our code
    ResetPins();    
    digitalWrite(shift_vals[substep_num][0], 1);
    if (shift_vals[substep_num][1]!=0)
        digitalWrite(shift_vals[substep_num][1], 1);
    
    if (clockwise == 1){
        if (++substep_num == 7) 
            substep_num = 0; 
    }
    else{
        if (--substep_num == -1) 
            substep_num = 6;
    }
}

void Setup(){
    wiringPiSetup();
    pinMode(PIN_1A, OUTPUT);
    pinMode(PIN_1B, OUTPUT);
    pinMode(PIN_2A, OUTPUT);
    pinMode(PIN_2B, OUTPUT);
    ResetPins();
    printf("Setup complete.\n");
}

int main(void){
    Setup();
    printf("Print 1 for clockwise rotate, 0 for anticlockwise\n");
    scanf("%d", &clockwise);
    while(1){
        Rotate();
        usleep(DELAY*1000);//us
    };
}
