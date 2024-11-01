#include <ArduinoJson.h>
#define PIN A5

float mapFloat(float x, float Xo, float X, float Yo, float Y){
    float minVal = min(Yo, Y);
    float maxVal = max(Yo, Y);
    float func = (Y-Yo)/(X-Xo)*(x-Xo)+Yo;

    if(func < minVal)
        return minVal;

    if(func > maxVal)
        return maxVal;

    return func;
}


void setup(){
    Serial.begin(115200);

    while(!Serial.available())
        continue;
}

void loop(){
    static unsigned long startTime = millis();
    StaticJsonDocument<64> data;
    String response;

    data["time"]   = (millis() - startTime) / 1000.0;
    data["output"] = mapFloat(analogRead(PIN), 0, 1023, 0.0, 5.0);

    serializeJson(data, response);
    Serial.println(response);
}


