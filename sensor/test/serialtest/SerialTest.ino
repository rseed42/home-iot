#include <SPI.h>

#define LED_PIN 1
#define DELAY 100

unsigned long counter;

void setup () {
  pinMode(LED_PIN, OUTPUT);
}

void loop () {
  digitalWrite(LED_PIN, HIGH);
  delay(DELAY);
  digitalWrite(LED_PIN, LOW);
  delay(DELAY);

}
