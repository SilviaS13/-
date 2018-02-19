#include <wiringPi.h>
#include <stdio.h>

#define SDA 12
#define LED 30

//mode for write
//111(2) = 7(10) - rainbow
//000 = 0 - single color
//010(2) = 2(10) - start changing
//101(2) = 5(10) - stop changing

//ONE_WIRE interface

//11 10 xxx xxxx_xxxx xxxx_xxxx xxxx_xxxx 11 - write
//11 01 xxx xxxx_xxxx xxxx_xxxx xxxx_xxxx 11 - read

unsigned char buffer[39];
unsigned char light_mode;

int sda_pulse_width = 40000;//us

void sendBit(char bit){
    
    if (bit == 1){
        digitalWrite(SDA, 1);
	digitalWrite(LED, 1);
	printf("1");
        usleep(sda_pulse_width);
    }
    else{
	printf("0");
        usleep(sda_pulse_width);
    }
    printf("\n");
    digitalWrite(SDA, 0);
    digitalWrite(LED, 0);
}

void sendByte(unsigned char byte, int lenght){
    
    char cur_bit = lenght - 1;
    unsigned char bit;
    for( bit = 0 ; bit < lenght ; bit++ ) {
      sendBit( (byte >> cur_bit) & 1 );
      cur_bit--; // and then shift left so bit 6 moves into 7, 5 moves into 6, etc 
    }
}

void startStop(){    
    //printf("Start_stop\n");
    //digitalWrite(LED, 1);
    //digitalWrite(SDA, 1);
    //usleep(sda_pulse_width*2);
    sendBit(1);
    sendBit(1);
    //digitalWrite(SDA, 0);
    //digitalWrite(LED, 0);
}

void sendNeopixel(unsigned char r_w, unsigned char mode, unsigned char r,unsigned char g, unsigned char b){
    
    //pinMode(SDA, OUTPUT);
    startStop();
    sendByte(r_w, 2);
    if (mode = 0){
        sendByte(r, 8);
        sendByte(g, 8);
        sendByte(b, 8);
    }
    else if (mode = 2){
        sendByte(mode, 3);
    }
    else if (mode = 5){
        sendByte(mode, 3);
    }
    else if (mode = 7){
        sendByte(mode, 3);
    }
    startStop();
    //pinMode(SDA, INPUT);
}

void sendMessage(unsigned char byte, int lenght){
    //pinMode(SDA, OUTPUT);
    startStop();
    sendByte(byte, lenght);
    startStop();
    //pinMode(SDA, INPUT);
}

void setup(){
    wiringPiSetup();
    pinMode(SDA, OUTPUT);
    //pinMode(LED, OUTPUT);
    //digitalWrite(LED, 1);
    //printf("setup complete\n");
}

int main(void){
    
    setup();
    sendMessage(1, 3);
    sleep(10);
    sendMessage(2, 3);
    sleep(10);
    sendMessage(4, 3);
    //SendNeopixel('w', 0, 255, 0, 0);
}
