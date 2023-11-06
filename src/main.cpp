#include <Arduino.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<driver/gpio.h>


// these constants won't change. They are the lowest and highest readings you
// get from your sensor:
const int sensorMin = 0;    // sensor minimum, discovered through experiment
const int sensorMax = 1500;  // sensor maximum, discovered through experiment
#define PhotoSensor 2      //This is the photosensor input
const TickType_t Delay = 500/ portTICK_PERIOD_MS; // This represent the delay time

/* ===============_First task_================================== */
TaskHandle_t Photosensor_Reading_task = NULL;
void PhotoSensor_task(void *param){
  for(;;){
    // read the sensor:
  int sensorReading = analogRead(PhotoSensor);
  // map the sensor range to a range of four options:
  int range = map(sensorReading, sensorMin, sensorMax, 0, 5);

  // do something different depending on the range value:
  switch (range) {
    case 0:  // your hand is on the sensor
      Serial.println("dark");
      break;
    case 1:  // your hand is close to the sensor
      Serial.println("Shadow");
      break;
    case 2:
      Serial.println("dim");
      break;
    case 3:  // your hand is a few inches from the sensor
      Serial.println("medium");
      break;
    case 4:  // your hand is nowhere near the sensor
      Serial.println("clair");
      break;
    case 5:
      Serial.println("Bright");
      break;
  }
  vTaskDelay(Delay);  // delay in between reads for stability
  }

}
/* ================================================================== */


void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  pinMode(PhotoSensor, INPUT);
}

void loop() {
  xTaskCreatePinnedToCore(PhotoSensor_task, "PhotoSensor_task", 5000, NULL, 2, &Photosensor_Reading_task, 1); //Call the task to core 1
}
