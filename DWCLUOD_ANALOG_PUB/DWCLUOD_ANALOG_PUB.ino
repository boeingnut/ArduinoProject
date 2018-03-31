/***
 * LED ON  : mosquitto_pub -h 192.168.1.160 -t iot/deaware/led -m "1"
 * LED OFF : mosquitto_pub -h 192.168.1.160 -t iot/deaware/led -m "0"
 *
 *****/

#include <DWCLOUDMQTT.h>

ESP esp(&Serial1, &Serial, 13);

#define WIFI_SSID                  "DEAWARE_FRONT"
#define WIFI_PASS                  "DEAW1234567890"
#define HOST_IP                    "128.199.203.210"
#define HOST_PORT                  1883
#define MQTT_TOPIC                 "b/gauge"

void wifiCb(uint8_t status)
{
  Serial.println("WIFI: Connected");
  esp.mqttConnect(HOST_IP, HOST_PORT);
}

void mqttConnected(uint32_t* args)
{
  Serial.println("MQTT:Connected");
  esp.subscribe(MQTT_TOPIC);
}

void mqttDisconnected(uint32_t* args)
{
  Serial.println("MQTT:Disconnected");
  esp.mqttConnect(HOST_IP, HOST_PORT);
  Serial.println("MQTT:Reconnect");
}

void mqttPublished(uint32_t* args)
{
  Serial.println("MQTT:Published");
}

void mqttData(uint32_t* args)
{

}

const char clientId[] = "clientId";
const char user[] = "";
const char pass[] = "";


void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
  Serial1.begin(115200);
  Serial.println("Inited");
  
  /* setup event */
  esp.wifiCb.attach(&wifiCb);
  esp.mqttConnected.attach(&mqttConnected);
  esp.mqttDisconnected.attach(&mqttDisconnected);
  esp.mqttPublished.attach(&mqttPublished);
  esp.mqttData.attach(&mqttData);

  /* Init data */
  esp.initMqttClient(clientId, user, pass, 30);
  
  /* wifi connect */
  esp.wifiConnect(WIFI_SSID, WIFI_PASS);
}

char buff[12];
char count = 0;
void loop() {
  count++;
  esp.process();
  if(count > 10) {
    sprintf(buff, "%d", analogRead(0));
    esp.publish(MQTT_TOPIC, buff, strlen(buff), 0, 2);
  if (analogRead(0)>100){
    esp.publish("b/warning", buff, strlen(buff), 0, 2);
    Serial.println("Warning");
  }
    
    count = 0;
  }
}





