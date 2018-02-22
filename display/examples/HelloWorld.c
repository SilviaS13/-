#include <wiringPi.h>          
#include <lcd.h>               
 
//USE WIRINGPI PIN NUMBERS
#define LCD_RS  8               //Register select pin
#define LCD_E   9               //Enable Pin
#define LCD_D4  7               //Data pin 4
#define LCD_D5  0               //Data pin 5
#define LCD_D6  2               //Data pin 6
#define LCD_D7  3               //Data pin 7
 
int main()
{
    int lcd;               
    wiringPiSetup();        
    lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
    lcdClear(lcd);      
    lcdPuts(lcd, "Hello, world!");
}
 
