#ifndef JOYSTICK_ADAPTER_H
#define JOYSTICK_ADAPTER_H

#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <Arduino.h>

class T16000MParser : public HIDReportParser {
public:
    // Setters
    void setPitch(int pitch);
    void setRoll(int roll);
    void setYaw(int yaw);
    void setThrottle(int throttle);
    void setPitchTrim(int pitchTrime);
    void setRollTrim(int rollTrim);
    void setYawTrim(int yawTrim);
    void setTrimReset(int reset);
    void reset();
    
    // Getters
    int getPitch();
    int getRoll();
    int getYaw();
    int getThrottle();
    int getPitchTrim();
    int getRollTrim();
    int getYawTrim();
    int getTrimReset();
    
    // Override Parse method
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) override;

private:
    int pitch;
    int roll;
    int yaw;
    int throttle;
    int pitchTrim;
    int rollTrim;
    int yawTrim;
    int trimReset;
    int mapAxis(int value, int minVal, int maxVal, int center);
};

#endif