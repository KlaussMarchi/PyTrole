#include <ArduinoJson.h>

#define LED1 11
#define LED2 10
#define LED3 9

#define LED4 6
#define LED5 5
#define LED6 3
#define LDR A3

void allWrite(int step){
    analogWrite(LED1, step);
    analogWrite(LED2, step);
    analogWrite(LED3, step);
    analogWrite(LED4, step);
    analogWrite(LED5, step);
    analogWrite(LED6, step);
}

int applyStep(){
    static unsigned long startTime = millis();
    static int step = 255;
    allWrite(step);

    if(millis() - startTime < 1000)
        return step;

    startTime = millis();
    step = random(0, 255);
    return step;
}

double getOutput(){
    return analogRead(LDR);
}

void setup(){
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);

    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);
    pinMode(LED6, OUTPUT);
    randomSeed(millis());

    for(byte x=0; x<10; x++) rand();
    for(byte x=0; x<10; x++) random();

    Serial.begin(9600);
    allWrite(0);
    Serial.println("READY");

    while(!Serial.available())
        continue;
    
    Serial.println("STARTING");
    delay(2000);
}

void loop(){
    static unsigned long startTime = millis();
    unsigned long timePassed = millis() - startTime / 10; // precisao de 0.01

    if(timePassed % 15 != 0)
        return;
    
    StaticJsonDocument<256> data;
    String response;

    data["input"] = applyStep();
    data["output"] = getOutput();
    data["time"] = (timePassed)/1000.0;

    serializeJson(data, response);
    Serial.println(response);
}
