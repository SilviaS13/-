#include <WS2812B.h>
//#include "led_functions.h"
#define NUM_LEDS 13

#define DEBUG 1

//#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
#define WIRE_PIN_RECIEVER PB9
#define WIRE_PIN_TRANSMITTER PB8
#define MESSAGE_CRC(array)  ((array[0] == true) && (array[1] == true) && (array[5] == true) && (array[6] == true))
#define INTERRUPT_MODE FALLING    //FALLING, CHANGE, rising
#define LED PC13
#define LED_BUF PB7
#define BUFLEN  7//33
#define REDLED_HIGH 0
#define REDLED_LOW 1

volatile long int period_us = 40000; //ПЕРИОД СИГНАЛА = 40 мкс; Большая скорость достигается уменьшением номинала резистора подтяжки.
volatile bool wire_buffer[BUFLEN];
volatile int wire_buffer_counter = 0;
volatile bool start_bit = 0;
volatile bool after_set = 1;
volatile bool led_strip_set = 0;
volatile int counter = 0;
WS2812B strip = WS2812B(NUM_LEDS);
HardwareTimer wireTimer(2); //Timer for measuring input pulsewidths

void clearBuffer(){
  for (int i = 0; i < BUFLEN; i++) {
    wire_buffer[i] = false;
  }
}

void setup(){  
  wireTimer.pause();
  strip.begin();// Sets up the SPI
  strip.show();// Clears the strip, as by default the strip data is set to all LED's off.
  // initialise the pulse in port mask and set up the pulse timer ISRs
  pinMode(WIRE_PIN_RECIEVER, INPUT);//INPUT, INPUT_PULLUP, INPUT_PULLDOWN
  pinMode(LED, OUTPUT);
  pinMode(LED_BUF, OUTPUT);
  digitalWrite(LED_BUF, 0);
  digitalWrite(LED, REDLED_HIGH);
  clearBuffer();//init wire buffer
  attachInterrupt(WIRE_PIN_RECIEVER, pulse_isr, INTERRUPT_MODE);
}

void loop() {
  if (led_strip_set == 1) {
    led_strip_set = 0;
    if (MESSAGE_CRC(wire_buffer)){
      if (wire_buffer[2] == 1 /*&& wire_buffer[3] == 0 && wire_buffer[4] == 0*/)
        colorWipe(strip.Color(255, 0, 0), 20); // red
      //else if (wire_buffer[2] == 0 && wire_buffer[3] == 1 && wire_buffer[4] == 0){
      else if (wire_buffer[3] == 1){
        colorWipe(strip.Color(0, 255, 0), 1); // Green
       }
      //else if (wire_buffer[2] == 0 && wire_buffer[3] == 0 && wire_buffer[4] == 1){
      else if (wire_buffer[4] == 1){
        colorWipe(strip.Color(0, 0, 255), 1); // blue
        }
      else{
        checkTimer(strip.Color(0, 255, 255), 1); // gb        
      }
    }
    for(uint16_t i=0; i<BUFLEN; i++)
    {            
      strip.setPixelColor(i+3, strip.Color( ((wire_buffer[i] == false) ? 255 : 0), ( (wire_buffer[i] == true) ? 255 : 0), 255));
      strip.show();
      delay(1);
    }
    clearBuffer();
  }  
}

void pulse_isr(void){
  detachInterrupt(WIRE_PIN_RECIEVER);
  clearBuffer();
  wireTimer.pause();
  wireTimer.setPeriod((1.5 * period_us));
  wireTimer.setMode(1, TIMER_OUTPUT_COMPARE);
  wireTimer.setCompare(TIMER_CH1, 1);
  wireTimer.attachInterrupt(1, pulse_overfREDLED_LOW_isr);//0 - overflow, 1 - compare
  wireTimer.refresh();
  wireTimer.resume();
  after_set = true;
  start_bit = true;
  wire_buffer[0] = true;
  counter = 0;  
  digitalWrite(LED, REDLED_LOW);
}

void pulse_overfREDLED_LOW_isr(void){
  counter++;
  if (after_set) { //после рефреш таймера сразу происходит прерывание
    after_set = false;
    return;
  }
  
  wireTimer.pause();
  
  if (start_bit) {
    wireTimer.setPeriod(period_us);
    wireTimer.refresh();
    after_set = true;
    start_bit = 0;
    wire_buffer_counter = 1;
  }

  //read and invert wire value
  digitalWrite(LED, digitalRead(WIRE_PIN_RECIEVER)); //if 0 - led is 
  wire_buffer[wire_buffer_counter] = (digitalRead(WIRE_PIN_RECIEVER) == 1 ? false : true);
  wire_buffer_counter++;
  wireTimer.resume();
  
  if (wire_buffer_counter >= BUFLEN) {
    wireTimer.pause();
    attachInterrupt(WIRE_PIN_RECIEVER, pulse_isr, INTERRUPT_MODE);
    wireTimer.detachInterrupt(1);//0 - overflow, 1 - compare
    led_strip_set = 1;
    digitalWrite(LED, REDLED_HIGH);
    wire_buffer_counter = 0;
  }
}












uint8_t LEDGamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void checkTimer(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<4; i++)
  {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}



// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) 
{
  uint16_t i, j;

  for(j=0; j<256*5; j++) 
  { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
  if(WheelPos < 85) 
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else 
  {
    if(WheelPos < 170) 
    {
     WheelPos -= 85;
     return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } 
    else 
    {
     WheelPos -= 170;
     return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++){
    
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel

      for(int i=0; i< strip.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );

      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
          fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
          fadeVal--;
      }

        strip.show();
        delay(wait);
    }
  
  }



  delay(500);


  for(int k = 0 ; k < whiteLoops ; k ++){

    for(int j = 0; j < 256 ; j++){

        for(uint16_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, LEDGamma[j] ) );
          }
          strip.show();
        }

        delay(2000);
    for(int j = 255; j >= 0 ; j--){

        for(uint16_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, LEDGamma[j] ) );
          }
          strip.show();
        }
  }

  delay(500);


}

void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {
  
  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;


  while(true){
    for(int j=0; j<256; j++) {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
          strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
        }
        else{
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        
      }

      if(millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if(head == strip.numPixels()){
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) return;
    
      head%=strip.numPixels();
      tail%=strip.numPixels();
        strip.show();
        delay(wait);
    }
  }
  
}
void fullWhite() {
  
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
    }
      strip.show();
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}

