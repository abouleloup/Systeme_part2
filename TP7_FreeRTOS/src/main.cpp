
#include <Arduino.h>


#define LED1_GPIO 4
#define LED2_GPIO 2

static SemaphoreHandle_t hsem;

void led_task(void*argp) {
int led = (int)argp;
BaseType_t rc;

pinMode(led,OUTPUT);
digitalWrite(led,0);

for (;;) {
  // First gain control of hsem
  rc= xSemaphoreTake(hsem,portMAX_DELAY);
  assert(rc == pdPASS);

  for( int x=0; x<6; ++x) {
    digitalWrite(led ,digitalRead(led)^1);
    delay(500);
  }
  
  rc = xSemaphoreGive(hsem);
  assert(rc == pdPASS);
} 
}
  


void setup() {
  int app_cpu = xPortGetCoreID();


BaseType_t rc; //Return code 

hsem = xSemaphoreCreateBinary();
assert(hsem);

rc = xTaskCreatePinnedToCore(led_task,"ledtask",3000,(void*)LED1_GPIO,1,nullptr,app_cpu);
assert(rc == pdPASS);

//ALLOW ledtask to start first
rc = xSemaphoreGive(hsem),
assert(rc == pdPASS);

rc = xTaskCreatePinnedToCore(led_task,"ledtask",3000,(void*)LED2_GPIO, 1,nullptr,app_cpu);
  assert(rc == pdPASS);

}

void loop() {
  vTaskDelete(nullptr);
}
