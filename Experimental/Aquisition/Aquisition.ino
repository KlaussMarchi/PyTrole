#include <ArduinoJson.h>

#define SENSOR_PIN   A0
#define ACTUATOR_PIN 5

double applyStep(){
    static unsigned long startTime;
    static int step;

    if(millis() - startTime < 1000)
        return step;

    startTime = millis();
    step = random(0, 255);
    analogWrite(ACTUATOR_PIN, step);
    return step;
}

double getOutput(){
    int analog = analogRead(SENSOR_PIN);
    return analog;
}

void setup(){
    Serial.begin(9600);
    pinMode(ACTUATOR_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);

    randomSeed(millis());
    for(byte x=0; x<10; x++) rand();
    for(byte x=0; x<10; x++) random();

    while(!Serial.available())
        delay(500);
}

void loop(){
    static const unsigned long startTime = millis();
    static unsigned long attTime = millis();

    static StaticJsonDocument<64> data;
    static String response;

    if(millis() - attTime < 50) // T = 0.050 seg
        return;

    attTime = millis();
    data["time"]   = (millis() - startTime) / 1000.0;
    data["input"]  = applyStep();
    data["output"] = getOutput();
    
    serializeJson(data, response);
    Serial.println(response);
}

