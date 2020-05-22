#include "Core.h"

CommunicationState(communicaionState, WiFiEventsISP);
SensorState(sensorState, SensorInterruptISP);

Communication *CommunicationManager;
Sensor *SpeedSensor;
LED *LEDDisplay;

void SystemInitialize()
{
    if (APP_DEBUG_MODE)
    {
        Serial.setDebugOutput(0);
        Serial.begin(APP_DEBUG_SERIAL_BADURATE);
        Serial.println("System Initializing...");
    }
    CommunicationManager = new Communication(&communicaionState, COMMUNICATION_WIFI_SSID, COMMUNICATION_WIFI_PASSWORD, COMMUNICATION_UDP_SERVER_IP,
                                             COMMUNICATION_UDP_SERVER_PORT, COMMUNICATION_UDP_LOCAL_PORT, COMMUNICATION_ENCRKEY);
    SpeedSensor = new Sensor(&sensorState);
    LEDDisplay = new LED(HARDWARE_PWRLED_PIN, HARDWARE_CONLED_PIN);
    // 启用RTOS调度器
    xTaskCreatePinnedToCore(
        SensorSamplingTask, "SensorSamplingTask", RTOS_STACK_BYTES, NULL, RTOS_HIGH_PRIORITY, NULL, 1);
    xTaskCreatePinnedToCore(
        UDPCommunicationTask, "UDPCommunicationTask", RTOS_STACK_BYTES, NULL, RTOS_MID_PRIORITY, NULL, 1);
    xTaskCreatePinnedToCore(
        LEDDisplayTask, "LEDDisplayTask", RTOS_STACK_BYTES, NULL, RTOS_LOW_PRIORITY, NULL, 0);
}

void SensorSamplingTask(void *parameter)
{
    while (true)
    {
        SpeedSensor->DoEvents();
        // 覆写队列数据
        xQueueOverwrite(sensorState.speedInfo, (void *)&(SpeedSensor->sensorBlock->pulseSpeed));
        vTaskDelay(HARDWARE_SENSOR_REFRESHTIME / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void UDPCommunicationTask(void *parameter)
{
    static int disconnectedTimeoutCounter = 0;
    while (true)
    {
        if (CommunicationManager->GetWiFiConnectionState())
        {
            disconnectedTimeoutCounter = 0;
            if (CommunicationManager->communicationStateBlock->communicationState == OFFLINE ||
                CommunicationManager->communicationStateBlock->communicationState == SENDCLIENTNAME ||
                CommunicationManager->communicationStateBlock->communicationState == SENDDATA)
            {
                int dataValue = 0;
                // 清除之前接收到的流
                CommunicationManager->ClearRecievedPacket();
                if (CommunicationManager->communicationStateBlock->communicationState == SENDDATA)
                {
                    // 从消息队列取出Xs内的脉冲数量
                    xQueueReceive(sensorState.speedInfo, &dataValue, portMAX_DELAY);
                }
                CommunicationManager->StreamBuilder(CommunicationManager->streamToSend,
                                                    CommunicationManager->communicationStateBlock->communicationState, dataValue);
                CommunicationManager->SendPacket(CommunicationManager->streamToSend);
                CommunicationManager->communicationStateBlock->communicationState = WAITFORRESP;
            }
            else if (CommunicationManager->communicationStateBlock->communicationState == WAITFORRESP)
            {
                int timeOutCounter = 0;
                while (!(CommunicationManager->PacketRecieved() &&
                         CommunicationManager->StreamContentCheck(CommunicationManager->streamRecieved, WAITFORRESP)))
                {
                    // 等待上位机响应
                    vTaskDelay(COMMUNICATION_REQUIRE_INV / portTICK_RATE_MS);
                    timeOutCounter++;
                    if (timeOutCounter >= COMMUNICATION_TIMEOUT / COMMUNICATION_REQUIRE_INV)
                    {
                        // 超时，尝试重连
                        CommunicationManager->communicationStateBlock->communicationState = OFFLINE;
                        CommunicationManager->communicationStateBlock->ServerCommunicated = false;
                        timeOutCounter = 0;
                        goto EndPoint;
                    }
                }
                // 当前处理从OFFLINE状态转移的上下文
                if (!CommunicationManager->GetServerConnectionState())
                {
                    // 准备发送设备标识符
                    CommunicationManager->communicationStateBlock->communicationState = SENDCLIENTNAME;
                    // 设置为已经连接
                    CommunicationManager->communicationStateBlock->ServerCommunicated = true;
                }
                // 处理从数据发送状态转移的上下文
                else
                {
                    // 准备发送数据
                    CommunicationManager->communicationStateBlock->communicationState = SENDDATA;
                }
            }
        }
        else
        {
            CommunicationManager->communicationStateBlock->communicationState = OFFLINE;
            disconnectedTimeoutCounter++;
            if (disconnectedTimeoutCounter >= COMMUNICATION_LOST_CONN_RESET_TIMEOUT / COMMUNICATION_REQUIRE_INV)
            {
                if (APP_DEBUG_MODE)
                {
                    Serial.println("Ready To Reboot.");
                }
                disconnectedTimeoutCounter = 0;
                CommunicationManager->ReconnectToWiFi();
            }
        }
    EndPoint:
        vTaskDelay(COMMUNICATION_REQUIRE_INV / portTICK_RATE_MS);
    }
    CommunicationManager->Dispose();
    vTaskDelete(NULL);
}

void LEDDisplayTask(void *parameter)
{
    while (true)
    {
        LEDDisplay->SetLEDPWR(HIGH);
        LEDDisplay->SetLEDConn(CommunicationManager->GetServerConnectionState());
        vTaskDelay(HARDWARE_LED_REFRESHTIME / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}
