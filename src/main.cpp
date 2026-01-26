// External Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>
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

// Define radio CE & CSN pins
byte NRF_ADDRESS[6] = "00001";

// Global variables

// Define radio CE & CSN pins
const int NRF_CE_PIN = 7;   // Connect NRF24L01 CE to Arduino D7
const int NRF_CSN_PIN = 8;  // Connect NRF24L01 CSN to Arduino D8

// Define drivers
RadioAdapter radioAdapter(NRF_CE_PIN, NRF_CSN_PIN, NRF_ADDRESS);

// // Define services
// RadioService radioService(radioDriver);

DroneCommand droneCommand(0, 0, 0, 0);

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing Drone TX Controller...");
    radioAdapter.begin();
    if (!radioAdapter.isChipConnected()){
        Serial.println("Radio chip not connected properly!");
        while(1);

    }
    if (Usb.Init() == -1) {
        Serial.println("USB Host Shield initialization failed!");
        digitalWrite(6, LOW);
        while (1);
    }
    Serial.println("USB Host Shield initialized.");
    Hid.SetReportParser(0, &joyParser);
}

void loop() {
    Usb.Task();
    droneCommand.setPitch(joyParser.getPitch());
    droneCommand.setRoll(joyParser.getRoll());
    droneCommand.setYaw(joyParser.getYaw());
    droneCommand.setThrottle(joyParser.getThrottle());
    Serial.print("Sending command: ");
    Serial.print("Pitch: "); Serial.print(droneCommand.getPitch());
    Serial.print(" | Roll: "); Serial.print(droneCommand.getRoll());
    Serial.print(" | Yaw: "); Serial.print(droneCommand.getYaw());
    Serial.print(" | Throttle: "); Serial.println(droneCommand.getThrottle());
    bool commandSent = radioAdapter.send(droneCommand);
    Serial.print("Command sent: "); Serial.println(commandSent);
    delay(10);
}