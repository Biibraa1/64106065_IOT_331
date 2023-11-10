#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

float temperature;
float humidity;
DHT HT(D4, DHT11);
const char* WK = "CH3GIV5DQEA4SDNR";
unsigned long channelID = 2340468;
unsigned long timeDelay = 25000;
WiFiClient client;

void init_wifi(String ssid, String password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  init_wifi("SivannaCL", "sushiluckyto");
  HT.begin();
  temperature = 0;
  humidity = 0;
  ThingSpeak.begin(client);
}

void getSensor() {
  humidity = (HT.readHumidity());
  temperature = (HT.readTemperature());
  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);
}

void loop() {
  getSensor();
  int x = ThingSpeak.writeFields(channelID, WK);
  if (x == 200) {
    Serial.println("Sent humidity and temperature to ThingSpeak");
  } else {
    Serial.println("Problem sending to ThingSpeak");
  }
  delay(timeDelay);
}