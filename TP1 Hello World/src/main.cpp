#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#define DHT11PIN 26
#define LED 2
#define Bp1 12

DHT dht(DHT11PIN, DHT11);
void setup() {
  pinMode(LED,OUTPUT);
  pinMode(Bp1,INPUT_PULLUP);
  Serial.begin(115200);
  dht.begin();

}

void loop() {

  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("ºC ");
  Serial.print("Humidity: ");
  Serial.println(humi);
  delay(1000);

  if (digitalRead(Bp1) == 1)
    {
      Serial.print("Bouton_Actif");
      digitalWrite(LED,HIGH);
    }
  else {
    digitalWrite(LED,LOW);
  }
  // put your main code here, to run repeatedly:
}
