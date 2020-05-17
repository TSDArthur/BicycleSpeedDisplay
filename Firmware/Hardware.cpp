#include "Hardware.h"

Sensor::Sensor(struct SensorBlock *_sensorBlock)
{
    sensorBlock = _sensorBlock;
    pinMode(sensorBlock->sensorPin, HARDWARE_SENSOR_TYPE == 0 ? INPUT_PULLUP : INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(sensorBlock->sensorPin), sensorBlock->SensorISP,
                    HARDWARE_SENSOR_TYPE == 0 ? FALLING : RISING);
}

void Sensor::DoEvents()
{
    if(millis() - sensorBlock->lastSampleTime > HARDWARE_SENSOR_TOZEROTIME)
    {
        if(sensorBlock->lastPulseCounter == sensorBlock->pulseCounter)
        {
            sensorBlock->firstPulse = true;
            sensorBlock->pulseSpeed = 0;
        }
        sensorBlock->lastPulseCounter = sensorBlock->pulseCounter;
        sensorBlock->lastSampleTime = millis();
    }
}

LED::LED(uint8_t _pinLEDPWR, uint8_t _pinLEDConn)
{
    pinLEDPWR = _pinLEDPWR;
    pinLEDConn = _pinLEDConn;
    pinMode(pinLEDPWR, OUTPUT);
    pinMode(pinLEDConn, OUTPUT);
    digitalWrite(pinLEDPWR, HIGH);
    digitalWrite(pinLEDConn, LOW);
}

void LED::SetLEDPWR(bool state)
{
    digitalWrite(pinLEDPWR, state);
}

void LED::SetLEDConn(bool state)
{
    digitalWrite(pinLEDConn, state);
}
