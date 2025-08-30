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

// ��������Ϣ
#define SSID "NET"				 // ·����SSID����
#define PASS "12345678"			 // ·��������
#define ServerIP "183.230.40.39" // ������IP��ַ
#define ServerPort 6002			 // ������IP��ַ�˿ں�
// �豸��Ϣ
#define PROID "582859"	  // ��ƷID
#define DEVID "1060927909" // �豸ID
#define AUTH_INFO "13"	  // ��Ȩ��Ϣ
// MQTT����
#define S_TOPIC_NAME "classroom_topic" // ��Ҫ���ĵ�����
#define P_TOPIC_NAME "classroomapp_topic" // ��Ҫ����������

//  �豸ʹ�����趨��
#define OLED 1 // �Ƿ�ʹ��OLED

// �Զ���ִ�н������
typedef enum
{
	MY_SUCCESSFUL = 0x01, // �ɹ�
	MY_FAIL = 0x00		  // ʧ��

} mySta; // �ɹ���־λ

typedef enum
{
	OPEN = 0x01, // ��
	CLOSE = 0x00 // �ر�

} On_or_Off_TypeDef; // �ɹ���־λ

typedef enum
{
	DERVICE_SEND = 0x00, // �豸->ƽ̨
	PLATFORM_SEND = 0x01 // ƽ̨->�豸

} Send_directino; // ���ͷ���

typedef struct
{
	U8 App;			 // ָ��ģʽ
	
	U8 Flage;			 // ģʽѡ��
	U16 people;		 //����
	U8 light;			 // ����
	U16 somg;			 // ����Ũ��

} Data_TypeDef; // ���ݲ����ṹ��

typedef struct
{

	F32 threshold_temperatuer_max; // �¶�
	F32 threshold_temperatuer_min; // �¶�

} Threshold_Value_TypeDef; // ���ݲ����ṹ��

typedef struct
{
	U8 water_state; // ˮ��
	U8 fan_state; // ����
	U8 led_state; // �Ƶ���Ŀ
} Device_Satte_Typedef; // ״̬�����ṹ��

// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data);
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state);
// ����OLED��ʾ��������
mySta Update_oled_massage(void);
// �����豸״̬
mySta Update_device_massage(void);

#endif
