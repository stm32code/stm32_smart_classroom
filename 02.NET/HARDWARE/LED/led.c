#include "delay.h"
#include "led.h"
// Э���ļ�
#include "onenet.h"
// �����豸����
#include "esp8266.h"
extern Device_Satte_Typedef device_state_init;		  // �豸״̬
// ��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��
// LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(Relay_GPIO_CLK|LED1_GPIO_CLK, ENABLE); // ʹ�ܶ˿�ʱ��C

	GPIO_InitStructure.GPIO_Pin = Relay_GPIO_PIN;	  // �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	GPIO_Init(Relay_GPIO_PORT, &GPIO_InitStructure);  // ������� ��IO���ٶ�Ϊ50MHz
	
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	  // �˿�����, �������
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);  // ������� ��IO���ٶ�Ϊ50MHz
	
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;	  // �˿�����, �������
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);  // ������� ��IO���ٶ�Ϊ50MHz
	
	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;	  // �˿�����, �������
	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);  // ������� ��IO���ٶ�Ϊ50MHz
	
	GPIO_InitStructure.GPIO_Pin = LED4_GPIO_PIN;	  // �˿�����, �������
	GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);  // ������� ��IO���ٶ�Ϊ50MHz

	LED = OFF;
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}
/**
 * @brief  MQTT����LEDģ��
 * @param  ��
 * @retval ��
 */
void Mqtt_LED(U8 Cmd)
{

	switch (Cmd)
	{
		case 1:
			LED1_ON;
			LED2_OFF;
			LED3_OFF;
			LED4_OFF;
			device_state_init.led_state=1;
			break;
		case 2:
			LED1_ON;
			LED2_ON;;
			LED3_OFF;
			LED4_OFF;
			device_state_init.led_state=2;
			break;
		case 3:
			LED1_ON;
			LED2_ON;
			LED3_ON;
			LED4_OFF;
			device_state_init.led_state=3;
			break;
		case 4:
			LED1_ON;
			LED2_ON;
			LED3_ON;
			LED4_ON;
			device_state_init.led_state=4;
			break;
		case 0:
			LED1_OFF;
			LED2_OFF;
			LED3_OFF;
			LED4_OFF;
			device_state_init.led_state=0;
			break;

	}
	OneNet_SendMqtt(2); // �ϴ����ĵ�����
}
/*********************************************************************************
* @Function	:	ϵͳ����PB3��PB4Ϊ��ͨIO��
* @Input		:	deviceSta,�豸״̬
* @Output		: 	None
* @Return		: 	None
* @Others		: 	JTAG���Է�ʽ����Ӱ��
* @Date			:  2020-04-22
**********************************************************************************/
void System_PB34_setIO( void )
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
}
