#include <Arduino.h>

#define HID
#define MCP23017
#define SERIAL_OUT

#ifdef HID
#undef SERIAL_OUT
#endif

#ifdef HID
#include "Adafruit_TinyUSB.h"
#endif
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

#define INTERUPT_PIN 29

#ifdef HID
uint8_t const desc_hid_report[] =
    {
        TUD_HID_REPORT_DESC_KEYBOARD()};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

uint8_t keycodes[16] = {
    HID_KEY_A,
    HID_KEY_B,
    HID_KEY_C,
    HID_KEY_D,
    HID_KEY_E,
    HID_KEY_F,
    HID_KEY_G,
    HID_KEY_H,
    HID_KEY_I,
    HID_KEY_J,
    HID_KEY_K,
    HID_KEY_L,
    HID_KEY_M,
    HID_KEY_N,
    HID_KEY_O,
    HID_KEY_P};
#endif

#ifdef MCP23017
Adafruit_MCP23X17 mcp; // Create MCP23017 device

TwoWire I2C1(i2c1, 6, 7); // Create a new I2C bus on pins 6 and 7 (SDA, SCL)

uint16_t pinValues = 0xFFFF;
#endif

void switchChange();
void hidOut(uint16_t pinValues);
uint16_t readMCP();
void failsafe(String error);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INTERUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERUPT_PIN), switchChange, FALLING);
  interrupts();

#ifdef SERIAL_OUT
  Serial.begin(115200);

  Serial.println("Wait for serial port to be opened...");

  while (!Serial.available())
    delay(1);

  for (int i = 0; i < 20; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(25);
    digitalWrite(LED_BUILTIN, LOW);
    delay(25);

    Serial.print(".");
  }
  Serial.println("");

  Serial.println("HID example");
#endif

#ifdef MCP23017
  if (!mcp.begin_I2C(0x20, &I2C1))
  {
    failsafe("mcp init failure"); // go into failsafe
  }

  mcp.setupInterrupts(true, false, LOW); // Set interrupt to active-low, open drain mode

  for (int i = 0; i < 16; i++)
  {
    mcp.pinMode(i, INPUT_PULLUP);

    mcp.setupInterruptPin(i, CHANGE); // Set pin to trigger interrupt on state change
  }
  mcp.clearInterrupts();
#endif

#ifdef HID
  if (!usb_hid.begin())
  {
    failsafe("usb_hid init failure"); // go into failsafe
  }

  // wait until device mounted
  while (!TinyUSBDevice.mounted())
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
#endif
}

void loop()
{
  uint8_t const report_id = 0;
  uint8_t const modifier = 0;
  uint8_t keycode[16] = {0};

  for (int i = 0; i < 16; i++)
  {
    if (!(pinValues & (1 << i)))
    {
      keycode[i] = keycodes[i];
    }
  }

  usb_hid.keyboardReport(report_id, modifier, keycode);
}

void switchChange()
{
  readMCP();
}

/*
void hidOut(uint16_t pinValues)
{
#ifdef HID
  uint8_t const report_id = 0;
  uint8_t const modifier = 0;

  uint8_t keycode[16] = {0};

  for (int i = 0; i < 16; i++)
  {
    if (!(pinValues & (1 << i)))
    {
      keycode[i] = keycodes[i];
    }
  }

  usb_hid.keyboardReport(report_id, modifier, keycode);
  delay(10);
  usb_hid.keyboardRelease(report_id);
#endif
}
*/

uint16_t readMCP()
{
#ifdef MCP23017
  pinValues = mcp.readGPIOAB(); // Read the pin values from the MCP23017

  return pinValues;
#endif

  return 0;
}

void failsafe(String error)
{
  while (true)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);

#ifdef SERIAL_OUT
    Serial.println(error);
#endif
  }
}