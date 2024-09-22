#include <Servo.h>
#include <ArduinoJson.h>

#define LED1 11
#define LED2 10
#define LED3 9

#define LED4 6
#define LED5 5
#define LED6 3
#define LDR A3


float applyStep(float t){
    int step = 355.0/50.0*t;
    analogWrite(LED1, step);
    analogWrite(LED2, step);
    analogWrite(LED3, step);
    analogWrite(LED4, step);
    analogWrite(LED5, step);
    analogWrite(LED6, step);
    return step;
}

float getOutput(){
    return analogRead(LDR);
}

void setup(){
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);
    pinMode(LED6, OUTPUT);

    int step = 0;
    analogWrite(LED1, step);
    analogWrite(LED2, step);
    analogWrite(LED3, step);
    analogWrite(LED4, step);
    analogWrite(LED5, step);
    analogWrite(LED6, step);

    Serial.begin(9600);
    delay(1000);
    Serial.println("READY");
    randomSeed(millis());
    for(int x=0; x<10; x++);

    while(!Serial.available())
      continue;
    
    delay(2000);
}

void loop(){
    static unsigned long startTime = millis();
    static unsigned long attTime = millis();

    if(millis() - attTime < 100)
        return;
    
    float timePassed = (millis() - startTime)/1000.0;
    StaticJsonDocument<200> data;
    String response;

    data["time"]   = timePassed;
    data["input"]  = applyStep(timePassed);
    data["output"] = getOutput();
    
    serializeJson(data, response);
    Serial.println(response);
}
