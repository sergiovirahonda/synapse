#include "joystick_adapter.h"

// Setters
void T16000MParser::setPitch(int pitch) {
    this->pitch = pitch;
}

void T16000MParser::setRoll(int roll) {
    this->roll = roll;
}

void T16000MParser::setYaw(int yaw) {
    this->yaw = yaw;
}

void T16000MParser::setThrottle(int throttle) {
    this->throttle = throttle;
}

void T16000MParser::reset() {
    this->pitch = 0;
    this->roll = 0;
    this->yaw = 0;
    this->throttle = 0;
}

// Getters
int T16000MParser::getPitch() {
    return this->pitch;
}

int T16000MParser::getRoll() {
    return this->roll;
}

int T16000MParser::getYaw() {
    return this->yaw;
}

int T16000MParser::getThrottle() {
    return this->throttle;
}

// Override Parse method
void T16000MParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
    if (len < 9) return;

    // Raw values
    // X and Y are 16-bit
    int rawRoll = (buf[4] << 8) | buf[3];  // forward/back
    int rawPitch = (buf[6] << 8) | buf[5];   // left/right
    // Yaw is 8-bit (buf[7])
    uint8_t rawYaw = buf[7];
    // Throttle is inverted in buf[8]
    uint8_t rawThrottle = 255 - buf[8];

    // --- Center and scale constants ---
    const int16_t centerAxis = 8192;     // neutral value for roll/pitch
    const int16_t maxAxis = 16383;       // max 14-bit value
    const int16_t minAxis = 0;

    // --- Normalize to -100 ... +100 ---
    int roll = mapAxis(rawRoll, minAxis, maxAxis, centerAxis);
    int pitch = mapAxis(rawPitch, minAxis, maxAxis, centerAxis);
    int yaw = mapAxis(rawYaw, 0, 255, 128); // yaw is 8-bit
    int throttle = map(rawThrottle, 0, 255, 0, 100); // 0 → 100%
    // Set normalized values
    setRoll(roll);
    setPitch(pitch);
    setYaw(yaw);
    setThrottle(throttle);
}

// Custom map function for -100..+100 normalization
int T16000MParser::mapAxis(int value, int minVal, int maxVal, int center) {
    if (value >= center)
        return (int)(((float)(value - center) / (maxVal - center)) * 100.0f);
    else
        return (int)(((float)(value - center) / (center - minVal)) * 100.0f);
}