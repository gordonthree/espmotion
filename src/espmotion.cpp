#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

const char* ssid = "Tell my WiFi I love her";
const char* password = "2317239216";
const char* mqtt_server = "192.168.2.30";
const char* mqttMotion = "home/motion1/motion";
const char* mqttPub = "home/motion1/msg";
const char* mqttSub = "home/motion1/cmd";
const char* clientid = "motion2";

ADC_MODE(ADC_VCC);

// static IP address information
IPAddress myIp(192,168,2,62);
IPAddress mySub(255,255,255,0);
IPAddress myDns(192,168,2,1);
IPAddress myGate(192,168,2,1);

long lastMsg = 0;
long conCnt = 0;
char msg[50];
int value = 0;
const uint8_t ledPin = LED_BUILTIN;
const uint8_t motionPin = 5;
const uint8_t beeper = 14;
bool motion=false;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // skipSleep=true;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect(clientid)) {
      // Once connected, publish an announcement...
      client.publish(mqttMotion, "detected");
    } else {
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}

void wifiSetup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(myIp, myGate, mySub, myDns);
  String hostname(clientid);
  WiFi.hostname(hostname);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) { // blocking call that waits for wifi connection. if connection unsuccessful, reboot esp
    ESP.restart();
    delay(2000);
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(beeper, OUTPUT);
  pinMode(motionPin, INPUT);
  digitalWrite(beeper, 0);

  wifiSetup();

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(clientid);

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
  });
  ArduinoOTA.onEnd([]() {
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  });
  ArduinoOTA.onError([](ota_error_t error) {
  });
  ArduinoOTA.begin();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  ArduinoOTA.handle();
  client.loop();

  conCnt++;

  if (conCnt>300) {
    conCnt=0;
    client.publish(mqttPub, "offline");
    client.loop();
    ESP.deepSleep(1000000, WAKE_NO_RFCAL); // sleep, can't wake up since gpio16 is not connected to reset
    delay(5000);
  }

  delay(100);
}