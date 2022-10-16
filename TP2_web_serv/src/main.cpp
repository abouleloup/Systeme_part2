/*
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const char* ssid = "mohamed";
const char* password = "1234momo";
const char* PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello, world");
    });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
}
*/
/*
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
 
const char* ssid = "mohamed";
const char* password = "1234momo";
 
const char* PARAM_MESSAGE = "message";
 
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
 
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_DATA)
  {
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if(info->final && info->index == 0 && info->len == len)
    {
      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      }
 
      Serial.printf("%s\n",msg.c_str());
    }
  }
}
 
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
 
void setup() {
 
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
 
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
 
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", "<!DOCTYPE html><body><h1>Salut c'est cool</h1><p>Hello World</p></body></html>");
    });
 
    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });
 
    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });
 
    server.onNotFound(notFound);
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
    server.begin();
}
 
void loop() {
  ws.textAll("Coucou");
  delay(1000);
}
*/

#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <DHT.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <Wire.h>

#define BUTTON 32
#define DHTPIN 26
#define DHTTYPE DHT11
#define LED 4
#define LED_CLIENT 2

const char* ssid = "mohamed";
const char* password = "1234momo";

boolean ledState = false;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

DHT dht(DHTPIN, DHTTYPE);


void listAllFiles() {
  File root = LittleFS.open("/");
  File file = root.openNextFile();

  while (file) {
    Serial.print("FILE: ");
    Serial.println(file.name());

    file = root.openNextFile();
  }

  root.close();
}

void notifyClients() {
  const float humidity = dht.readHumidity();
  const float temperature = dht.readTemperature();

  DynamicJsonDocument doc(256);

  doc["button"] = digitalRead(BUTTON);
  doc["led"] = ledState;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;

  char json_string[256];
  serializeJson(doc, json_string);

  Serial.println(json_string);

  ws.textAll(json_string);
}

void handleWebSocketMessage(void* arg, uint8_t* data, size_t len) {
  AwsFrameInfo* info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len &&
      info->opcode == WS_TEXT) {
    data[len] = 0;

    Serial.println((char*)data);

    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }
  }
}

void onWsEvent(AsyncWebSocket* server,
               AsyncWebSocketClient* client,
               AwsEventType type,
               void* arg,
               uint8_t* data,
               size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
                    client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  dht.begin();

  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED_CLIENT, OUTPUT);

  // Initialize SPIFFS
  if (!LittleFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  delay(250);


  Serial.println("\n\n----Listing files----");
  listAllFiles();
  Serial.println("\n\n----END Listing files----");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Route to load root file
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/style.css", "text/css");
  });

  // Route to load function.js file
  server.on("/function.js", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/function.js", "text/js");
  });

  ws.onEvent(onWsEvent);
  server.onNotFound(notFound);
  server.addHandler(&ws);
  server.begin();

 
}

void loop() {
  if (ws.count()) {
    digitalWrite(LED_CLIENT, HIGH);
    notifyClients();
  } else {
    digitalWrite(LED_CLIENT, LOW);
  }
  
  digitalWrite(LED, ledState);

  ws.cleanupClients();
  delay(1000);
}
