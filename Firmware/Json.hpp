#ifndef JSON_HPP
#define JSON_HPP

#include "Arduino.h"
#include "Config.hpp"
#include "Sensor.hpp"
#include "ArduinoJson.h"

// Json结构
// 字段：role，cmd，data_size，data
// role: 定义该Json数据包是来自服务器还是客户端
// cmd：定义该Json数据包包含的控制指令，如请求连接，请求传感器数据等
// data_size：数据数组的大小
// data：数据数组，根据cmd字段对数据按照一定协议定义进行读取或填充

// 考虑到Websocket单次中断接收的数据可能不完整，我们每次将中断接收的数据附加到缓冲字符串的末尾，并尝试解析，若成功则说明单次数据接收已经完成
#define ERROR 0 // 代表执行失败的返回值
#define OK 1 // 代表执行成功的返回值

String jsonStrBuffer = ""; // 定义Json缓冲字符串

// 函数：尝试将Json缓冲字符串解析为Json实例
// 输入：Json缓冲字符串实例
// 输出：解析结果（成功/失败）
int Json_TryParse(const String& input)
{
	// 定义Json实例
	DynamicJsonDocument doc(COMM_BUFFER_MAXSIZE);

	// 清空Json实例
	doc.clear();

	// 定义返回值，默认值为解析不成功
	int retValue = ERROR;

	// 尝试将缓冲字符串解析为Json实例
	if (deserializeJson(doc, input) == DeserializationError::Ok)
	{
		// 若解析成功且Json中每个字段均有值，将Json缓冲字符串转换为Json实例，返回值设置为解析成功
		// 注意，为了简化判断，在服务器构造Json时，data数组第一位默认为设备名称（服务器：Server，客户端：Config.hpp中设定的名称）
		if (!doc.isNull() && !doc[PROTO_ROLE].isNull() && !doc[PROTO_CMD].isNull()
			&& !doc[PROTO_DATA_SIZE].isNull() && !doc[PROTO_DATA].isNull())
			{
				// 返回值置为成功
				retValue = OK;

				// 打印串口调试信息
				Serial.println("Recieve: " + input);
			}
	}

	// 返回返回值
	return retValue;
}

// 函数：按照服务器与设备的通讯协议，完成接收到的Json字符串的处理，返回是否成功
// 输入：Json缓冲字符串实例
// 输出：执行结果（成功/失败）
int Json_ProtocolProcess(const String& input)
{
	// 定义返回值，默认值为处理不成功
	int retValue = ERROR;

	// 定义Json实例
	DynamicJsonDocument doc(COMM_BUFFER_MAXSIZE);

	// 清空Json实例
	doc.clear();

	// 转换为Json
	deserializeJson(doc, input);

	// 首先判断数据是否来自服务器，对设备端而言，只处理来自服务器的Json数据
	if (doc[PROTO_ROLE] == PROTO_ROLE_SERVER)
	{
		// 若Json数据指令为RESP_OK，返回成功
		if (doc[PROTO_CMD] == PROTO_CMD_RESP_OK)
			retValue = OK;
	}

	// 返回返回值
	return retValue;
}

// 函数：按照服务器与设备的通讯协议，构造用于发送的Json字符串
// 输入：Json缓冲字符串实例，构造类型
// 输出：无
void Json_StreamBuilder(String& output, int builderType)
{
	// 定义Json实例
	DynamicJsonDocument doc(COMM_BUFFER_MAXSIZE);

	// 清空字符串与Json实例
	output = "";
	doc.clear();
	
	// 根据构造器类型进行Json字符串构造
	// 构造“连接请求”Json字符串
	if (builderType == PROTO_CMD_CONNECT_REQ)
	{
		// 设置发送角色为客户端
		doc[PROTO_ROLE] = PROTO_ROLE_CLIENT;

		// 设置协议指令为请求连接
		doc[PROTO_CMD] = PROTO_CMD_CONNECT_REQ;

		// 设置数据大小为2，以随连接请求发送设备名称与设备版本
		doc[PROTO_DATA_SIZE] = 2;

		// 将设备名称与设备版本加入数据数组
		JsonArray data = doc.createNestedArray(PROTO_DATA);
		data.add(COMM_DEVICE_NAME);
		data.add(COMM_DEVICE_VERSION);
	}
	// 构造“发送传感器数据”Json字符串
	else if (builderType == PROTO_CMD_SENSOR_DATA_SEND)
	{
		// 设置发送角色为客户端
		doc[PROTO_ROLE] = PROTO_ROLE_CLIENT;

		// 设置协议指令为发送传感器数据
		doc[PROTO_CMD] = PROTO_CMD_SENSOR_DATA_SEND;

		// 设置数据大小为2
		doc[PROTO_DATA_SIZE] = 2;

		// 将设备名称与霍尔传感器速度信息加入数据数组
		JsonArray data = doc.createNestedArray(PROTO_DATA);
		data.add(COMM_DEVICE_NAME);
		data.add(Sensor_GetSpeed());
	}

	// 序列化Json，并赋值给Json缓冲字符串
	serializeJson(doc, output);
}

#endif
