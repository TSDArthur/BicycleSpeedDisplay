#ifndef HARDWARE_H
#define HARDWARE_H

#include "Arduino.h"
#include "Config.h"

struct SensorBlock
{
    void (*SensorISP)();
    xQueueHandle speedInfo;
    volatile uint8_t sensorPin;
    volatile bool firstPulse;
    volatile uint16_t pulseSpeed;
    volatile uint32_t pulseCounter;
    volatile uint32_t lastPulseCounter;
    volatile uint32_t lastSampleTime;
};

#define SensorState(structName, sensorISPName)                                                  \
    void sensorISPName();                                                                       \
    portMUX_TYPE muxSensorISP;                                                                  \
    struct SensorBlock structName = {sensorISPName, xQueueCreate(1, sizeof(int)),               \
                                     HARDWARE_SENSOR_PIN, true, 0, 0, 0, 0};                    \
    void sensorISPName()                                                                        \
    {                                                                                           \
        portENTER_CRITICAL_ISR(&muxSensorISP);                                                  \
        static uint32_t pulseStartMillis = millis();                                            \
        static uint32_t pulseEndMillis = millis();                                              \
        pulseEndMillis = millis();                                                              \
        if (!structName.firstPulse && pulseEndMillis > pulseStartMillis)                        \
        {                                                                                       \
            structName.pulseSpeed = HARDWARE_SENSOR_MSPS / (pulseEndMillis - pulseStartMillis); \
            structName.pulseCounter++;                                                          \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            structName.firstPulse = false;                                                      \
        }                                                                                       \
        pulseStartMillis = pulseEndMillis;                                                      \
        portEXIT_CRITICAL_ISR(&muxSensorISP);                                                   \
    }

class Sensor
{
public:
    struct SensorBlock *sensorBlock;
    Sensor(struct SensorBlock *_sensorBlock);
    void DoEvents();
};

class LED
{
private:
    uint8_t pinLEDPWR;
    uint8_t pinLEDConn;

public:
    LED(uint8_t _pinLEDPWR = HARDWARE_PWRLED_PIN, uint8_t _pinLEDConn = HARDWARE_CONLED_PIN);
    void SetLEDPWR(bool state);
    void SetLEDConn(bool state);
};

#endif
