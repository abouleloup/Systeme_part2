#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <Firebase_ESP_Client.h>
#include <SSD1306AsciiWire.h>
#include <WiFi.h>
#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"

#define WIFI_SSID "mohamed"
#define WIFI_PASSWORD "1234momo"

#define API_KEY "AIzaSyC-KavwQ3sYFR7Sm1EGR-52KY4WW6dGaf8"
#define DATABASE_URL "https://tpfire-d8ac6-default-rtdb.europe-west1.firebasedatabase.app/"


#define BUTTON 12
#define DHTPIN 26
#define DHTTYPE DHT11
#define LDR 33
#define LED_ONE 4
#define LED_TWO 5

FirebaseAuth auth;
FirebaseConfig config;

FirebaseData stream;
FirebaseData fbdo;

unsigned long sendDataPrevMillis, updateDisplayPrevMillis = 0;

bool oldButtonState;
bool ledOne, ledTwo = false;

volatile bool dataChanged = false;

DHT dht(DHTPIN, DHTTYPE);
SSD1306AsciiWire oled;

void sendTelemetry() {
  FirebaseJson json;

  json.add("brightness", map(analogRead(LDR), 0, 4095, 100, 0));
  json.add("humidity", dht.readHumidity());
  json.add("temperature", dht.readTemperature());

  Firebase.RTDB.pushJSON(&fbdo, "/ESP32/tele", &json);
}

void sendState() {
  bool state = digitalRead(BUTTON);

  if (state != oldButtonState || dataChanged) {
    FirebaseJson json;

    json.add("button", state);
    json.add("light_one", ledOne);
    json.add("light_two", ledTwo);

    Firebase.RTDB.setJSON(&fbdo, "ESP32/state", &json);
    oldButtonState = state;
  }
}

void updateDisplay() {
  oled.setCursor(75, 0);
  oled.clearField(75, 0, 50);
  oled.print(dht.readTemperature());
  oled.setCursor(75, 1);
  oled.clearField(75, 1, 50);
  oled.print(dht.readHumidity());
  oled.setCursor(75, 2);
  oled.clearField(75, 2, 50);
  oled.print(map(analogRead(LDR), 0, 4095, 100, 0));

  oled.setCursor(75, 4);
  oled.clearField(75, 4, 50);
  oled.print(ledOne ? "ON" : "OFF");
  oled.setCursor(75, 5);
  oled.clearField(75, 5, 50);
  oled.print(ledTwo ? "ON" : "OFF");

  oled.setCursor(75, 7);
  oled.clearField(75, 7, 50);
  oled.print(digitalRead(BUTTON) ? "ON" : "OFF");
}

void streamCallback(FirebaseStream data) {
  Serial.println(data.jsonString());

  FirebaseJsonData result;

  if (data.jsonObject().get(result, "light_one")) {
    ledOne = (bool)result.boolValue;
  }
  if (data.jsonObject().get(result, "light_two")) {
    ledTwo = (bool)result.boolValue;
  }

  dataChanged = true;
}

void streamTimeoutCallback(bool timeout) {
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(),
                  stream.errorReason().c_str());
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON, INPUT);
  pinMode(LED_ONE, OUTPUT);
  pinMode(LED_TWO, OUTPUT);

  dht.begin();

  Wire.begin(21, 22);
  delay(250);
  oled.begin(&Adafruit128x64, 0x3C, -1);
  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.print("Demarrage...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback =
      tokenStatusCallback;  // see addons/TokenHelper.h
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in Test Mode, see
  // Authentications/TestMode/TestMode.ino
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (!Firebase.RTDB.beginStream(&stream, "/ESP32/cmd"))
    Serial.printf("sreams begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.RTDB.setStreamCallback(&stream, streamCallback,
                                  streamTimeoutCallback);

  /** Timeout options, below is default config.
  //WiFi reconnect timeout (interval) in ms (10 sec - 5 min) when WiFi
  disconnected. config.timeout.wifiReconnect = 10 * 1000;
  //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266)
  in ms (1 sec - 1 min). config.timeout.socketConnection = 30 * 1000;
  //ESP32 SSL handshake in ms (1 sec - 2 min). This option doesn't allow in
  ESP8266 core library. config.timeout.sslHandshake = 2 * 60 * 1000;
  //Server response read timeout in ms (1 sec - 1 min).
  config.timeout.serverResponse = 10 * 1000;
  //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's
  keep-alive event data received. config.timeout.rtdbKeepAlive = 45 * 1000;
  //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB
  Stream closed and want to resume. config.timeout.rtdbStreamReconnect = 1 *
  1000;
  //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It
  determines how often the readStream
  //will return false (error) when it called repeatedly in loop.
  config.timeout.rtdbStreamError = 3 * 1000;
  */

  oled.clear();
  oled.setCursor(0, 0);
  oled.print("Humidite: ");
  oled.setCursor(0, 1);
  oled.print("Temperature: ");
  oled.setCursor(0, 2);
  oled.print("Luminosite: ");

  oled.setCursor(0, 4);
  oled.print("LED One: ");
  oled.setCursor(0, 5);
  oled.print("LED Two: ");

  oled.setCursor(0, 7);
  oled.print("Bouton: ");
}

void loop() {
  if (Firebase.ready())
    sendState();

  if (millis() - updateDisplayPrevMillis > 500 ||
      updateDisplayPrevMillis == 0) {
    updateDisplayPrevMillis = millis();
    updateDisplay();
  }
  if (Firebase.ready() &&
      (millis() - sendDataPrevMillis > 30000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    sendTelemetry();
  }

  if (dataChanged) {
    dataChanged = false;
    digitalWrite(LED_ONE, ledOne);
    digitalWrite(LED_TWO, ledTwo);
  }
}