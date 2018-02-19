#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>

#define PIXELS 13 // Number of pixels in the string

// These values are for digital pin 8 on an Arduino Yun or digital pin 12 on a DueMilinove
// Note that you could also include the DigitalWriteFast header file to not need to to this lookup.

#define PIXEL_PIN 13 // Bit of the pin the pixels are connected to
 
// These are the timing constraints taken mostly from the WS2812 datasheets
// These are chosen to be conservative and avoid problems rather than for maximum throughput 

// 1 is >550ns high and >450ns low; 0 is 200..500ns high and >450ns low 
#define T1H  900    // Width of a 1 bit in ns 
#define T1L  350    // Width of a 1 bit in ns
 
#define T0H  50    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns
 
#define RES 50000    // Width of the low gap between bits to cause a frame to latch
 
// Here are some convenience defines for using nanoseconds specs to generate actual CPU delays
 
//#define NS_PER_SEC (1000000000L) // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives
 
/*#define CYCLES_PER_SEC (F_CPU)
 
#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )
 
#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )
 
#define DELAY_CYCLES(n) ( ((n)&gt;0) ? __builtin_avr_delay_cycles( n ) : __builtin_avr_delay_cycles( 0 ) ) // Make sure we never have a delay less than zero
*/

// Actually send a bit to the string. We turn off optimizations to make sure the compile does
// not reorder things and make it so the delay happens in the wrong place.
 
//void sendBit(char) __attribute__ ((optimize(0)));

/*
 
The following three functions are the public API:
  ledSetup() - set up the pin that is connected to the string. Call once at the beginning of the program.
  sendPixel( r , g , b ) - send a single pixel to the string. Call this once for each pixel in a frame.
  show() - latch the recently sent pixels on the LEDs . Call once per frame.
*/
 


void sendBit( char bitVal ) {
    // 1 is >550ns high and >450ns low; 0 is 200..500ns high and >450ns low
    if ( bitVal == 1) {      // 1-bit 
    //printf("1\n");
        digitalWrite(PIXEL_PIN, 1); 
        nanosleep(T1H);               // 1-bit width less overhead for the actual bit setting
                                        // Note that this delay could be longer and everything would still work
        digitalWrite(PIXEL_PIN, 0); 
        //DELAY_CYCLES( NS_TO_CYCLES( T1L ) - 10 ); // 1-bit gap less the overhead of the loop
        nanosleep(T1L);
    } 
    else {             // 0-bit
    //printf("0\n");
      //cli();                                       // We need to protect this bit from being made wider by an interrupt  
      digitalWrite(PIXEL_PIN, 1);  
      nanosleep(T0H); // 0-bit width less overhead      
                                                    // **************************************************************************
                                                    // This line is really the only tight goldilocks timing in the whole program!
                                                    // **************************************************************************
      digitalWrite(PIXEL_PIN, 0);  
      nanosleep(T0L);   // 0-bit gap less overhead of the loop
      //sei(); 
    }
 
    // Note that the inter-bit gap can be as long as you want as long as it doesn't exceed the 5us reset timeout (which is A long time)
    // Here I have been generous and not tried to squeeze the gap tight but instead erred on the side of lots of extra time.
    // This has thenice side effect of avoid glitches on very long strings becuase
 
}
 
void sendByte( unsigned char byte ) {
    char cur_bit = 7;
    unsigned char bit;
    for( bit = 0 ; bit < 8 ; bit++ ) {
        sendBit( (byte >> cur_bit) & 1 ); // Neopixel wants bit in highest-to-lowest order
	//sendBit(0);                                             // so send highest bit (bit #7 in an 8-bit byte since they start at 0)
      cur_bit--; // and then shift left so bit 6 moves into 7, 5 moves into 6, etc 
    }
} 

void show() {
	//nanosleep(RES);				// Round up since the delay must be _at_least_ this long (too short might not work, too long not a problem)
}
 
void sendPixel( unsigned char r, unsigned char g , unsigned char b ) {
    //printf("green\n");
    sendByte(g); // Neopixel wants colors in green-then-red-then-blue order
    //printf("red\n");
    sendByte(r);
    //printf("blue\n");
    sendByte(b);
    //show(); 
}
 
// Just wait long enough without sending any bots to cause the pixels to latch and display the last sent frame
 
// Just wait long enough without sending any bots to cause the pixels to latch and display the last sent frame
unsigned int rgbColour[3];
/*void colors(){
    

  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;
    int deColour;
  // Choose the colours to increment and decrement.
  for (decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    int i;
    for(i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
    }
}
*/
void ledSetup(){
    wiringPiSetup();
    pinMode(PIXEL_PIN, OUTPUT);
}    

int leds = 5;

int main (void){
    ledSetup();
    int i;   
    while(1){
    for (i=0; i<leds; i++){
	sendPixel(0, 100, 255);	
	//printf("next\n");
    }
    usleep(55);
    }
}