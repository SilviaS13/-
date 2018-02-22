#include <stdio.h>
#include <wiringPi.h>
#include <lcd.h>
#include <string.h>

//USE WIRINGPI PIN NUMBERS
#define LCD_RS  25               //Register select pin
#define LCD_E   24               //Enable Pin
#define LCD_D4  23               //Data pin 4
#define LCD_D5  22               //Data pin 5
#define LCD_D6  21               //Data pin 6
#define LCD_D7  14               //Data pin 7

void scrollText(void);
char message[] = "Hello, world!";
int count = 0;
int j = 0;
int lcd;

int main()
{
        wiringPiSetup();
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
        while(1){
            scrollText();
            }
}

void  scrollText(void)
{
        int i, n;
        int h;
        int tempSpace = 0;
        char scrollPadding[] = "                ";
        int messageLength = strlen(scrollPadding) + strlen(message);
        for (n = 0; n < messageLength; n++){h = 16; usleep(300000); printf("\x1B[2J"); if (j > messageLength)
                        j = 0;

                for (i = strlen(message); i >= 0; i--){
                        scrollPadding[j - h] = message[i];
                h++;
                }
                lcdPosition(lcd, 0, 0);
                lcdClear(lcd);
                lcdPrintf(lcd, "%s", scrollPadding);
                j++;
        }
}
 
