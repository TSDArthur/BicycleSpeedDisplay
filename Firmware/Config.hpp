#ifndef CONFIG_HPP
#define CONFIG_HPP

// 连接设置
#define COMM_SERIAL_BAUDRATE 115200L
#define COMM_WIFI_SSID "MSI" // 服务器WiFi名称
#define COMM_WIFI_PASSWORD "zhangmuhua123" // 服务器WiFi密码
#define COMM_SERVER_IP "192.168.137.1" // 服务器IP地址
#define COMM_SERVER_PORT 3333 // 服务器端口
#define COMM_DEVICE_NAME "Client0" // 设备名称
#define COMM_DEVICE_VERSION "Rev 1.0" // 设备版本
#define COMM_BUFFER_MAXSIZE 512 // Json接收缓冲区大小
#define COMM_TIMEOUT 4000 // 超时自动重连时间
#define COMM_UPDATE_TIME 500 // 连接状态刷新时间内
#define COMM_DISCONNECTED 0 // 连接状态：未连接
#define COMM_WAITFORRESP 1 // 连接状态：等待回复
#define COMM_SENDSENSORDATA 2 // 连接状态：发送传感器数据
#define COMM_WIFI_TIMEOUT 10 // WiFi连接等待超时时间，T = Timeout * 1000 ms

// 通讯协议设置
#define PROTO_ROLE "role" // Json中协议发送方角色字段名称
#define PROTO_CMD "cmd" // Json中协议指令字段名称
#define PROTO_DATA_SIZE "data_size" // Json中数据大小字段名称
#define PROTO_DATA "data" // Json中数据字段名称
#define PROTO_ROLE_SERVER 0 // Json中协议发送方服务器角色
#define PROTO_ROLE_CLIENT 1 // Json中协议发送方客户端角色
#define PROTO_CMD_RESP_OK 0 // Json中协议指令：返回成功信息
#define PROTO_CMD_RESP_ERROR 1 // Json中协议指令：返回失败信息
#define PROTO_CMD_CONNECT_REQ 2 // Json中协议指令：客户端向服务器请求连接
#define PROTO_CMD_SENSOR_DATA_SEND 3 // Json中协议指令：客户端向服务端发送传感器数据

// 硬件设置
#define HDW_SENSOR_PIN 32 // 传感器引脚编号
#define HDW_SENSOR_STOP_TIMEOUT 2000 // 在该周期内传感器脉冲数为0，可判定速度为0
#define HDW_SENSOR_RESOLUTION_PERIOD 50 // 传感器脉冲响应周期（ms）

// FreeRTOS设置
#define RTOS_STACK_BYTES 4096 // FreeRTOS任务栈空间
#define RTOS_HIGH_PRIORITY 3 // 高任务优先级
#define RTOS_MID_PRIORITY 2 // 默认任务优先级
#define RTOS_LOW_PRIORITY 1 // 低任务优先级

#endif
