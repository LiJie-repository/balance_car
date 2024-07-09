#ifndef	__ESP8266_H
#define	__ESP8266_H
#include "stm32f10x.h" 
/*MQTT 连接参数*/
//#define clientId "a1Zw81YmcId.ESP8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1694524012615|"
//#define username "ESP8266&a1Zw81YmcId"
//#define passwd "a8bed5d35c6c689437c6ef2fa8d360044879460f6e6647fb4b8f985180ad42ca"
//#define mqttHostUrl "a1Zw81YmcId.iot-as-mqtt.cn-shanghai.aliyuncs.com"
//#define port "1883"
#define post "/sys/a1FVTVlUjXw/ESP8266/thing/event/property/post"
#define post_name "powerstate"

#define MQTT_set	"AT+MQTTUSERCFG=0,1,\"NULL\",\"ESP8266&a1FVTVlUjXw\",\"1e89f86e841b020f4dac3fc344179cb59340f9d957228e717641e09b665e9e60\",0,0,\"\""
#define MQTT_Client "AT+MQTTCLIENTID=0,\"a1FVTVlUjXw.ESP8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1709899324563|\""
#define MQTT_Pass "AT+MQTTCONN=0,\"a1FVTVlUjXw.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1"

extern unsigned char Property_Data[];		//数据保存数组
void ESP8266_Init(void); 
unsigned char ESP8266_Re(unsigned char *ACK_AT);
void ESP8266_Send(char *property,int Data);
void ESP8266_Received(char *PRO);
u8 esp8266_send_cmd(char *cmd,char *ack,u16 waittime);
u8* esp8266_check_cmd(char *str);
#endif

