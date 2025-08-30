#ifndef __GIT_H
#define __GIT_H
#include "stdio.h"

typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned int U32;
typedef signed int S32;
typedef float F32;

// 服务器信息
#define SSID "NET"				 // 路由器SSID名称
#define PASS "12345678"			 // 路由器密码
#define ServerIP "183.230.40.39" // 服务器IP地址
#define ServerPort 6002			 // 服务器IP地址端口号
// 设备信息
#define PROID "582859"	  // 产品ID
#define DEVID "1060927909" // 设备ID
#define AUTH_INFO "13"	  // 鉴权信息
// MQTT主题
#define S_TOPIC_NAME "classroom_topic" // 需要订阅的主题
#define P_TOPIC_NAME "classroomapp_topic" // 需要发布的主题

//  设备使用外设定义
#define OLED 1 // 是否使用OLED

// 自定义执行结果类型
typedef enum
{
	MY_SUCCESSFUL = 0x01, // 成功
	MY_FAIL = 0x00		  // 失败

} mySta; // 成功标志位

typedef enum
{
	OPEN = 0x01, // 打开
	CLOSE = 0x00 // 关闭

} On_or_Off_TypeDef; // 成功标志位

typedef enum
{
	DERVICE_SEND = 0x00, // 设备->平台
	PLATFORM_SEND = 0x01 // 平台->设备

} Send_directino; // 发送方向

typedef struct
{
	U8 App;			 // 指令模式
	
	U8 Flage;			 // 模式选择
	U16 people;		 //人数
	U8 light;			 // 光照
	U16 somg;			 // 烟雾浓度

} Data_TypeDef; // 数据参数结构体

typedef struct
{

	F32 threshold_temperatuer_max; // 温度
	F32 threshold_temperatuer_min; // 温度

} Threshold_Value_TypeDef; // 数据参数结构体

typedef struct
{
	U8 water_state; // 水泵
	U8 fan_state; // 风扇
	U8 led_state; // 灯的数目
} Device_Satte_Typedef; // 状态参数结构体

// 获取数据参数
mySta Read_Data(Data_TypeDef *Device_Data);
// 初始化
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state);
// 更新OLED显示屏中内容
mySta Update_oled_massage(void);
// 更新设备状态
mySta Update_device_massage(void);

#endif
