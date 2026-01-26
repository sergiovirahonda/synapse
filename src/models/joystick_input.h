#ifndef JOYSTICK_INPUT_H
#define JOYSTICK_INPUT_H


class JoystickTrim {
    private:
        int pitch;
        int roll;
        int yaw;
        int throttle;
        float throttleDeltaFactor;
        int throttleDeadZone;

    public:
        JoystickTrim(
            int pitch,
            int roll,
            int yaw,
            int throttle
        );
        // Setters
        void setPitch(int pitch);
        void setRoll(int roll);
        void setYaw(int yaw);
        void setThrottle(int throttle);
        void reset();
        // Getters
        int getPitch();
        int getRoll();
        int getYaw();
        int getThrottle();
        float getThrottleDeltaFactor();
        int getThrottleDeadZone();
};

class JoystickInput {
    private:
        int pitch;
        int roll;
        int yaw;
        int throttle;

    public:
        JoystickInput(int pitch, int roll, int yaw, int throttle);
        // Setters
        void setPitch(int pitch);
        void setRoll(int roll);
        void setYaw(int yaw);
        void setThrottle(int throttle);
        void reset();
        // Getters
        int getPitch();
        int getRoll();
        int getYaw();
        int getThrottle();
};

#endif
