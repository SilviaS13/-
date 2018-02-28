#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 4
#define COLS 3
#define DELAY 300000

char keys[COLS][ROWS] = {
    {'1','4','7','*'},
    {'2','5','8','0'},
    {'3','6','9','#'}
};

int keyPinRows[4] = {1,6 ,10 ,11};
int keyPinCols[3] = {5, 4, 16};

char getKey(){
    int r, c;
    for (c = 0; c < COLS; c++){
        digitalWrite(keyPinCols[c], HIGH);
        for (r = 0; r < ROWS; r++){
            if(digitalRead(keyPinRows[r]) == 1){
                digitalWrite(keyPinCols[c], LOW);
                return keys[c][r];
            }
        }
        digitalWrite(keyPinCols[c], LOW);
    }
    return 0;
}

void setup(void){
    wiringPiSetup();
    int i;
    for (i = 0; i< ROWS;i++){
        pinMode(keyPinRows[i], INPUT);
        pullUpDnControl (keyPinRows[i], PUD_DOWN);
    }
    
    for (i = 0; i< COLS;i++)
        pinMode(keyPinCols[i], OUTPUT);
}

int main(void){
    setup();
    char key = 0;
    while(1){
        key = getKey();
        if(key != 0){
            printf("%c\n", key);
            usleep(DELAY);
        }
        usleep(DELAY);
    }
}