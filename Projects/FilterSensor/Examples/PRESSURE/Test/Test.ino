#include <HX711.h> 
#include <Wire.h>  

#define LOADCELL_DOUT_PIN 4
#define LOADCELL_SCK_PIN 5
HX711 scale;

void tarePressure(){
    const unsigned long time = millis();

    while(millis() - time < 2000)
        isBlowing(true);
}

double getPressure(){
    static double Y_n1, Y_n2;
    static double X_n1, X_n2;

    double X_n = scale.read();
    double Y_n = 0.03493266184214039*X_n + 0.030566089508458405*X_n1 + 1.6048212946850406*Y_n1-0.6703200460356394*Y_n2;

    X_n2 = X_n1;
    X_n1 = X_n;

    Y_n2 = Y_n1;
    Y_n1 = Y_n;
    return Y_n;
}

float getDerivative(float newValue, float dt){
    static float previousValue = 0.0; // Initialize previousValue
    
    float derivative = (newValue - previousValue) / dt; 
    previousValue = newValue;

    return derivative;
}

bool isBlowing(bool reset){
    static unsigned long attTime = millis();
    static bool is_blowing;

    if(millis() - attTime < 10)
        return is_blowing;

    if(reset)
        is_blowing = false;

    attTime = millis();
    float pressure   = getPressure();
    float derivative = getDerivative(pressure, 0.010);

    if(derivative > 0.7e6)
        is_blowing = true;

    if(derivative < -0.7e6)
        is_blowing = false;

    return is_blowing;    
}

void setup(){
    Serial.begin(115200);
    Wire.begin(18, 19);
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    tarePressure();
}

void loop(){
    Serial.println(isBlowing(false));
}

