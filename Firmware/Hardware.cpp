#include "Hardware.h"

Sensor::Sensor(struct SensorBlock *_sensorBlock)
{
    sensorBlock = _sensorBlock;
    pinMode(sensorBlock->sensorPin, HARDWARE_SENSOR_TYPE == 0 ? INPUT_PULLUP : INPUT_PULLDOWN);
}

void Sensor::DoEvents()
{
    static uint64_t lastSensorTrigTime = millis();
    uint64_t currentTimeMS = millis();
    if (digitalRead(sensorBlock->sensorPin) == HARDWARE_SENSOR_TYPE)
    {
        if (!sensorBlock->firstPulse && currentTimeMS > lastSensorTrigTime)
        {
            sensorBlock->pulseSpeed = HARDWARE_SENSOR_MSPS / (currentTimeMS - lastSensorTrigTime);
            sensorBlock->pulseCounter++;
        }
        else
        {
            sensorBlock->firstPulse = false;
        }
        while (digitalRead(sensorBlock->sensorPin) == HARDWARE_SENSOR_TYPE)
        {
            if (millis() - currentTimeMS >= HARDWARE_SENSOR_DEADAREA_TIMEOUT)
            {
                break;
            }
        }
        lastSensorTrigTime = currentTimeMS;
    }
    if (currentTimeMS - sensorBlock->lastSampleTime > HARDWARE_SENSOR_TOZEROTIME)
    {
        if (sensorBlock->lastPulseCounter == sensorBlock->pulseCounter)
        {
            sensorBlock->firstPulse = true;
            sensorBlock->pulseSpeed = 0;
        }
        sensorBlock->lastPulseCounter = sensorBlock->pulseCounter;
        sensorBlock->lastSampleTime = currentTimeMS;
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
