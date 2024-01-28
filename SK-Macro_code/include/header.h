#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <SoftwareSerial.h>
#include "Adafruit_TinyUSB.h"

// #define SERIAL_OUT // used for debugging and not compatible with tinyUSB

#define INTERUPT_PIN 29

extern uint8_t const desc_hid_report[];
extern Adafruit_USBD_HID usb_hid;
extern uint8_t keycodes[][6];