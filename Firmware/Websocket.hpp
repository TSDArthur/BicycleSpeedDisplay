#ifndef WEBSOCKET_HPP
#define WEBSOCKET_HPP

#include "Arduino.h"
#include "WiFiClientSecure.h"
#include "WebSocketsClient.h"
#include "Config.hpp"
#include "Json.hpp"
#include "WiFi.h"
#include "WiFiMulti.h"

// 定义WiFi实例
WiFiMulti WiFiMulti;

// 定义Websocket客户端实例
WebSocketsClient client;

int communicationState = COMM_DISCONNECTED; // 定义通讯状态变量，初始状态：未与服务器构建通讯通道
int lastCommunicationState = COMM_DISCONNECTED; // 定义状态转移前上一次的通讯状态变量，初始状态：未与服务器构建通讯通道
bool networkCommunicated = false; // 定义网络状态变量，初始状态：未连接至服务器网络
bool serverCommunicated = false; // 定义Websocket服务器连接状态，初始状态：未连接至Websocket服务器

// 函数：Websocket事件回调
// 由Websocket中断调用
void Websocket_webSocketEvent(WStype_t type, uint8_t * payload, size_t length) 
{
	if (type == WStype_CONNECTED) 
	{
		// 若Websocket连接建立，设置网络状态为已连接
		networkCommunicated = true;
		// 串口输出调试信息
		Serial.println("Connnection Opened.");
	} 
	else if (type == WStype_DISCONNECTED) 
	{
		// 若Websocket连接关闭，设置网络状态为未连接，设置通讯状态为未连接
		networkCommunicated = false;
		serverCommunicated = false;
		communicationState = COMM_DISCONNECTED;
		// 串口输出调试信息
		Serial.println("Connnection Closed.");
	}
	else if (type == WStype_TEXT)
	{
		jsonStrBuffer += (const char*) payload;
	}
}

// 函数：初始化WiFi与Websocket客户端
// 输入：无
// 输出：无
void Websocket_Initialize()
{
	// 初始化WiFi
	WiFiMulti.addAP(COMM_WIFI_SSID, COMM_WIFI_PASSWORD);

	// 等待WiFi连接，串口输出调试信息
	Serial.println("Waiting for WiFi Ready...");
	for (int i = 0; i < COMM_WIFI_TIMEOUT && WiFiMulti.run() != WL_CONNECTED; i++) delay(1000);

	// 若超时后仍然未连接WiFi，重启设备
	if (WiFiMulti.run() != WL_CONNECTED) 
	{
		Serial.println("Rebooting...");
		ESP.restart();
	}

	// WiFi连接成功，串口输出调试信息
	Serial.println("Connected to WiFi.");

	// 绑定Websocket回调函数
	client.onEvent(Websocket_webSocketEvent);

	// 尝试连接Websocket
	Serial.println("Waiting for Websocket Ready...");
	client.begin(COMM_SERVER_IP, COMM_SERVER_PORT, "/");

	// Websocket连接成功，串口输出调试信息
	Serial.println("Connected to Websocket.");
}

// 函数：使用Websocket发送数据流
// 输入：字符串实例
// 输出：无
void Websocket_SendStream(String& str)
{
	// Websoccket发送数据流
	client.sendTXT(str);
	
	// 打印串口调试信息
	Serial.println("Send: " + str);
}

// 函数：刷新Websocket
// 输入：无
// 输出：无
void Websocket_Handler()
{
	client.loop();
}

#endif
