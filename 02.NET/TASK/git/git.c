#include "git.h"
#include "onenet.h"
#if OLED // OLED�ļ�����
#include "oled.h"
#endif
#include "dht11.h"
#include "bh1750.h"
#include "adc.h"
#include "led.h"
#include "relay.h"  
// ����豸�����ļ�

Data_TypeDef Data_init;						  // �豸���ݽṹ��
Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
Device_Satte_Typedef device_state_init;		  // �豸״̬
DHT11_Data_TypeDef DHT11_Data;
// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data)
{
	Read_DHT11(&DHT11_Data); //��ȡ��ʪ������
	Device_Data->light = getValue();								  // ��ȡ���ǿ��
	Device_Data->somg = Smog_Trans_Concentration();								  // ��ȡ
	return MY_SUCCESSFUL;
}
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state)
{
	// ��ֵ����
	threshold_value->threshold_temperatuer_max = 25;
	threshold_value->threshold_temperatuer_min = 23;
	// ״̬����
	device_state->water_state = 0;
	return MY_SUCCESSFUL;
}
// ����OLED��ʾ��������
mySta Update_oled_massage()
{
#if OLED // �Ƿ��
	char str[50];
	u8 i;
	// ����OELD��ʾ������
	for(i=0;i<4;i++){
		OLED_ShowChinese(18*i, 0, i, 16);
	}
	sprintf(str, ": %02d", Data_init.people);
	OLED_ShowString(74, 0, (unsigned char *)str, 16);
	sprintf(str, "Ligth: %03d", Data_init.light);
	OLED_ShowString(0, 16, (unsigned char *)str, 16);
	sprintf(str, "Temp: %02d.%02d", DHT11_Data.temp_int,DHT11_Data.temp_deci);
	OLED_ShowString(0, 32, (unsigned char *)str, 16);
	sprintf(str, "Somg: %04d", Data_init.somg);
	OLED_ShowString(0, 48, (unsigned char *)str, 16);
	
	if(Data_init.Flage){
		for(i=6;i<8;i++){
		OLED_ShowChinese(90+(i-6)*16, 48, i, 16);
		}
	
	}else{
		for(i=4;i<6;i++){
		OLED_ShowChinese(90+(i-4)*16, 48, i, 16);
		}
	}

	OLED_Refresh(); // ������ʾ
#endif

	return MY_SUCCESSFUL;
}

// �����豸״̬
mySta Update_device_massage()
{
		if (Data_init.App==3)
		{
			OneNet_SendMqtt(4); // �ϴ����ĵ�����
			Data_init.App=0;
		}
		// ���в���ִֻ��һ�Σ���Լϵͳ��Դ,
		if(!Data_init.Flage){
		if (Data_init.people>30&& device_state_init.led_state!=4)
		{
			Mqtt_LED(4);
		}
		else if (Data_init.people>20&&Data_init.people<=30&& device_state_init.led_state!=3)
		{
			Mqtt_LED(3);
		}
		else if (Data_init.people>10&&Data_init.people<=20&& device_state_init.led_state!=2)
		{
			Mqtt_LED(2);
		}	
		else if (Data_init.people>1&&Data_init.people<=10&& device_state_init.led_state!=1)
		{
			Mqtt_LED(1);
		}
		else if (Data_init.people==0&& device_state_init.led_state!=0)
		{
			Mqtt_LED(0);
		}
		if (device_state_init.fan_state==0&&Data_init.somg>2000)
		{
			Mqtt_Relay(1);
		}
		else if ( device_state_init.fan_state==1&&Data_init.somg<1800)
		{
			Mqtt_Relay(0);
		}
	}else{
		switch(Data_init.App){
			case 1:
				if (device_state_init.led_state==4)
				{
					Mqtt_LED(4);
				}
				else if ( device_state_init.led_state==3)
				{
					Mqtt_LED(3);
				}
				else if (device_state_init.led_state==2)
				{
					Mqtt_LED(2);
				}	
				else if (device_state_init.led_state==1)
				{
					Mqtt_LED(1);
				}
				else if (device_state_init.led_state==0)
				{
					Mqtt_LED(0);
				}
				break;
			case 2:
				if (device_state_init.fan_state==0)
				{
					Mqtt_Relay(0);
				}
				else if ( device_state_init.fan_state==1)
				{
					Mqtt_Relay(1);
				}
				break;
		}
		Data_init.App=0;
	}

	return MY_SUCCESSFUL;
}
