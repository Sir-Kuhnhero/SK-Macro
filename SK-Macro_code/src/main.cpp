#include <Arduino.h>

// #define ArduinoIDE
#define SERIAL_OUT
#ifdef ArduinoIDE
#undef SERIAL_OUT
#endif

#ifdef ArduinoIDE
#include "Adafruit_TinyUSB.h"
#endif
#include <Wire.h>
// #include "wiring_private.h"
#include <Adafruit_MCP23X17.h>

// #define HID

#define INTERUPT_PIN 4

#ifdef HID
// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor
uint8_t const desc_hid_report[] =
    {
        TUD_HID_REPORT_DESC_KEYBOARD()};

// USB HID object. For ESP32 these values cannot be changed after this declaration
// desc report, desc len, protocol, interval, use out endpoint
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

Adafruit_MCP23X17 mcp; // Create MCP23017 device

#ifndef ArduinoIDE
TwoWire I2C1(6, 7); // Create a new I2C bus on pins 6 and 7 (SDA, SCL)
#endif

void switchChange();
void hidOut(uint16_t pinValues);
uint16_t readMCP();
void failsafe(String error = "Unknown error");

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  // attachInterrupt(digitalPinToInterrupt(INTERUPT_PIN), readMCP, FALLING);

#ifdef SERIAL_OUT
  Serial.begin(115200);

  Serial.println("Wait for serial port to be opened...");

  while (!Serial.available())
    delay(1);

  for (int i = 0; i < 20; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);

    Serial.print(".");
  }
  Serial.println("");

  Serial.println("HID example");
#endif

#pragma region MCP23017
#ifdef ArduinoIDE
  if (!mcp.begin_I2C(0x20, &Wire1))
  {
    failsafe(); // go into failsafe
  }
#endif
#ifndef ArduinoIDE
  if (!mcp.begin_I2C(0x20, &I2C1))
  {
    failsafe(); // go into failsafe
  }
#endif

  mcp.pinMode(INTERUPT_PIN, INPUT);            // Set the interrupt pin as an input
  mcp.setupInterrupts(true, false, HIGH);      // Set interrupt to active-low, open drain mode
  mcp.setupInterruptPin(INTERUPT_PIN, CHANGE); // Set pin to trigger interrupt on state change

  for (int i = 0; i < 16; i++)
  {
    mcp.pinMode(i, INPUT_PULLUP);
  }
#pragma endregion

#ifdef HID
  usb_hid.begin();

  // wait until device mounted
  while (!TinyUSBDevice.mounted())
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
#endif
}

void loop()
{
}

void switchChange()
{
  uint16_t pinValues = readMCP();

#ifdef HID
  hidOut(pinValues);
#endif
}

#ifdef HID
void hidOut(uint16_t pinValues)
{
  uint8_t const report_id = 0;
  uint8_t const modifier = 0;

  uint8_t keycode[16] = {0};

  for (int i = 0; i < 16; i++)
  {
    if (pinValues & (1 << i))
    {
      keycode[i] = keycodes[i];
    }
  }
  Serial.println("");

  usb_hid.keyboardReport(report_id, modifier, keycode);
}
#endif

uint16_t readMCP()
{
  uint16_t pinValues = mcp.readGPIOAB(); // Read the pin values from the MCP23017

#ifdef SERIAL_OUT
  Serial.print("interrupt: ");
  Serial.print(pinValues, BIN);
  Serial.println("");
#endif

  return pinValues;
}

void failsafe(String error = "Unknown error")
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