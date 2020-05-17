#ifndef CORE_H
#define CORE_H

#include "Arduino.h"
#include "Config.h"
#include "Communication.h"
#include "Hardware.h"

void SystemInitialize();
void SensorSamplingTask(void *parameter);
void UDPCommunicationTask(void *parameter);
void LEDDisplayTask(void *parameter);

#endif
