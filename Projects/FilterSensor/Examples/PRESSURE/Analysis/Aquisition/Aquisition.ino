#include <HX711.h>
#include <Wire.h>
#include <ArduinoJson.h>

#define LOADCELL_DOUT_PIN 4
#define LOADCELL_SCK_PIN 5

HX711 scale;
unsigned long startTime;


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

float getMean(float *array, int size){
    float sum = 0.0;

    for(int x = 0; x < size; x++)
        sum += array[x];

    return sum / size;
}

float getStandardDeviation(float *array, float mean, int size){
    if(size < 2) 
        return -1;

    float sum = 0.0;

    for(int x = 0; x < size; x++)
        sum += (array[x] - mean) * (array[x] - mean);
    
    float variance = sum / (size - 1);
    return sqrt(variance);
}

float getUncertainty(float mean, float std, int size) {
    float meanUncertainty = std / sqrt(size);
    return (meanUncertainty / mean) * 100;
}

void setup(){
    Serial.begin(115200);
    Wire.begin(18, 19);
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

    unsigned long startWait = millis();
    while(!Serial.available()) {
        if (millis() - startWait > 5000) { // Timeout after 5 seconds
            Serial.println("Serial not available");
            break;
        }
        continue;
    }
    
    startTime = millis();
}

void loop(){
    static const int TAM = 10;
    static int i = 0;
    static float readings[TAM];
    static unsigned long attTime = millis();

    if(millis() - attTime < 10)
        return;

    attTime = millis();
    float pressure   = getPressure();
    float derivative = getDerivative(pressure, 0.010);
    readings[i]      = pressure;

    float mean = getMean(readings, TAM);
    float std  = getStandardDeviation(readings, mean, TAM);
    float rel  = getUncertainty(mean, std, TAM);
    float timePassed = (millis() - startTime) / 1000.0;

    StaticJsonDocument<500> data;
    String response;

    data["pressure"]   = pressure;
    data["derivative"] = derivative;
    data["mean"]       = mean;
    data["std"]        = std;
    data["relative"]   = rel;
    data["time"]       = timePassed;

    serializeJson(data, response);
    Serial.println(response);
    i = (i + 1 < TAM) ? i + 1 : 0;
}
