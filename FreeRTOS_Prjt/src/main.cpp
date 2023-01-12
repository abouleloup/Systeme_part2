#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define LED1_GPIO 4
#define LED2_GPIO 5
#define BP 12
#define PIN_RED 23
#define PIN_BLUE 19
#define PIN_GREEN 18

#define SCREEN_WIDTH 128        // oled display width in pixel
#define SCREEN_HEIGHT 64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


bool condition = false;
bool taskGlobal = false;
bool state = true;
SemaphoreHandle_t hsem;
TaskHandle_t taskA;
TaskHandle_t taskB;


void bpTask(void *argp) {
  
  for (;;) {
    
    if(digitalRead(BP) == LOW && state == true){
      condition = !condition;
      state = false;}
    if(digitalRead(BP) == HIGH && state == false ){state = true;}

    if(condition == true && taskGlobal != true){
      digitalWrite(LED1_GPIO , LOW);
      taskGlobal = true;
      vTaskSuspend(taskA);
      vTaskResume(taskB);
      }

    else if(condition == false && taskGlobal != false){
      taskGlobal = false;
      digitalWrite(LED2_GPIO , LOW);
      vTaskSuspend(taskB);
      vTaskResume(taskA);}
   
    } 
 }

void ledA(void *argp) {

  for (;;) { 
    digitalWrite(LED1_GPIO,digitalRead(LED1_GPIO)^1);
    delay(500);
    
    display.clearDisplay();
    display.setTextSize(1);
    display.println("activation des moteurs...");
    display.display();
    } 
 }

void ledB(void *argp) {
  
  for (;;) {
    digitalWrite(LED2_GPIO,digitalRead(LED2_GPIO)^1);  
    delay(250);
 
    display.clearDisplay();
    display.setTextSize(1);
    display.println("accélération des moteurs...");
    display.display();
    
  }
    
 }

void ledC(void *argp) {
  
  for (;;) {

  }


}

void setup() {
  Serial.begin(115200);
  pinMode(LED1_GPIO , OUTPUT);
  pinMode(LED2_GPIO , OUTPUT);
  pinMode(BP,INPUT_PULLUP);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);



  digitalWrite(LED1_GPIO , HIGH);
  digitalWrite(LED2_GPIO , HIGH);
  delay(2000);
  digitalWrite(LED1_GPIO , LOW);
  digitalWrite(LED2_GPIO , LOW);

  int app_cpu = xPortGetCoreID();


  xTaskCreatePinnedToCore(ledA, "taskA", 3000, NULL, 0, &taskA, app_cpu); 

  xTaskCreatePinnedToCore(ledB, "taskB", 3000, NULL, 0, &taskB, app_cpu); 

  xTaskCreatePinnedToCore(bpTask, "taskBp", 3000, NULL, 0, NULL, 0); 

    vTaskSuspend(taskB);
    vTaskSuspend(taskA);
}

// Not used
void loop() {
 vTaskDelete(nullptr); 

 /*
 if BP ...
 analogWrite(PIN_RED, 255);
 analogWrite(PIN_BLUE, 0);
 analogWrite(PIN_GREEN, 0);
 */ 
 }
