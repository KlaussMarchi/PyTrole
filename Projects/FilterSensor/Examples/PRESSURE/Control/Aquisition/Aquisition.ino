#include <HX711.h>
#include <Wire.h>

#define LOADCELL_DOUT_PIN 4
#define LOADCELL_SCK_PIN 5

HX711 scale;
unsigned long startTime;

void setup(){
    Serial.begin(115200);
    Wire.begin(18, 19);
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    
    while(!Serial.available())
        continue;
    
    startTime = millis();
}

void loop(){
    float pressure   = scale.read();
    float timePassed = (millis() - startTime)/1000.0;
    Serial.println("[" + String(pressure) + "," + String(timePassed) + "]");
}

