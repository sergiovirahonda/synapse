#include "drone_command.h"
#include <Arduino.h>

// =================================================================
// CONTROL LIMITS CONSTANTS
// =================================================================
const int16_t MAX_ANGLE_DEGREES = 30;   
const int16_t MAX_YAW_RATE_DPS = 120;   
const int16_t MIN_THROTTLE_PWM = 200;   // Updated: 200 is safer idle than 48
const int16_t MAX_THROTTLE_PWM = 2047;

DroneCommand::DroneCommand(
    int16_t pitch,
    int16_t roll,
    int16_t yaw,
    int16_t throttle,
    int16_t rollTrim,
    int16_t pitchTrim,
    int16_t yawTrim,
    uint8_t trimReset
) {
    this->pitch = pitch;
    this->roll = roll;
    this->yaw = yaw;
    this->rollTrim = rollTrim;
    this->pitchTrim = pitchTrim;
    this->yawTrim = yawTrim;
    this->trimReset = trimReset;
};

// --- SETTERS ---
void DroneCommand::setPitch(int16_t pitch) { this->pitch = pitch; }
void DroneCommand::setRoll(int16_t roll)   { this->roll = roll; }
void DroneCommand::setYaw(int16_t yaw)     { this->yaw = yaw; }
void DroneCommand::setThrottle(int16_t throttle) { this->throttle = throttle; }
void DroneCommand::setRollTrim(int16_t rollTrim) { this->rollTrim = rollTrim; }
void DroneCommand::setPitchTrim(int16_t pitchTrim) { this->pitchTrim = pitchTrim; }
void DroneCommand::setYawTrim(int16_t yawTrim) { this->yawTrim = yawTrim; }
void DroneCommand::setTrimReset(uint8_t trimReset) { this->trimReset = trimReset; }
void DroneCommand::reset() {
    pitch = 0; roll = 0; yaw = 0; throttle = 0;
}

// --- GETTERS ---
int16_t DroneCommand::getPitch()    { return this->pitch; }
int16_t DroneCommand::getRoll()     { return this->roll; }
int16_t DroneCommand::getYaw()      { return this->yaw; }
int16_t DroneCommand::getThrottle() { return this->throttle; }
int16_t DroneCommand::getRollTrim() { return this->rollTrim; }
int16_t DroneCommand::getPitchTrim() { return this->pitchTrim; }
int16_t DroneCommand::getYawTrim() { return this->yawTrim; }
uint8_t DroneCommand::getTrimReset() { return this->trimReset; }

// --- RADIO HELPERS (The Bridge) ---
DronePacket DroneCommand::createPacket() {
    DronePacket pkt;
    pkt.pitch = this->pitch;
    pkt.roll = this->roll;
    pkt.yaw = this->yaw;
    pkt.throttle = this->throttle;
    pkt.rollTrim = this->rollTrim;
    pkt.pitchTrim = this->pitchTrim;
    pkt.yawTrim = this->yawTrim;
    pkt.trimReset = this->trimReset;
    return pkt;
}

void DroneCommand::loadFromPacket(DronePacket pkt) {
    this->pitch = pkt.pitch;
    this->roll = pkt.roll;
    this->yaw = pkt.yaw;
    this->throttle = pkt.throttle;
    this->rollTrim = pkt.rollTrim;
    this->pitchTrim = pkt.pitchTrim;
    this->yawTrim = pkt.yawTrim;
    this->trimReset = pkt.trimReset;
}

// =================================================================
// TelemetryData
// =================================================================

TelemetryData::TelemetryData(int16_t pwm, int16_t roll, int16_t pitch) {
    this->pwm = pwm;
    this->roll = roll;
    this->pitch = pitch;
}

int16_t TelemetryData::getPwm()   { return this->pwm; }
int16_t TelemetryData::getRoll()  { return this->roll; }
int16_t TelemetryData::getPitch() { return this->pitch; }

void TelemetryData::setPwm(int16_t pwm)     { this->pwm = pwm; }
void TelemetryData::setRoll(int16_t roll)   { this->roll = roll; }
void TelemetryData::setPitch(int16_t pitch) { this->pitch = pitch; }

void TelemetryData::reset() {
    pwm = 0;
    roll = 0;
    pitch = 0;
}

TelemetryPacket TelemetryData::createPacket() {
    TelemetryPacket pkt;
    pkt.pwm = this->pwm;
    pkt.roll = this->roll;
    pkt.pitch = this->pitch;
    return pkt;
}

void TelemetryData::loadFromPacket(TelemetryPacket pkt) {
    this->pwm = pkt.pwm;
    this->roll = pkt.roll;
    this->pitch = pkt.pitch;
}