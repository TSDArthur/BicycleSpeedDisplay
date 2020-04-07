#ifndef BICYCLEMANAGER_H
#define BICYCLEMANAGER_H

#include "Arduino.h"
#include "PinChangeInt.h"

#define MICROS_PER_SEC 1000000L
#define SEC_PER_MIN 60L

struct EncoderManager
{
    void (*EncoderISP)();
    volatile unsigned char pinEncoderPinA;
    volatile unsigned char pinEncoderPinB;
    long encoderPPR;
    volatile bool firstPulse;
    volatile unsigned long pulseCounter;
    volatile int SpeedPPS;
    volatile unsigned long lastPulseCounter;
    volatile unsigned long lastSampleTime;
};

#define MotorState(x, y, a, b, c, d)                                                             \
    void y();                                                                                    \
    struct EncoderManager x = {y, a, b, c, true, 0, 0, 0, 0};                                    \
    void y()                                                                                     \
    {                                                                                            \
        static bool currentDirection = true;                                                     \
        static unsigned long pulseStartMicros = micros();                                        \
        static unsigned long pulseEndMicros = micros();                                          \
        pulseEndMicros = micros();                                                               \
        if (!x.firstPulse && pulseEndMicros > pulseStartMicros)                                  \
        {                                                                                        \
            x.motorSpeedPPS = MICROS_PER_SEC / (pulseEndMicros - pulseStartMicros);              \
            currentDirection = !(digitalRead(x.pinEncoderPinA) ^ digitalRead(x.pinEncoderPinB)); \
            x.SpeedPPS = currentDirection ? x.motorSpeedPPS : -1 * x.motorSpeedPPS;              \
            x.pulseCounter++;                                                                    \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            x.firstPulse = false;                                                                \
        }                                                                                        \
        pulseStartMicros = pulseEndMicros;                                                       \
    }

class Bicycle
{
private:
    unsigned char pinDirA;
    unsigned char pinDirB;

public:
    struct EncoderManager *encoderManager;
    Motor(int _pinDirA, int _pinDirB,
          struct EncoderManager *_encoderManager);
    int GetSpeedRPM();
    int GetSpeedPPS();
    void Handle();
};

#endif
