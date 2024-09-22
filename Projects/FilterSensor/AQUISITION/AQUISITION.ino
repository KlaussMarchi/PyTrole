#define PIN A5

float mapLinear(float x, float Xo, float X, float Yo, float Y){
    if(x < Xo)
        return Yo;
    if(x > X)
        return Y;

    return Yo + (Y - Yo)/(X - Xo)*(x - Xo);
}

void setup(){
    Serial.begin(9600);
}

void loop(){
    float x = analogRead(PIN);
    float leitura = mapLinear(x, 0, 1023, 0, 5);

    Serial.println(leitura);
    delay(10)
}

