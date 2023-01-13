#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLnfvQijOr"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "o6qyQ2xT5NqdegS1ux7tdDVrTCGiHVaX"



// Update these with values suitable for your network.
const char* ssid = "mohamed";
const char* password = "1234momo";
const char* mqtt_server = "192.168.1.50";




// Insert Firebase project API Key
#define API_KEY "AIzaSyCAiTy-xbSHpI9C51wG9HzbcPflH61RfFs"
#define DATABASE_URL "https://fir-5f221-default-rtdb.europe-west1.firebasedatabase.app/"



WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


#define DHT11PIN 26
#define LED 5
#define LED2 4
#define LDR 33




DHT dht(DHT11PIN, DHT11);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED,HIGH);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    (char)payload[0] == '0';
    digitalWrite(LED, LOW);  // Turn the LED off by making the voltage HIGH
  }


}




void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("led1");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void notifyClient() {
  DynamicJsonDocument doc(256);


  char json_string[256];
  serializeJson(doc, json_string);

  Serial.println(json_string);

  client.publish("ESP32-1", json_string);
}




void setup() {

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(LED,OUTPUT);
  pinMode(LED2, OUTPUT);
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


  int value_LDR = analogRead(LDR);
  Serial.println(value_LDR);


  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
    notifyClient();
    
  }

  static char temperature[7];
  dtostrf(temp, 2, 2, temperature);

  static char humidity[7];
  dtostrf(humi, 2, 2, humidity);



  DynamicJsonDocument doc(500);

    doc["TEMPERATURE"] = temperature;
    doc["HUMIDITY"] = humidity;
    doc["LDR"] = value_LDR;

  char json_string[256];
  serializeJson(doc, json_string);
  client.publish("Gauge", json_string);

  delay(250);  
}