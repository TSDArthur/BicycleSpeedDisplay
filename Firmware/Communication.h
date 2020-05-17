#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "Arduino.h"
#include "WiFi.h"
#include "Config.h"

enum CommunicationState
{
    OFFLINE,
    SENDCLIENTNAME,
    SENDDATA,
    WAITFORRESP
};

struct CommunicationStateBlock
{
    void (*WiFiEvents)(WiFiEvent_t event);
    volatile bool WiFiCommunicated;
    volatile bool ServerCommunicated;
    CommunicationState communicationState;
};

#define CommunicationState(structName, wifiISPName)                                   \
    void wifiISPName(WiFiEvent_t event);                                              \
    struct CommunicationStateBlock structName = {wifiISPName, false, false, OFFLINE}; \
    void wifiISPName(WiFiEvent_t event)                                               \
    {                                                                                 \
        switch (event)                                                                \
        {                                                                             \
        case SYSTEM_EVENT_STA_GOT_IP:                                                 \
            structName.WiFiCommunicated = true;                                       \
            structName.ServerCommunicated = false;                                    \
            structName.communicationState = OFFLINE;                                  \
            break;                                                                    \
        case SYSTEM_EVENT_STA_LOST_IP:                                                \
        case SYSTEM_EVENT_STA_DISCONNECTED:                                           \
            structName.WiFiCommunicated = false;                                      \
            structName.ServerCommunicated = false;                                    \
            structName.communicationState = OFFLINE;                                  \
            break;                                                                    \
        default:                                                                      \
            break;                                                                    \
        }                                                                             \
    }

class Communication
{
private:
    WiFiUDP *wifiUDP;
    const char *WiFiSSID;
    const char *WiFiPassword;
    const char *serverIPAddress;
    int serverPort;
    int localPort;
    const char *streamEncrKey;

public:
    struct CommunicationStateBlock *communicationStateBlock;
    String *streamToSend;
    String *streamRecieved;
    Communication(struct CommunicationStateBlock *_communicationStateBlock, const char *_WiFiSSID, const char *_WiFiPassword,
                  const char *_serverIPAddress, int _serverPort, int _localPort, const char *_streamEncrKey);
    bool GetWiFiConnectionState();
    bool GetServerConnectionState();
    void StreamBuilder(String *stream, CommunicationState streamType, int dataValue);
    String *StreamProcess(String *stream);
    bool StreamValid(String *stream);
    bool StreamContentCheck(String *stream, CommunicationState streamType);
    void SendPacket(String *stream);
    bool PacketRecieved();
    void ClearRecievedPacket();
    void Dispose();
};

#endif
