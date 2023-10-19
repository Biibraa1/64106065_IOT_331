#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

float temperature;
float humidity;
DHT dht14(D4,DHT11);
LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
 temperature = 0;
 humidity = 0;
 Serial.begin(9600);
 dht14.begin();

 lcd.begin();
 lcd.setCursor(1,0);
 lcd.print("Temp:      C");
 lcd.setCursor(1,1);
 lcd.print("Hum:      %");
}

void loop()
{
  humidity = (dht14.readHumidity());
  temperature = (dht14.readTemperature()); 

  Serial.print("temperature:  C");
  Serial.print("humidity:  ");
  Serial.println("%");

  lcd.setCursor(6,0);
  lcd.print(temperature);
  lcd.setCursor(6,1);
  lcd.print(humidity);

  delay(1000);


}