#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Config.hpp"

volatile int sensorPulses = 0; // 传感器在采集周期中所采集的脉冲数量
volatile int sensorLastSamplingTime = -1; // 传感器上一次下降沿的时刻，初始设置为-1，防止发生除0情况
volatile int sensorLastPinLevel = HIGH; // 传感器引脚上一次读取时的电平，用于判定下降沿的发生
volatile int sensorSpeed = 0; // 传感器在采集周期中的平均速度，单位：圈/分钟
SemaphoreHandle_t sensorMutex = NULL; // 声明传感器数据资源锁，防止多线程不安全访问

// 函数：初始化传感器
// 输入：无
// 输出：无
void Sensor_Initialize()
{
	// 设置传感器引脚为上拉输入模式（传感器感应到磁场时引脚状态为低电平）
	pinMode(HDW_SENSOR_PIN, INPUT_PULLUP);

	// 创建传感器数据资源锁
	sensorMutex = xSemaphoreCreateMutex();
}

// 函数：传感器进行采集
// 输入：无
// 输出：无
void Sensor_Sampling()
{
	// 获取资源锁，防止在数据修改时被读取
	xSemaphoreTake(sensorMutex, portMAX_DELAY);

	// 当传感器引脚读取值为低且上一次引脚读取值为高（出现下降沿）时，脉冲数加一，并计算速度
	if (digitalRead(HDW_SENSOR_PIN) == LOW && sensorLastPinLevel == HIGH)
	{
		// 脉冲数加一
		sensorPulses++;

		// 计算速度：速度 = 60 * 1000ms / (当前采集系统时刻 - 上一次采集系统时刻) 圈/分钟
		sensorSpeed = (int) (60.0f * 1000.0f / (millis() - sensorLastSamplingTime));

		// 使得当前下降沿时刻赋值成为传感器上一次下降沿的时刻，准备下一次采集
		sensorLastSamplingTime = millis();
	}

	// 将当前引脚电平赋值给“传感器引脚上一次读取时的电平”，为下一次读取做准备
	sensorLastPinLevel = digitalRead(HDW_SENSOR_PIN);

	// 释放资源锁，声明数据修改已经完成，多线程读取安全
	xSemaphoreGive(sensorMutex);
}

// 函数：检查传感器速度是否为0
// 输入：距离上一次调用该函数所过去的时间周期（ms）
// 输出：无
void Sensor_CheckSpeed(int period)
{
	// 获取资源锁
	xSemaphoreTake(sensorMutex, portMAX_DELAY);

	// 若超时周期内脉冲数为0，速度为0
	if (sensorPulses == 0) sensorSpeed = 0;

	// 清空超时周期内脉冲数
	sensorPulses = 0;

	// 释放资源锁，声明数据已经更新完毕
	xSemaphoreGive(sensorMutex);
}

// 函数：获取传感器速度数据
// 输入：无
// 输出：传感器在采样周期内的平均速度（圈/分钟）
int Sensor_GetSpeed()
{
	// 声明返回值变量
	int retValue = 0;

	// 获取资源锁
	xSemaphoreTake(sensorMutex, portMAX_DELAY);

	// 将速度值赋值给返回值
	retValue = sensorSpeed;

	// 释放资源锁，声明数据已经读取完毕
	xSemaphoreGive(sensorMutex);

	// 返回返回值变量
	return retValue;
}

#endif
