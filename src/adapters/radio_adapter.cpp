#include "radio_adapter.h"
#include <SPI.h>

RadioAdapter::RadioAdapter(int cePin, int csnPin, byte* address) {
    this->cePin = cePin;
    this->csnPin = csnPin;
    this->address = address;
    this->radio = RF24(cePin, csnPin);
}

void RadioAdapter::begin() {
    bool started = this->radio.begin();
    if (!started) {
        // Handle radio initialization failure
        Serial.println("Radio hardware not responding!");
        while (1);
    }
    this->radio.setAutoAck(true);
    this->radio.enableAckPayload();
    this->radio.setPALevel(RF24_PA_HIGH);  // Match TX side
    this->radio.setDataRate(RF24_250KBPS); // Longest Range
    this->radio.setChannel(108); // Avoid WiFi interference (Above 2.48GHz) 
    this->radio.openWritingPipe(this->address);
    this->radio.stopListening(); // TX mode
}

bool RadioAdapter::isChipConnected() {
    return this->radio.isChipConnected();
}

bool RadioAdapter::send(const DroneCommand& packet) {
    return this->radio.write(&packet, sizeof(packet));
}

bool RadioAdapter::receiveTelemetry(TelemetryData& data) {
    if (radio.isAckPayloadAvailable()) {
        radio.read(&data, sizeof(data));
        return true;
    }
    return false;
}