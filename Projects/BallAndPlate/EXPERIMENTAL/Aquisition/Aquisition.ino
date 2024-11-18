#include <ArduinoJson.h>
#include <Servo.h>

#define TRIGGER_PIN 8
#define ECHO_PIN 9
#define SERVO_PIN 11
#define PI 3.14159
Servo servo;

double applyStep(){
    static unsigned long startTime = millis();
    const float t = millis() - startTime;
    const float T = 0.5; 

    int step = 15.0*sin(t * PI/T);
    servo.write(90 + step);
    return step;
}

double getOutput(){
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(10);

    float duration = pulseIn(ECHO_PIN, HIGH);
    return duration * (0.034/2.0);
}

void setup(){
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    servo.attach(SERVO_PIN);
    Serial.begin(9600);
    servo.write(90);        // PONTO MORTO

    while(!Serial.available())
        delay(500);
}

void loop(){
    static const unsigned long startTime = millis();
    static unsigned long attTime = millis();

    static StaticJsonDocument<64> data;
    static String response;

    if(millis() - attTime < 100) // T = 0.1 seg
        return;

    attTime = millis();
    data["time"]   = (millis() - startTime) / 1000.0;
    data["input"]  = applyStep();
    data["output"] = getOutput();
    
    serializeJson(data, response);
    Serial.println(response);
}

