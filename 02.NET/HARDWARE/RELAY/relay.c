#include "relay.h"  
// Э���ļ�
#include "onenet.h"
// �����豸����
#include "esp8266.h"
extern Device_Satte_Typedef device_state_init;		  // �豸״̬
void RELAY_GPIO_Config(void)
{		

		GPIO_InitTypeDef GPIO_InitStructure;

	
		RCC_APB2PeriphClockCmd( BEEP_GPIO_CLK|RELAY4_GPIO_CLK, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = RELAY4_GPIO_PIN	;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(RELAY4_GPIO_PORT, &GPIO_InitStructure);
		
//	
//		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN	;		
//		GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
//	
		GPIO_ResetBits(RELAY4_GPIO_PORT, RELAY4_GPIO_PIN);
//		GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
}
/**
 * @brief  MQTT�����̵���ģ��
 * @param  ��
 * @retval ��
 */
void Mqtt_Relay(U8 Cmd)
{

	ESP8266_Clear(); // ��ջ���
	switch (Cmd)
	{
		case 0:
			RELAY4_OFF;
			device_state_init.fan_state=0;
			break;
		case 1:
			RELAY4_ON;
			device_state_init.fan_state=1;
			break;

		default:

			break;
	}
	OneNet_SendMqtt(3); // �ϴ����ĵ�����
}

