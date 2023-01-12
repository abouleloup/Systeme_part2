#include <Arduino.h>
// semaphores.ino
// Practical ESP32 Multitasking
// Binary Semaphores
#define LED1_GPIO 4
#define LED2_GPIO 5
#define BP 12

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
    } 
 }

void ledB(void *argp) {
  
  for (;;) {
    digitalWrite(LED2_GPIO,digitalRead(LED2_GPIO)^1);  
    delay(250);
  }
    
 }
 
void setup() {
  Serial.begin(115200);
  pinMode(LED1_GPIO , OUTPUT);
  pinMode(LED2_GPIO , OUTPUT);
  pinMode(BP,INPUT_PULLUP);

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
 }