/*
MIT License

Copyright (c) 2071 Kenneth S. Davis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


KVM Switcher
-------------------------------------------------------------------------------

A simple application for the PJRC teensy 3.2, (although other versions
should work fine).  Provides 6 buttons to switch a 4 PORT KVM.  The KVM
has a USB console port, which is not fully compatible with my RGB Keyboard.
Although the KVM supports many key combinations, I only use 6, a direct
switch to one of the four ports, a 'next port', and a 'previous port'.

Also four LEDs are driven to indicate which port is currently switch.  although
there exists no direct query from the KVM, hence no LED's are turned on until
the first selection is made.

In order to ensure the Teensy has USB KEYBOARD support ensure you compile with
the 'USE_HID' option (ex: -DUSE_HID).  I could not get the 'USE_KEYBOARDONLY'
to work, but USE_HID works fine enough.

The buttons have PULL-UPS on the digital pin and I switch GROUND.  The LED pin
on the otherhand provide power when turned on. 

*/

#include "Arduino.h"
#include "Bounce2.h"

#define BUTTON_UP 2
#define BUTTON_1  3
#define BUTTON_2  4
#define BUTTON_3  5
#define BUTTON_4  6
#define BUTTON_DOWN 7

#define LED_1   20
#define LED_2   21
#define LED_3   22
#define LED_4   23

#define BUTTON_COUNT 6

struct KVM
{
  Bounce button;
  int keyCode;
  int pin;
  int ledPin;
};

struct KVM kvm[BUTTON_COUNT] =
{
  { Bounce(), KEY_UP_ARROW, BUTTON_UP, 0 },
  { Bounce(), KEY_1, BUTTON_1, LED_1},
  { Bounce(), KEY_2, BUTTON_2, LED_2},
  { Bounce(), KEY_3, BUTTON_3, LED_3},
  { Bounce(), KEY_4, BUTTON_4, LED_4},
  { Bounce(), KEY_DOWN_ARROW, BUTTON_DOWN, 0 }
};



void SetButton(Bounce &bounce, int button)
{
  pinMode(button, INPUT_PULLUP);
  bounce.attach(button);
  bounce.interval(5);
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    for (int i = 0; i < BUTTON_COUNT; i++)
    {
      SetButton(kvm[i].button, kvm[i].pin);
      if (kvm[i].ledPin != 0)
      {
        pinMode( kvm[i].ledPin, OUTPUT);
        digitalWrite( kvm[i].ledPin, LOW);
      }
    }

    Keyboard.begin();

}

void SendKVM(int code)
{
  Keyboard.set_key1(KEY_SCROLL_LOCK);
  Keyboard.send_now();
  delay(50);

  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(50);

  Keyboard.set_key1(KEY_SCROLL_LOCK);
  Keyboard.send_now();
  delay(50);

  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(50);

  Keyboard.set_key1(code);
  Keyboard.send_now();
  delay(50);

  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(50);
}

int currentLED = LED_1;

void loop()
{
  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    kvm[i].button.update();
    if (kvm[i].button.fell())
    {
      digitalWrite(LED_BUILTIN, HIGH);

      digitalWrite( currentLED, LOW);
      switch (kvm[i].keyCode)
      {
        case KEY_1: currentLED = LED_1; break;
        case KEY_2: currentLED = LED_2; break;
        case KEY_3: currentLED = LED_3; break;
        case KEY_4: currentLED = LED_4; break;
        case KEY_UP_ARROW:
        {
          switch (currentLED)
          {
            case LED_1: currentLED = LED_4; break;
            case LED_2: currentLED = LED_1; break;
            case LED_3: currentLED = LED_2; break;
            case LED_4: currentLED = LED_3; break;
          }
          break;
        }
        case KEY_DOWN_ARROW:
        {
          switch (currentLED)
          {
            case LED_1: currentLED = LED_2; break;
            case LED_2: currentLED = LED_3; break;
            case LED_3: currentLED = LED_4; break;
            case LED_4: currentLED = LED_1; break;
          }
        }
        break;
      }
      digitalWrite( currentLED, HIGH);


      SendKVM(kvm[i].keyCode);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

}
