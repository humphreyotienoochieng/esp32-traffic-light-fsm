#include "PWMDevice.h"

pwmDevice::pwmDevice(uint8_t pin, uint8_t channel)
{
    _pin = pin;
    _channel = channel;
    _currentLevel = 0;
    _targetLevel = 0;

}

void pwmDevice::begin(uint32_t freq, uint8_t res)
{
    ledcSetup(_channel, freq, res);
    ledcAttachPin(_pin, _channel);
    ledcWrite(_channel, 0);
}

void pwmDevice::setTarget(int target)
{
    _targetLevel = target;
}

void pwmDevice::update()
{
    if(_currentLevel < _targetLevel)
    {
        _currentLevel++;
    }
    else if (_currentLevel > _targetLevel)
    {
        _currentLevel--;
    }
    ledcWrite(_channel, _currentLevel);
}

int pwmDevice::getCurrentLevel()
{
    return _currentLevel;
}