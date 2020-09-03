#include "Arduino.h"
#include "Config.hpp"
#include "Websocket.hpp"
#include "Json.hpp"
#include "Sensor.hpp"

// 函数：Arduino 初始化函数
// 输入：无
// 输出：无
void setup()
{
	// 初始化串口
	Serial.begin(COMM_SERIAL_BAUDRATE);

	// 初始化传感器
	Sensor_Initialize();

	// 初始化Websocket客户端
	Websocket_Initialize();

	// 定义RTOS通讯采集任务
	xTaskCreatePinnedToCore(
		CommunicationTask, "CommunicationTask", RTOS_STACK_BYTES, NULL, RTOS_HIGH_PRIORITY, NULL, 0);

	// 定义RTOS传感器采集状态
	xTaskCreatePinnedToCore(
		SensorSamplingTask, "SensorSamplingTask", RTOS_STACK_BYTES, NULL, RTOS_MID_PRIORITY, NULL, 1);
}

// 函数：Arduino 无限循环函数
// 输入：无
// 输出：无
void loop()
{
	// 刷新Websocket状态
	Websocket_Handler();
}

// 函数：连接管理任务
// 输入：无
// 输出：无
void CommunicationTask(void *parameter)
{
	// 定义连接超时计数器变量
	int connectionTimeoutCounter = 0;

	// 任务无限循环
	while (true)
	{
		// 当网络连接建立时，执行操作
		if (networkCommunicated)
		{
			// 连接有效，清零连接超时计数器
			connectionTimeoutCounter = 0;
			// 若未建立通讯通道，发送连接请求，转移至等待回复状态
			if (communicationState == COMM_DISCONNECTED)
			{
				// 构建连接请求Json字符串
				Json_StreamBuilder(jsonStrBuffer, PROTO_CMD_CONNECT_REQ);
				// 发送连接请求Json字符串
				Websocket_SendStream(jsonStrBuffer);
				// 清空Json缓冲字符串
				jsonStrBuffer = "";
				// 将当前状态赋值至上一次状态，准备进行状态转移
				lastCommunicationState = communicationState;
				// 连接状态转移至等待服务器回复
				communicationState = COMM_WAITFORRESP;
			}
			// 发送传感器数据，转移至等待回复状态
			else if (communicationState == COMM_SENDSENSORDATA)
			{
				// 构建传感器数据Json字符串
				Json_StreamBuilder(jsonStrBuffer, PROTO_CMD_SENSOR_DATA_SEND);
				// 发送传感器数据Json字符串
				Websocket_SendStream(jsonStrBuffer);
				// 清空Json缓冲字符串
				jsonStrBuffer = "";
				// 将当前状态赋值至上一次状态，准备进行状态转移
				lastCommunicationState = communicationState;
				// 连接状态转移至等待服务器回复
				communicationState = COMM_WAITFORRESP;
			}
			// 处理等待回复状态
			else if (communicationState == COMM_WAITFORRESP)
			{
				// 定义回复等待计数器变量
				int timeoutCounter = 0;
				// 循环等待服务器发送来的数据
				while (Json_TryParse(jsonStrBuffer) == ERROR)
				{
					// 非阻塞式等待
					vTaskDelay(COMM_UPDATE_TIME / portTICK_RATE_MS);
					// 回复等待计数器自增
					timeoutCounter++;
					// 若等待超时，判定为连接丢失，直接跳转运行至EndPoint，准备尝试重连
					if (timeoutCounter >= COMM_TIMEOUT / COMM_UPDATE_TIME)
					{
						// 重置连接状态变量
						lastCommunicationState = COMM_DISCONNECTED;
						communicationState = COMM_DISCONNECTED;
						serverCommunicated = false;
						// 重置超时计数器
						timeoutCounter = 0;
						// 跳转运行至EndPoint
						goto EndPoint;
					}
				}

				// 若当前状态由“未连接”状态转移而来，检查服务器是否同意连接
				if (lastCommunicationState == COMM_DISCONNECTED)
				{
					// 若服务器返回OK，标记为已经连接，转移至发送传感器数据状态
					if (Json_ProtocolProcess(jsonStrBuffer) == OK)
					{
						// 标记为已连接至Websocket服务器
						serverCommunicated = true;
						// 将当前状态赋值至上一次状态，准备进行状态转移
						lastCommunicationState = communicationState;
						// 状态转移至“发送传感器数据”
						communicationState = COMM_SENDSENSORDATA;
					}
					// 若服务器拒绝连接，重新尝试发送连接请求
					else
					{
						// 重置连接状态变量
						lastCommunicationState = COMM_DISCONNECTED;
						communicationState = COMM_DISCONNECTED;
						serverCommunicated = false;
					}
				}

				// 若当前状态由“发送传感器状态”转移而来，检查服务器是否已经收到
				else if (lastCommunicationState == COMM_SENDSENSORDATA)
				{
					// 若服务器返回OK，说明上一次数据已收到，重新转移至发送传感器数据状态
					if (Json_ProtocolProcess(jsonStrBuffer) == OK)
					{
						// 将当前状态赋值至上一次状态，准备进行状态转移
						lastCommunicationState = communicationState;
						// 状态转移至“发送传感器数据”
						communicationState = COMM_SENDSENSORDATA;
					}
				}
			}
		}
		// 若WiFi网络连接一直失效，则超时后自动重启设备
		else
		{
			// 重置连接状态
			lastCommunicationState = COMM_DISCONNECTED;
			communicationState = COMM_DISCONNECTED;
			serverCommunicated = false;
			// 定义连接超时计数器自增
			connectionTimeoutCounter++;
			// 若连接超时计数器超时，则说明连接丢失，设备重新启动
			if (connectionTimeoutCounter >= COMM_TIMEOUT / COMM_UPDATE_TIME)
			{
				Serial.println("Rebooting...");
				ESP.restart();
			}
		}
		
		EndPoint:
		vTaskDelay(COMM_UPDATE_TIME / portTICK_RATE_MS);
	}
}

// 函数：传感器采集任务
// 输入：无
// 输出：无
void SensorSamplingTask(void *parameter)
{
	// 定义计数器变量
	int sensorPeriodCounter = 0;

	// 任务无限循环
	while (true)
	{
		// 传感器脉冲响应周期内，采集脉冲
		Sensor_Sampling();

		// 周期计数器自增
		sensorPeriodCounter++;

		// 当周期计数器对应时间达到速度为0超时周期时，检查速度是否为0
		if (sensorPeriodCounter * HDW_SENSOR_RESOLUTION_PERIOD >= HDW_SENSOR_STOP_TIMEOUT)
		{
			// 检查速度是否为0
			Sensor_CheckSpeed(sensorPeriodCounter * HDW_SENSOR_RESOLUTION_PERIOD);

			// 归零周期计数器
			sensorPeriodCounter = 0;
		}

		// 任务延时脉冲响应周期
		vTaskDelay(HDW_SENSOR_RESOLUTION_PERIOD / portTICK_RATE_MS);
	}
}
