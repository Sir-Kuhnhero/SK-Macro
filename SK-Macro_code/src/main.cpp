#include "header.h"
#include "keymap.h"

#ifdef HID
uint8_t const desc_hid_report[] = {TUD_HID_REPORT_DESC_KEYBOARD()};
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);
Adafruit_USBD_MSC usb_msc;
#endif

#ifdef MCP23017
Adafruit_MCP23X17 mcp;
TwoWire I2C1(i2c1, 6, 7); // Create a new I2C bus on pins 6 and 7 (SDA, SCL)

uint16_t pinValues = 0xFFFF;
#endif

void switchChange();
void readMCP();
void failsafe(String error);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INTERUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERUPT_PIN), switchChange, LOW);
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

  // Read the state of the first button
  readMCP();

  // If the first button is pressed, mount the drive
  if ((pinValues & 0x01) == 0)
  {
    usb_msc.setUnitReady(true);
  }
#endif
}

void loop()
{
#ifdef HID &&MCP23017
  uint8_t const report_id = 0;
  uint8_t const modifier = 0;

  uint8_t keycode[6] = {0}; // you can send a maximum of 6 keys at a time
  int codeIndex = 0;

  for (int i = 0; i < 16; i++)
  {
    if (!(pinValues & (1 << i)))
    {
      if (codeIndex <= 5)
      {
        for (int j = 0; j < int(sizeof(keycodes[i]) / sizeof(keycodes[i][0])); j++)
        {
          if (keycodes[i][j] == 0)
          {
            break;
          }
          keycode[codeIndex] = keycodes[i][j];
          codeIndex++;
        }
        // keycode[codeIndex] = keycodes[i];
        // codeIndex++;
      }
    }
  }

  usb_hid.keyboardReport(report_id, modifier, keycode);

#endif
}

void switchChange()
{
  readMCP();
}

void readMCP()
{
#ifdef MCP23017
  pinValues = mcp.readGPIOAB(); // Read the pin values from the MCP23017
#endif
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