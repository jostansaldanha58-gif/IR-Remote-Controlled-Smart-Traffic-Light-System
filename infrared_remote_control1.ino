
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include <IRremote.h>

Adafruit_7segment display = Adafruit_7segment();

// LED pins
#define RED_LED    3
#define GREEN_LED  4
#define BLUE_LED   5

// IR receiver
#define IR_PIN 2

const int BTN_OFF   = 12;   // Numeric 0
const int BTN_RED   = 16;   // Numeric 1
const int BTN_BLUE  = 17;   // Numeric 2
const int BTN_GREEN = 18;   // Numeric 3
const int BTN_PLAY  = 5;    // Play/Pause

bool autoMode = false;
int timerCount = 60;
unsigned long lastTick = 0;

int mapCodeToButton(unsigned long code)
{
  if ((code & 0x0000FFFF) == 0x0000BF00)
  {
    code >>= 16;
    if (((code >> 8) ^ (code & 0x00FF)) == 0x00FF)
      return code & 0xFF;
  }
  return -1;
}

int readInfrared()
{
  int result = -1;
  if (IrReceiver.decode())
  {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    result = mapCodeToButton(code);
    IrReceiver.resume();
  }
  return result;
}

void showValue(int value)
{
  display.clear();
  display.print(value);
  display.writeDisplay();
}

void ledsOff()
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}

void red()
{
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}

void blue()
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);
}

void green()
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);
}

void setup()
{
  Serial.begin(9600);

  display.begin(0x70);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  IrReceiver.begin(IR_PIN);

  ledsOff();
  showValue(0);
}

void automaticMode()
{
  if (millis() - lastTick >= 1000)
  {
    lastTick = millis();

    if (timerCount > 30)
      red();
    else if (timerCount > 20)
      blue();
    else if (timerCount > 0)
      green();
    else
      ledsOff();

    showValue(timerCount);

    timerCount--;

    if (timerCount < 0)
      timerCount = 60;
  }
}

void loop()
{
  int button = readInfrared();

  if (button >= 0)
  {
    switch (button)
    {
      case BTN_OFF:
        autoMode = false;
        ledsOff();
        showValue(0);
        break;

      case BTN_RED:
        autoMode = false;
        red();
        showValue(1);
        break;

      case BTN_BLUE:
        autoMode = false;
        blue();
        showValue(2);
        break;

      case BTN_GREEN:
        autoMode = false;
        green();
        showValue(3);
        break;

      case BTN_PLAY:
        autoMode = !autoMode;

        if (autoMode)
        {
          timerCount = 60;
          lastTick = millis();
        }
        else
        {
          ledsOff();
          showValue(0);
        }
        break;
    }
  }

  if (autoMode)
    automaticMode();
}
