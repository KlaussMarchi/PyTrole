#include <Servo.h>
#include <ArduinoJson.h>

#define TRIGGER_PIN 8
#define ECHO_PIN 9
#define SERVO_PIN 11
#define PI 3.14159
Servo servo;


float applyStep(float t){
    static const int T = 5;

    int output = (135.0 - 45.0)/2.0*sin(1.5*PI/T*t) + (135.0 - 45.0);
    servo.write(output);
    return output;
}

float getOutput(){
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(10);

    float duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}

void setup(){
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    servo.attach(SERVO_PIN);

    Serial.begin(9600);
    applyStep(0);        // PONTO MORTO

    delay(1000);
    Serial.println("READY");

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
