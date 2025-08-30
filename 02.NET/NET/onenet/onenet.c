// �����豸
#include "esp8266.h"

// Э���ļ�
#include "onenet.h"
#include "mqttkit.h"
#include "delay.h"

// Ӳ������
#include "usart.h"
#include "led.h"
#include "dht11.h"
#include "relay.h"  

//  C��
#include <string.h>
#include <stdio.h>

extern Data_TypeDef Data_init;
extern Device_Satte_Typedef device_state_init;		  // �豸״̬

extern unsigned char esp8266_buf[256];
extern DHT11_Data_TypeDef DHT11_Data;

U8 Connect_Net = 0;
//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-�ɹ�	0-ʧ��
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_DevLink(void)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���

	unsigned char *dataPtr;

	_Bool status = 1;

	printf("PROID: %s,	AUIF: %s,	DEVID:%s\r\n", PROID, AUTH_INFO, DEVID);

	if (MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len); // �ϴ�ƽ̨

		dataPtr = ESP8266_GetIPD(250); // �ȴ�ƽ̨��Ӧ
		if (dataPtr != NULL)
		{
			if (MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch (MQTT_UnPacketConnectAck(dataPtr))
				{
				case 0:
					printf("Tips:	���ӳɹ�\r\n");
					status = 0;
					break;

				case 1:
					printf("WARN:	����ʧ�ܣ�Э�����\r\n");
					break;
				case 2:
					printf("WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");
					break;
				case 3:
					printf("WARN:	����ʧ�ܣ�������ʧ��\r\n");
					break;
				case 4:
					printf("WARN:	����ʧ�ܣ��û������������\r\n");
					break;
				case 5:
					printf("WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");
					break;

				default:
					printf("ERR:	����ʧ�ܣ�δ֪����\r\n");
					break;
				}
			}
		}

		MQTT_DeleteBuffer(&mqttPacket); // ɾ��
	}
	else
		printf("WARN:	MQTT_PacketConnect Failed\r\n");

	return status;
}
unsigned char OneNet_FillBuf(char *buf)
{
	char text[256];

	memset(text, 0, sizeof(text));

	strcpy(buf, ",;");


	memset(text, 0, sizeof(text));
	sprintf(text, "Temp,%02d.%02d;", DHT11_Data.temp_int,DHT11_Data.temp_deci); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "Humi,%02d.%02d;", DHT11_Data.humi_int,DHT11_Data.humi_deci); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "Light,%03d;", Data_init.light); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "Peopel,%02d;", Data_init.people); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "Somg,%04d;", Data_init.somg); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "Fan,%d;", device_state_init.fan_state); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "Led,%d;", device_state_init.led_state); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	return strlen(buf);
}
unsigned char MQTT_FillBuf(char *buf)
{
	char text[256];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"Temp\":\"%02d.%02d\",", DHT11_Data.temp_int,DHT11_Data.temp_deci); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"Humi\":\"%02d.%02d\",", DHT11_Data.humi_int,DHT11_Data.humi_deci); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"Light\":\"%03d\",",  Data_init.light); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"Peopel\":\"%02d\",",  Data_init.people); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"Somg\":\"%04d\",",  Data_init.somg); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"Fan\":\"%d\",",device_state_init.fan_state); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"Led\":\"%d\",", device_state_init.led_state); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"Flage\":\"%d\"", Data_init.Flage); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
