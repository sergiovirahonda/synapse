#include "drone_command.h"
#include <Arduino.h>

// =================================================================
// CONTROL LIMITS CONSTANTS
// =================================================================
const int16_t MAX_ANGLE_DEGREES = 30;   
const int16_t MAX_YAW_RATE_DPS = 120;   
const int16_t MIN_THROTTLE_PWM = 200;   // Updated: 200 is safer idle than 48
const int16_t MAX_THROTTLE_PWM = 2047;  

DroneCommand::DroneCommand(int16_t pitch, int16_t roll, int16_t yaw, int16_t throttle) {
    this->pitch = pitch;
    this->roll = roll;
    this->yaw = yaw;
    this->throttle = throttle;
};

// --- SETTERS ---
void DroneCommand::setPitch(int16_t pitch) { this->pitch = pitch; }
void DroneCommand::setRoll(int16_t roll)   { this->roll = roll; }
void DroneCommand::setYaw(int16_t yaw)     { this->yaw = yaw; }
void DroneCommand::setThrottle(int16_t throttle) { this->throttle = throttle; }

void DroneCommand::reset() {
    pitch = 0; roll = 0; yaw = 0; throttle = 0;
}

// --- GETTERS ---
int16_t DroneCommand::getPitch()    { return this->pitch; }
int16_t DroneCommand::getRoll()     { return this->roll; }
int16_t DroneCommand::getYaw()      { return this->yaw; }
int16_t DroneCommand::getThrottle() { return this->throttle; }

// --- RADIO HELPERS (The Bridge) ---
DronePacket DroneCommand::createPacket() {
    DronePacket pkt;
    pkt.pitch = this->pitch;
    pkt.roll = this->roll;
    pkt.yaw = this->yaw;
    pkt.throttle = this->throttle;
    return pkt;
}

void DroneCommand::loadFromPacket(DronePacket pkt) {
    this->pitch = pkt.pitch;
    this->roll = pkt.roll;
    this->yaw = pkt.yaw;
    this->throttle = pkt.throttle;
}