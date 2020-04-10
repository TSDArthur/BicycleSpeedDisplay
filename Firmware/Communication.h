#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "Core.h"
#include "HardwareSerial.h"
#include "Wire.h"

//Defines
#define COMM_START_SYMBOL 0x7f
#define COMM_SPEC_SYMBOL 0x7e
#define COMM_SET_SYMBOL 0x7d
#define COMM_END_SYMBOL 0x7c
#define COMM_CONNECT_SPEC_NUM 2
#define COMM_TRANSEND_SPEC_NUM 3
#define COMM_CONTINUE_SPEC_NUM 4
#define COMM_RESET_SPEC_NUM 5
#define COMM_CLEAR_DEVICES_STATE_NUM 6
#define COMM_STATE0 0
#define COMM_STATE1 1
#define COMM_STATE2 2

class Communication
{
private:
    uint8_t *USARTPack;
    uint8_t USARTPackSize;
    uint8_t packMaxSize;
    uint8_t charDelayTime;
    uint8_t streamWaitTime;
    uint8_t transferTimeOut;
    uint8_t COMMConnectState;
    long baudRate;
    HardwareSerial *serialPort;
    bool HasRead();
    bool Valided();
    bool RepeatTimes(uint8_t symbol, uint8_t count);
    void SendSpecialData(uint8_t count);

public:
    Communication(HardwareSerial *_serialPort, long _baudRate, uint8_t _packMaxSize,
                  uint8_t _charDelayTime, uint8_t _streamWaitTime, uint8_t _transferTimeOut);
    void DoEvents();
};

#endif
