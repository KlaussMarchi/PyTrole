#include "DHT.h"
#define DHTPIN  26
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();
    
    pinMode(35, INPUT);
    pinMode(0, OUTPUT);
    digitalWrite(0, LOW);
}

void loop() {
    int   analogico   = analogRead(35);
    float umidade     = dht.readHumidity();
    float temperatura = dht.readTemperature();

    if(isnan(umidade) || isnan(temperatura))
        return;

    Serial.println(String(temperatura) + "," + String(umidade) + "," + String(analogico));
    delay(80);
}

