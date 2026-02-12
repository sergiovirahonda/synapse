// External Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

// Internal Libraries
#include "models/drone_command.h"
#include "models/joystick_input.h"
#include "adapters/radio_adapter.h"
#include "adapters/joystick_adapter.h"

// Global drivers
USB     Usb;
USBHub  Hub(&Usb);
HIDUniversal Hid(&Usb);
T16000MParser joyParser;

// Define radio address & pins
byte NRF_ADDRESS[6] = "00001";
const int NRF_CE_PIN = 7;   // Connect NRF24L01 CE to Arduino D7
const int NRF_CSN_PIN = 8;  // Connect NRF24L01 CSN to Arduino D8

// ---------------------------------------------------------
// SCREEN DEFINITIONS
// ---------------------------------------------------------
#define OLED_ADDR 0x3C

static unsigned long lastScreenUpdate = 0;

// Define adapters
RadioAdapter radioAdapter(NRF_CE_PIN, NRF_CSN_PIN, NRF_ADDRESS);
SSD1306AsciiWire display;

// Global variables
DroneCommand droneCommand(0, 0, 0, 0, 0, 0, 0, 0);
TelemetryData telemetryData(0, 0, 0);

void setup() {
    Serial.begin(9600);

    // 1. Initialize OLED
    Wire.begin();
    Wire.setClock(400000L); // Fast I2C
    display.begin(&Adafruit128x64, OLED_ADDR);
    display.setFont(Adafruit5x7);
    display.clear();
    display.println(F("Synapse TX starting..."));

    // 2. Initialize Radio
    display.println(F("Init radio..."));
    radioAdapter.begin();
    if (!radioAdapter.isChipConnected()) {
        display.println(F("Radio chip FAILED!"));
        while (1);
    }
    display.println(F("Radio OK."));

    // 3. Initialize USB Host Shield
    display.println(F("Init USB Host..."));
    if (Usb.Init() == -1) {
        display.println(F("USB Host FAILED!"));
        while (1);
    }
    display.println(F("USB Host OK."));
    Hid.SetReportParser(0, &joyParser);

    display.println(F("All systems ready."));
    delay(1000);
    display.clear(); // Wipe boot messages before entering loop
}

void loop() {
    Usb.Task();
    droneCommand.setPitch(joyParser.getPitch());
    droneCommand.setRoll(joyParser.getRoll());
    droneCommand.setYaw(joyParser.getYaw());
    droneCommand.setThrottle(joyParser.getThrottle());
    droneCommand.setPitchTrim(joyParser.getPitchTrim());
    droneCommand.setRollTrim(joyParser.getRollTrim());
    droneCommand.setYawTrim(joyParser.getYawTrim());
    droneCommand.setTrimReset(joyParser.getTrimReset());
    bool commandSent = radioAdapter.send(droneCommand);
    bool telemetryReceived = radioAdapter.receiveTelemetry(telemetryData);
    if (millis() - lastScreenUpdate > 500) {
        display.setCursor(0, 0);

        display.println(F("  -- SYSTEM READY --"));
        display.print(F("T: "));      display.print(droneCommand.getThrottle());
        display.print(F(" | Sent: ")); display.print(commandSent);
        display.clearToEOL();          display.println(); display.println();
        display.println(F("   -- TELEMETRY --"));
        display.print(F("Ack: "));    display.print(telemetryReceived);
        display.print(F(" | PWM: ")); display.print(telemetryData.getPwm());
        display.clearToEOL();          display.println();
        display.print(F("Roll: "));   display.print(telemetryData.getRoll());
        display.print(F(" | Pitch: ")); display.print(telemetryData.getPitch());
        display.clearToEOL();          display.println();

        lastScreenUpdate = millis();
    }
    delay(10);
}
