#ifndef PWM_DEVICE_H
#define PWM_DEVICE_H

#include<Arduino.h>

class pwmDevice
{
    private:
    uint8_t _pin;
    uint8_t _channel;
    int _currentLevel;
    int _targetLevel;

    public:
    pwmDevice(uint8_t pin, uint8_t channel);
    void begin(uint32_t freq, uint8_t res);
    void setTarget(int target);
    void update();
    int getCurrentLevel();
};

#endif
