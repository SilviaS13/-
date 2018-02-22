//https://omerk.github.io/lcdchargen/ 

#include <wiringPi.h>         
#include <lcd.h>                

//USE WIRINGPI PIN NUMBERS
#define LCD_RS  25               //Register select pin
#define LCD_E   24               //Enable Pin
#define LCD_D4  23               //Data pin 4
#define LCD_D5  22               //Data pin 5
#define LCD_D6  21               //Data pin 6
#define LCD_D7  14               //Data pin 7

char omega[8] = { 0b00000,
                  0b01110,
                  0b10001,
                  0b10001,
                  0b10001,
                  0b01010,
                  0b11011,
                  0b00000};

char pi[8] = { 0b00000,
               0b00000,
               0b11111,
               0b01010,
               0b01010,
               0b01010,
               0b10011,
               0b00000};

char mu[8] = { 0b00000,
               0b10010,
               0b10010,
               0b10010,
               0b10010,
               0b11101,
               0b10000,
               0b10000};
               
char drop[8] = { 0b00100,
	             0b00100,
	             0b01010,
	             0b01010,
	             0b10001,
	             0b10001,
	             0b10001,
	             0b01110}; 
	       
char temp[8] = { 0b00100,
                 0b01010,
	             0b01010,
	             0b01110,
	             0b01110,
	             0b11111,
	             0b11111,
	             0b01110};

void customChar(void);
int lcd;

int main()
{
    int lcd;
    wiringPiSetup();
    lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

    customChar();
}

void customChar(void)
{       
        //Define chars
        lcdCharDef(lcd, 10, omega); 
        lcdCharDef(lcd, 11, pi);
        lcdCharDef(lcd, 12, mu);
        lcdCharDef(lcd, 13, drop);
        lcdCharDef(lcd, 14, temp);

        lcdClear(lcd);

        lcdPutchar(lcd, 10);
        lcdPutchar(lcd, 11);
        lcdPutchar(lcd, 12);
        lcdPutchar(lcd, 13);
        lcdPutchar(lcd, 14);

        sleep(3);
}

