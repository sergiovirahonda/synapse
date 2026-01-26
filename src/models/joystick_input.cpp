#include "joystick_input.h"

JoystickTrim::JoystickTrim(
    int pitch,
    int roll,
    int yaw,
    int throttle
) {
    this->pitch = pitch;
    this->roll = roll;
    this->yaw = yaw;
    this->throttle = throttle;
    this->throttleDeltaFactor = 0.03;
    this->throttleDeadZone = 1;
}

// Setters

void JoystickTrim::reset() {
    pitch = 0;
    roll = 0;
    yaw = 0;
    throttle = 0;
}

void JoystickTrim::setPitch(int pitch) {
    this->pitch = pitch;
}

void JoystickTrim::setRoll(int roll) {
    this->roll = roll;
}

void JoystickTrim::setYaw(int yaw) {
    this->yaw = yaw;
}

void JoystickTrim::setThrottle(int throttle) {
    this->throttle = throttle;
}

// Getters

int JoystickTrim::getPitch() {
    return this->pitch;
}

int JoystickTrim::getRoll() {
    return this->roll;
}

int JoystickTrim::getYaw() {
    return this->yaw;
}

int JoystickTrim::getThrottle() {
    return this->throttle;
}

float JoystickTrim::getThrottleDeltaFactor() {
    return this->throttleDeltaFactor;
}

int JoystickTrim::getThrottleDeadZone() {
    return this->throttleDeadZone;
}

JoystickInput::JoystickInput(
    int pitch,
    int roll,
    int yaw,
    int throttle
) {
    this->pitch = pitch;
    this->roll = roll;
    this->yaw = yaw;
    this->throttle = throttle;
}

// Setters

void JoystickInput::reset() {
    pitch = 0;
    roll = 0;
    yaw = 0;
    throttle = 0;
}

void JoystickInput::setPitch(int pitch) {
    this->pitch = pitch;
}
    
void JoystickInput::setRoll(int roll) {
    this->roll = roll;
}

void JoystickInput::setYaw(int yaw) {
    this->yaw = yaw;
}

void JoystickInput::setThrottle(int throttle) {
    this->throttle = throttle;
}

// Getters

int JoystickInput::getPitch() {
    return this->pitch;
}

int JoystickInput::getRoll() {
    return this->roll;
}

int JoystickInput::getYaw() {
    return this->yaw;
}

int JoystickInput::getThrottle() {
    return this->throttle;
}