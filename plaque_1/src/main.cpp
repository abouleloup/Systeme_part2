#include <Arduino.h>
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

Adafruit_MPU6050 mpu;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int trigPin = 27;
const int echoPin = 14;

long duration;
int distanceCm;
int distanceInch;


#define LED 5
#define LED2 4
#define Bp1 12
#define PIN_RED    23 // GIOP23
#define PIN_GREEN  19 // GIOP22
#define PIN_BLUE   18 // GIOP21



//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701



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
      client.subscribe("led");
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
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input  
  pinMode(LED,OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(Bp1,INPUT_PULLUP);
  Serial.begin(115200);
  
  Serial.println("MPU6050 OLED demo");

  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(500);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);



}

void loop() {



    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);


  display.clearDisplay();
  display.setCursor(0, 10);
  //Display distance in cm
  display.print(distanceCm);
  display.print(" cm");
  
  sensors_event_t a, g, tempera;
  mpu.getEvent(&a, &g, &tempera);


  Serial.print("Gyroscope ");
  Serial.print("X: ");
  Serial.print(g.gyro.x, 1);
  Serial.print(" rps, ");
  Serial.print("Y: ");
  Serial.print(g.gyro.y, 1);
  Serial.print(" rps, ");
  Serial.print("Z: ");
  Serial.print(g.gyro.z, 1);
  Serial.println(" rps");

  float axe_x = g.gyro.x;
  float axe_y = g.gyro.y;
  float axe_z = g.gyro.z;
  
  Serial.print(axe_x, 1);
  Serial.print(axe_y, 1);
  Serial.print(axe_z, 1);




  if (digitalRead(Bp1) == 0)
    {
      digitalWrite(LED2,HIGH);
      display.setCursor(0, 25);
      Serial.print("parler");
      display.print("parler!!");
      
    }
  else {
    digitalWrite(LED2,LOW);
    display.setCursor(0, 25);
    display.print("muet!!");
    Serial.print("muet!!");

  }

  display.display(); 


  if (distanceCm < 100) {
  
  analogWrite(PIN_RED,   0);
  analogWrite(PIN_GREEN, 255);
  analogWrite(PIN_BLUE,  0);
  } 


  if (distanceCm < 50 & distanceCm < 100) {
 
  analogWrite(PIN_RED,   255);
  analogWrite(PIN_GREEN, 255);
  analogWrite(PIN_BLUE,  0);
  }

  if (distanceCm < 20){
  
  analogWrite(PIN_RED,   255);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE,  0);
  } 

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




  static char X[7];
  dtostrf(axe_x, 2, 2, X);

  static char Y[7];
  dtostrf(axe_y, 2, 2, Y);

  static char Z[7];
  dtostrf(axe_z, 2, 2, Z);


  DynamicJsonDocument doc(500);

    doc["AXE_X"] = X;
    doc["AXE_Y"] = Y;
    doc["AXE_Z"] = Z;

  char json_string[256];
  serializeJson(doc, json_string);
  client.publish("Gauge", json_string);

  delay(250);  
}
