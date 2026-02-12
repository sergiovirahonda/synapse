// External Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <Wire.h>

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

// Define radio CE & CSN pins
byte NRF_ADDRESS[6] = "00001";

// Global variables

// Define radio CE & CSN pins
const int NRF_CE_PIN = 7;   // Connect NRF24L01 CE to Arduino D7
const int NRF_CSN_PIN = 8;  // Connect NRF24L01 CSN to Arduino D8

// ---------------------------------------------------------
// SCREEN DEFINITIONS
// ---------------------------------------------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C // Almost all 0.96" OLEDs are 0x3C

// Define adapters
RadioAdapter radioAdapter(NRF_CE_PIN, NRF_CSN_PIN, NRF_ADDRESS);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Global variables

DroneCommand droneCommand(0, 0, 0, 0, 0, 0, 0, 0);
TelemetryData telemetryData(0, 0, 0);

void setup() {
    Serial.begin(9600);

    // 2. Initialize OLED
    // 2. Initialize OLED
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("SSD1306 allocation failed. Checking I2C connection..."));
        for(;;); // Don't proceed, loop forever
    }
    // 4. Set Text Properties
    display.clearDisplay();
    display.invertDisplay(false);
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0,0); 

    // 2. Initialize Drivers (BUT DO NOT ARM YET)
    display.println("Initializing adapters...");
    radioAdapter.begin();
    if (!radioAdapter.isChipConnected()){
        display.println("Radio hardware failed.");
        display.println("Radio chip not connected properly!");
        display.display();
        while(1);

    }
    if (Usb.Init() == -1) {
        display.println("USB Host Shield initialization failed!");
        display.display();
        while (1);
    }
    display.println("USB Host Shield initialized.");
    Hid.SetReportParser(0, &joyParser);
    display.display();
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
    display.println(" -- SYSTEM READY -- ");
    display.print("T: "); display.print(droneCommand.getThrottle());
    bool commandSent = radioAdapter.send(droneCommand);
    display.print(" | Sent: "); display.println((commandSent));
    bool telemetryReceived = radioAdapter.receiveTelemetry(telemetryData);
    display.print("Ack: "); display.print((telemetryReceived));
    display.print(" | PWM: "); display.print(telemetryData.getPwm());
    display.print(" | Roll: "); display.print(telemetryData.getRoll());
    display.print(" | Pitch: "); display.println(telemetryData.getPitch());
    display.display();
    delay(10);
}