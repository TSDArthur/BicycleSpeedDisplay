#ifndef CONFIG_H
#define CONFIG_H

// WiFi与UDP相关设置
#define COMMUNICATION_WIFI_SSID "MSI" // 上位机WiFi名称
#define COMMUNICATION_WIFI_PASSWORD "zhangmuhua123" // 上位机WiFi连接密码
#define COMMUNICATION_UDP_SERVER_IP "192.168.137.1" // 上位机UDP服务器IP地址
#define COMMUNICATION_UDP_SERVER_PORT 3333 // 上位机UDP服务器端口
#define COMMUNICATION_UDP_LOCAL_PORT 2333 // 本地UDP监听端口

// 通讯协议设置
#define COMMUNICATION_DEVICENAME "Client0" // 该设备名称，局域网中每个设备应该有不同的两个名称
// 以下参数上下位机应设置为一致
#define COMMUNICATION_PACK_MAXSIZE 64 // 数据包最大长度
#define COMMUNICATION_ENCRYEN 0 // 0为不加密数据包，1为加密数据包
#define COMMUNICATION_ENCRKEY_SIZE 8 // 数据传输密钥长度
#define COMMUNICATION_ENCRKEY "uA%SVRmQ" // 数据传输密钥
#define COMMUNICATION_STREAMBEGIN '#' // 数据包开头帧
#define COMMUNICATION_STREAMEND '!' // 数据包结束帧
#define COMMUNICATION_STREAMDELIMITER ';' // 数据包定义帧分隔符
#define COMMUNICATION_REQUIRE_INV 200 // 通信状态机刷新时间间隔，单位：ms
#define COMMUNICATION_TIMEOUT 2000 // 上下位机无回传离线超时判据，单位：ms
#define COMMUNICATION_STREAMCSTL 4 // 数据包定义帧控制串长度
// 以下参数上下位机应设置为一致，且所有控制串长度必须与数据包定义帧控制串长度一致
#define COMMUNICATION_RESPONSESTR "succ" // 上下位机间回传状态确认串
#define COMMUNICATION_REPNAMESTR "name"  // 设备名称定义帧控制串
#define COMMUNICATION_SPDSTR "sped" // 采集每秒脉冲数量（速度）定义帧控制串
#define COMMUNICATION_REQSTR "requ" // 设备请求连接定义帧控制串

// 硬件设置
#define HARDWARE_SENSOR_PIN 32 // 设置霍尔磁传感器引脚编号
#define HARDWARE_SENSOR_TYPE 0 // 0为下降沿传感器，1为上升沿传感器
#define HARDWARE_SENSOR_TOZEROTIME 4000 // 传感器无脉冲清零时间，单位：ms
#define HARDWARE_PWRLED_PIN 35 // 设置设备电源显示LED引脚编号
#define HARDWARE_CONLED_PIN 34 // 设置设备网络连接状态LED引脚编号
#define HARDWARE_SENSOR_REFRESHTIME 100 // 设置速度更新时间，单位：ms
#define HARDWARE_LED_REFRESHTIME 500 // 设置LED显示更新时间，单位：ms

// FreeRTOS实时操作系统设置
#define RTOS_STACK_BYTES 4096 // 设置任务栈空间
#define RTOS_HIGH_PRIORITY 3 // 设置高任务优先级
#define RTOS_MID_PRIORITY 2 // 设置默认任务优先级
#define RTOS_LOW_PRIORITY 1 // 设置低任务优先级

#endif
