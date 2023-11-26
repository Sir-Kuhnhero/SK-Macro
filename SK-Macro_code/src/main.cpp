// #include <Arduino.h>
#include "Adafruit_TinyUSB.h"
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

// #define HID

#ifdef HID
// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor
uint8_t const desc_hid_report[] =
    {
        TUD_HID_REPORT_DESC_KEYBOARD()};

// USB HID object. For ESP32 these values cannot be changed after this declaration
// desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);
#endif

Adafruit_MCP23X17 mcp; // Create MCP23017 device
MbedI2C I2C1(6, 7);    // Create a new I2C bus on pins 6 and 7 (SDA, SCL)

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

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

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

  if (mcp.begin_I2C(0x20, &I2C1)) // use default address 0
  {
    Serial.println("mcp.begin_I2C(0) success");
  }
  else
  {
    Serial.println("mcp.begin_I2C(0) failed");

    while (true)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }

  for (int i = 0; i < 16; i++)
  {
    mcp.pinMode(i, INPUT_PULLUP);
  }

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

  bool pressedKeys[16] = {false};

  for (int i = 0; i < 16; i++)
  {
    pressedKeys[i] = !mcp.digitalRead(i);
  }

  uint8_t const report_id = 0;
  uint8_t const modifier = 0;

  uint8_t keycode[16] = {0};

  bool output = false;

  for (int i = 0; i < 16; i++)
  {
    if (pressedKeys[i])
    {
      Serial.print("Pressed key: ");
      Serial.print(i);
      Serial.print("; ");

      keycode[i] = keycodes[i];

      output = true;
    }
  }

  if (output)
  {
    Serial.println("");
  }

#ifdef HID
  usb_hid.keyboardReport(report_id, modifier, keycode);
  delay(10);
  usb_hid.keyboardRelease(0);
#endif

  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
}