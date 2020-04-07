#include "BicycleManager.h"

Motor::Motor(int _pinDirA, int _pinDirB,
             struct MotorEncoderManager *_motorEncoderManager)
{
    motorEncoderManager = _motorEncoderManager;

    //Setup Pins
    pinDirA = _pinDirA;
    pinDirB = _pinDirB;
    pinPWM = _pinPWM;
    pinMode(pinDirA, OUTPUT);
    pinMode(pinDirB, OUTPUT);
    pinMode(pinPWM, OUTPUT);
    pinMode(motorEncoderManager->pinEncoderPinA, INPUT);
    pinMode(motorEncoderManager->pinEncoderPinB, INPUT);

    //Attach Interrputs
    int pinToInterrupt = 0;
    attachInterrupt(digitalPinToInterrupt(motorEncoderManager->pinEncoderPinA), motorEncoderManager->EncoderISP, CHANGE);

    //Setup PID Control
    motorPID = _motorPID;

    //Reset Motor
    (*this) = 0;
}

void Motor::SetPWM(int pwmValue)
{
    digitalWrite(pinDirA, pwmValue >= 0 ? motorEncoderManager->motorLeft : !motorEncoderManager->motorLeft);
    digitalWrite(pinDirB, pwmValue >= 0 ? !motorEncoderManager->motorLeft : motorEncoderManager->motorLeft);
    pwmValue = pwmValue < 0 ? -1 * pwmValue : pwmValue;
    analogWrite(pinPWM, pwmValue);
}

void Motor::SetSpeedRPM(int targetSpeed)
{
    SetSpeedPPS((int)((long)(targetSpeed * motorEncoderManager->motorPPR) / SEC_PER_MIN));
}

void Motor::SetSpeedPPS(int targetSpeed)
{
    motorEncoderManager->motorTargetSpeedPPS = targetSpeed;
}

int Motor::GetSpeedRPM()
{
    return (int)((long)(motorEncoderManager->motorSpeedPPS * SEC_PER_MIN / motorEncoderManager->motorPPR));
}

int Motor::GetSpeedPPS()
{
    return motorEncoderManager->motorSpeedPPS;
}

void Motor::Handle()
{
    if (millis() - motorEncoderManager->lastSampleTime > 100)
    {
        if (motorEncoderManager->lastPulseCounter == motorEncoderManager->pulseCounter)
        {
            motorEncoderManager->firstPulse = true;
            motorEncoderManager->motorSpeedPPS = 0;
        }
        motorEncoderManager->lastPulseCounter = motorEncoderManager->pulseCounter;
        motorEncoderManager->lastSampleTime = millis();
    }
}

