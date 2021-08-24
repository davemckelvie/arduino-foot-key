#include <Arduino.h>
#include <Keyboard.h>

#define PIN_PLAY_STOP 2
#define PIN_FORWARD   3
#define PIN_BACK      4
#define PIN_SPEED     5
#define PIN_LOOP      6
#define LED_PIN         17

#define PLAY_BIT        (0x01)
#define FORWARD_BIT     (0x02)
#define BACK_BIT        (0x04)
#define SPEED_BIT       (0x08)
#define LOOP_BIT        (0x10)

#define PLAY_KEYCODE    ('Q')
#define FORWARD_KEYCODE ('U')
#define BACK_KEYCODE    ('E')
#define SPEED_KEYCODE   ('R')
#define LOOP_KEYCODE    ('T')

int ledState = HIGH;       
int lastButtonState = 0;
int buttonState;           

boolean playWasPressed = false;
boolean forwardWasPressed = false;
boolean backWasPressed = false;
boolean speedWasPressed = false;
boolean loopWasPressed = false;

unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50;   

void setup() {
  pinMode(PIN_PLAY_STOP, INPUT_PULLUP);
  pinMode(PIN_FORWARD, INPUT_PULLUP);
  pinMode(PIN_BACK, INPUT_PULLUP);
  pinMode(PIN_SPEED, INPUT_PULLUP);
  pinMode(PIN_LOOP, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  TXLED0;
  Keyboard.begin();
}

int readPins() {
   int play = digitalRead(PIN_PLAY_STOP);
   int forward = digitalRead(PIN_FORWARD);
   int back = digitalRead(PIN_BACK);
   int speed = digitalRead(PIN_SPEED);
   int loop = digitalRead(PIN_LOOP);
   return ~(play | (forward << 1) | (back << 2) | (speed << 3) | (loop << 4));
}

void toggleLed() {
   ledState = !ledState;
   digitalWrite(LED_PIN, ledState);
}

void sendKeyPress(uint8_t key) {
   toggleLed();
   Keyboard.write(key);
}

void loop() {

   int reading = readPins();

   if (reading != lastButtonState) {
      lastDebounceTime = millis();
   }

   if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
         buttonState = reading;
         
         boolean playpressed = (reading & PLAY_BIT) == PLAY_BIT;
         if (playpressed && !playWasPressed) {
            playWasPressed = true;
            sendKeyPress(PLAY_KEYCODE);
         } else {
            playWasPressed = false;
         }

         boolean forwardPressed = (reading & FORWARD_BIT) == FORWARD_BIT;
         if (forwardPressed && !forwardWasPressed) {
            forwardWasPressed = true;
            sendKeyPress(FORWARD_KEYCODE);
         } else {
            forwardWasPressed = false;
         }

         boolean backPressed = (reading & BACK_BIT) == BACK_BIT;
         if (backPressed && !backWasPressed) {
            backWasPressed = true;
            sendKeyPress(BACK_KEYCODE);
         } else {
            backWasPressed = false;
         }

         boolean speedPressed = (reading & SPEED_BIT) == SPEED_BIT;
         if (speedPressed && !speedWasPressed) {
            speedWasPressed = true;
            sendKeyPress(SPEED_KEYCODE);
         } else {
            speedWasPressed = false;
         }

         boolean loopPressed = (reading & LOOP_BIT) == LOOP_BIT;
         if (loopPressed && !loopWasPressed) {
            loopWasPressed = true;
            sendKeyPress(LOOP_KEYCODE);
         } else {
            loopWasPressed = false;
         }
      }
   }

   lastButtonState = reading;
}