#include "Communication.h"

Communication::Communication(struct CommunicationStateBlock *_communicationStateBlock, const char *_WiFiSSID, const char *_WiFiPassword,
                             const char *_serverIPAddress, int _serverPort, int _localPort, const char *_streamEncrKey)
{
    // 控制块赋值
    communicationStateBlock = _communicationStateBlock;
    // 初始化变量
    WiFiSSID = _WiFiSSID;
    WiFiPassword = _WiFiPassword;
    serverIPAddress = _serverIPAddress;
    serverPort = _serverPort;
    localPort = _localPort;
    wifiUDP = new WiFiUDP();
    streamEncrKey = _streamEncrKey;
    streamToSend = new String();
    streamRecieved = new String();
    streamToSend->clear();
    streamRecieved->clear();
    // 连接到WiFi网络
    WiFi.disconnect(true);
    WiFi.onEvent(communicationStateBlock->WiFiEvents);
    WiFi.begin(WiFiSSID, WiFiPassword);
    // 初始化UDP监听
    wifiUDP->begin(localPort);
}

bool Communication::GetWiFiConnectionState()
{
    return communicationStateBlock->WiFiCommunicated;
}

bool Communication::GetServerConnectionState()
{
    return GetWiFiConnectionState() & communicationStateBlock->ServerCommunicated;
}

void Communication::StreamBuilder(String *stream, CommunicationState streamType, int dataValue)
{
    stream->clear();
    *stream += COMMUNICATION_STREAMBEGIN;
    switch (streamType)
    {
    case OFFLINE:
        *stream += COMMUNICATION_REQSTR;
        break;
    case SENDCLIENTNAME:
        *stream += COMMUNICATION_REPNAMESTR;
        *stream += COMMUNICATION_DEVICENAME;
        break;
    case SENDDATA:
        *stream += COMMUNICATION_SPDSTR;
        *stream += dataValue;
        break;
    default:
        break;
    }
    *stream += COMMUNICATION_STREAMEND;
}

String *Communication::StreamProcess(String *stream)
{
    if (COMMUNICATION_ENCRYEN)
    {
        int streamLength = stream->length();
        for (int i = 0; i < streamLength; i++)
        {
            stream->setCharAt(i, (char)(stream->charAt(i) ^ streamEncrKey[i % COMMUNICATION_ENCRKEY_SIZE]));
        }
    }
    return stream;
}

bool Communication::StreamValid(String *stream)
{
    bool retValue = false;
    if (stream->charAt(0) == COMMUNICATION_STREAMBEGIN && stream->charAt(stream->length() - 1) == COMMUNICATION_STREAMEND)
    {
        retValue = true;
    }
    return retValue;
}

bool Communication::StreamContentCheck(String *stream, CommunicationState streamType)
{
    bool retValue = false;
    switch (streamType)
    {
    case WAITFORRESP:
        if (stream->substring(1, stream->length() - 2) == COMMUNICATION_RESPONSESTR)
        {
            retValue = true;
        }
        break;
    }
    return retValue;
}

void Communication::SendPacket(String *stream)
{
    if (GetWiFiConnectionState())
    {
        wifiUDP->beginPacket(serverIPAddress, serverPort);
        stream = StreamProcess(stream);
        wifiUDP->print(*stream);
        wifiUDP->endPacket();
    }
}

bool Communication::PacketRecieved()
{
    bool retValue = false;
    int packetSize = wifiUDP->parsePacket();
    if (packetSize && packetSize < COMMUNICATION_PACK_MAXSIZE)
    {
        char packetBuffer[COMMUNICATION_PACK_MAXSIZE];
        ClearRecievedPacket();
        memset(packetBuffer, '\0', sizeof(char) * COMMUNICATION_PACK_MAXSIZE);
        wifiUDP->read(packetBuffer, packetSize);
        *streamRecieved = packetBuffer;
        streamRecieved = StreamProcess(streamRecieved);
        retValue = StreamValid(streamRecieved);
        if (!retValue)
        {
            ClearRecievedPacket();
        }
    }
    else if (streamRecieved->length() > COMMUNICATION_STREAMCSTL + 2)
    {
        retValue = true;
    }
    return retValue;
}

void Communication::ClearRecievedPacket()
{
    streamRecieved->clear();
}

void Communication::Dispose()
{
    delete streamToSend;
    delete streamRecieved;
}