unsigned char Mqtt_Led_Info(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"Led\":\"%d\"", device_state_init.led_state); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
unsigned char Mqtt_Flage_Info(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"Flage\":\"%d\"", Data_init.Flage); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
unsigned char Mqtt_Fan_Info(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"Fan\":\"%d\"", device_state_init.fan_state); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	��
//
//	˵����
//==========================================================
void OneNet_SendData(void)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���

	char buf[128];

	short body_len = 0, i = 0;
	memset(buf, 0, sizeof(buf));
	// ���
	body_len = OneNet_FillBuf(buf); // ��ȡ��ǰ��Ҫ���͵����������ܳ���

	if (body_len)
	{
		// ���
		if (MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0) // ���
		{
			for (; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];

			ESP8266_SendData(mqttPacket._data, mqttPacket._len); // �ϴ����ݵ�ƽ̨

			MQTT_DeleteBuffer(&mqttPacket); // ɾ��
		}
		else
			printf("WARN:	EDP_NewBuffer Failed\r\n");
	}
}
//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	��
//
//	˵����
//==========================================================
 void OneNet_SendMqtt(U8 Cmd)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���
	char buf[254];
	short body_len = 0;
	memset(buf, 0, sizeof(buf));
	switch (Cmd)
	{
	case 1:
		body_len = MQTT_FillBuf(buf); // ������
		break;
	case 2:
		body_len = Mqtt_Led_Info(buf); // ������
		break;
	case 3:
		body_len = Mqtt_Fan_Info(buf); // ������
		break;	
	case 4:
		body_len = Mqtt_Flage_Info(buf); // ������
		break;	
	default:
		break;
	}

	if (body_len)
	{
		// ���
		if (MQTT_PacketPublish(MQTT_PUBLISH_ID, P_TOPIC_NAME, buf, body_len, MQTT_QOS_LEVEL1, 0, 1, &mqttPacket) == 0)
		{
			ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
			MQTT_DeleteBuffer(&mqttPacket);						 // ɾ��
		}
		else
			printf("WARN\r\n");
	}
}
//==========================================================
//	�������ƣ�	OneNet_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���

	char *req_payload = NULL;
	char *cmdid_topic = NULL;

	unsigned short topic_len = 0;
	unsigned short req_len = 0;

	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;

	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
		//---------------------------------------------����һ����ȡ������������---------------------------------------------
	type = MQTT_UnPacketRecv(cmd);
	switch (type)
	{
		//---------------------------------------------����������ú�������-------------------------------------------------
	case MQTT_PKT_CMD: // �����·�

		result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len); // ���topic����Ϣ��
		if (result == 0)
		{
			printf("cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);

			if (MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0) // ����ظ����
			{
				printf("Tips:	Send CmdResp\r\n");
				ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
				MQTT_DeleteBuffer(&mqttPacket);					 // ɾ��
			}
		}

		break;

	case MQTT_PKT_PUBLISH: // ���յ�Publish��Ϣ

		result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
		if (result == 0)
		{
			printf("topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
				   cmdid_topic, topic_len, req_payload, req_len);

			switch (qos)
			{
			case 1: // �յ�publish��qosΪ1���豸��Ҫ�ظ�Ack

				if (MQTT_PacketPublishAck(pkt_id, &mqttPacket) == 0)
				{
					printf("Tips:	Send PublishAck\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
					MQTT_DeleteBuffer(&mqttPacket);
				}

				break;

			case 2: // �յ�publish��qosΪ2���豸�Ȼظ�Rec
					// ƽ̨�ظ�Rel���豸�ٻظ�Comp
				if (MQTT_PacketPublishRec(pkt_id, &mqttPacket) == 0)
				{
					printf("Tips:	Send PublishRec\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
					MQTT_DeleteBuffer(&mqttPacket);
				}
				break;

			default:
				break;
			}
		}

		break;

	case MQTT_PKT_PUBACK: // ����Publish��Ϣ��ƽ̨�ظ���Ack

		if (MQTT_UnPacketPublishAck(cmd) == 0)
			printf("Tips:	MQTT Publish Send OK\r\n");
		Connect_Net = 60; // �����ɹ�
		break;

	case MQTT_PKT_PUBREC: // ����Publish��Ϣ��ƽ̨�ظ���Rec���豸��ظ�Rel��Ϣ

		if (MQTT_UnPacketPublishRec(cmd) == 0)
		{
			printf("Tips:	Rev PublishRec\r\n");
			if (MQTT_PacketPublishRel(MQTT_PUBLISH_ID, &mqttPacket) == 0)
			{
				printf("Tips:	Send PublishRel\r\n");
				ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
				MQTT_DeleteBuffer(&mqttPacket);
			}
		}

		break;

	case MQTT_PKT_PUBREL: // �յ�Publish��Ϣ���豸�ظ�Rec��ƽ̨�ظ���Rel���豸���ٻظ�Comp

		if (MQTT_UnPacketPublishRel(cmd, pkt_id) == 0)
		{
			printf("Tips:	Rev PublishRel\r\n");
			if (MQTT_PacketPublishComp(MQTT_PUBLISH_ID, &mqttPacket) == 0)
			{
				printf("Tips:	Send PublishComp\r\n");
				ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
				MQTT_DeleteBuffer(&mqttPacket);
			}
		}

		break;

	case MQTT_PKT_PUBCOMP: // ����Publish��Ϣ��ƽ̨����Rec���豸�ظ�Rel��ƽ̨�ٷ��ص�Comp

		if (MQTT_UnPacketPublishComp(cmd) == 0)
		{
			printf("Tips:	Rev PublishComp\r\n");
		}

		break;

	case MQTT_PKT_SUBACK: // ����Subscribe��Ϣ��Ack

		if (MQTT_UnPacketSubscribe(cmd) == 0)
			printf("Tips:	MQTT Subscribe OK\r\n");
		else
			printf("Tips:	MQTT Subscribe Err\r\n");

		break;

	case MQTT_PKT_UNSUBACK: // ����UnSubscribe��Ϣ��Ack

		if (MQTT_UnPacketUnSubscribe(cmd) == 0)
			printf("Tips:	MQTT UnSubscribe OK\r\n");
		else
			printf("Tips:	MQTT UnSubscribe Err\r\n");

		break;

	default:
		result = -1;
		break;
	}
	//---------------------------------------------�������������---------------------------------------------

	ESP8266_Clear(); // ��ջ���

	if (result == -1)
		return;

	dataPtr = strchr(req_payload, ':'); // ����':'

	if (dataPtr != NULL && result != -1) // ����ҵ���
	{
		dataPtr++;
		while (*dataPtr >= '0' && *dataPtr <= '9') // �ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;

		num = atoi((const char *)numBuf); // תΪ��ֵ��ʽ
		// printf( "num:%d\r\n",num);
		if (strstr((char *)req_payload, "Flage")) // ����"redled"
		{
			switch(num){
				case 0:
					Data_init.Flage=0;//�Զ�ģʽ
					break;
				case 1:
					Data_init.Flage=1;//�ֶ�ģʽ
					break;
				default:
					Data_init.Flage=0;//�Զ�ģʽ
					break;
				}
			OneNet_SendMqtt(4); // �ϴ����ĵ�����
			Data_init.App=3;//ָ��ģʽ
		}
	  if (strstr((char *)req_payload, "Led"))
		{
			device_state_init.led_state=num;
			Data_init.App=1;//ָ��ģʽ
		}
	  if (strstr((char *)req_payload, "Fan"))
		{
			device_state_init.fan_state=num;
			Data_init.App=2;//ָ��ģʽ
		}
		if (strstr((char *)req_payload, "Massage"))
		{
			OneNet_SendMqtt(1); // �ϴ����ĵ�����
		}
		
	}

	if (type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}
}

//==========================================================
//	�������ƣ�	void Link_OneNet(u8 Link)
//
//	�������ܣ�	�����ж�
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����
//==========================================================
void Link_OneNet(u8 Link)
{
	if (!Link)
	{
		ESP8266_Init();			 // ��ʼ��ESP8266
		while (OneNet_DevLink()) // ����OneNET
			delay_ms(500);
	}
}
//==========================================================
//	�������ƣ�	OneNet_Subscribe
//
//	�������ܣ�	����
//
//	��ڲ�����	topics�����ĵ�topic
//				topic_cnt��topic����
//
//	���ز�����	SEND_TYPE_OK-�ɹ�	SEND_TYPE_SUBSCRIBE-��Ҫ�ط�
//
//	˵����
//==========================================================
_Bool OneNet_Subscribe(const char *topics[], unsigned char topic_cnt)
{

	unsigned char i = 0;

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���

	for (; i < topic_cnt; i++)
	{
		if (MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL2, topics, topic_cnt, &mqttPacket) == 0)
		{
			ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������

			MQTT_DeleteBuffer(&mqttPacket); // ɾ��
		}
		delay_ms(100);
	}

	return 0;
}

//==========================================================
//	�������ƣ�	OneNet_Publish
//
//	�������ܣ�	������Ϣ
//
//	��ڲ�����	topic������������
//				msg����Ϣ����
//
//	���ز�����	0-�ɹ�	1-��Ҫ����
//
//	˵����
//==========================================================
_Bool OneNet_Publish(const char *topic, const char *msg)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���

	// printf( "Publish Topic: %s, Msg: %s\r\n", topic, msg);

	if (MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL1, 0, 1, &mqttPacket) != 1)
	{

		ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
		MQTT_DeleteBuffer(&mqttPacket);						 // ɾ��
	}
	return 0;
}
