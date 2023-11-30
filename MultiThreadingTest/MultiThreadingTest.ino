#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

void Task1(void *parameters);
void Task2(void *parameters);


void setup() {
  Serial.begin(115200);

  while(!Serial) {
    delay(10);
  }

  xTaskCreatePinnedToCore(
    Task1
    ,  "Task1"
    ,  1024  
    ,  NULL 
    ,  2  
    ,  NULL
    , ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    Task2
    ,  "Task2"
    ,  1024
    ,  NULL
    ,  2 
    ,  NULL 
    , ARDUINO_RUNNING_CORE);
}

void loop() {

}

void Task1(void *parameters) {
  while(1) {
    // if(xSemaphoreTake( isSerialPortFree, ( TickType_t ) 5 ) == pdTRUE) {
      Serial.println("Task1");
      // xSemaphoreGive(isSerialPortFree);
    // }
    vTaskDelay(1000);
  }
}

void Task2(void *parameters) {
  while(1) {
    // if(xSemaphoreTake( isSerialPortFree, ( TickType_t ) 5 ) == pdTRUE) {
      Serial.println("Task2");
    //   xSemaphoreGive(isSerialPortFree);
    // }
    vTaskDelay(3000);
  }
}
