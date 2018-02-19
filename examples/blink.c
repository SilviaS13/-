#include <wiringPi.h>
int main (void) {
  wiringPiSetup();
  pinMode (30, OUTPUT) ;
  for (;;) {
    digitalWrite(30, HIGH);
    delay (500) ;
    digitalWrite(30, LOW);
    delay(500);
  }
  return 0;
}