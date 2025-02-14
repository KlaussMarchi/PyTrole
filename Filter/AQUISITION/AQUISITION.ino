#include <ArduinoJson.h>
#define PIN A5

void setup(){
    Serial.begin(9600);

    while(!Serial.available())
        continue;
}

void loop(){
    static unsigned long startTime = millis();
    StaticJsonDocument<64> data;
    String response;

    data["time"]   = (millis() - startTime) / 1000.0;
    data["output"] = analogRead(PIN);

    serializeJson(data, response);
    Serial.println(response);
}


