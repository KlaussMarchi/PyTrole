#define SENSOR_PIN   A0
#define ACTUATOR_PIN 5

float getInput(){
    int analog = analogRead(SENSOR_PIN);
    return mapFloat(analog, 0, 1024, 0, 100);
}

void setOutput(float output){
    int step = mapFloat(output, 0, 100, 0, 255);
    analogWrite(ACTUATOR_PIN, step);
}

double compute(double inputValue, double setpoint){
    static double Y_n1, Y_n2;
    static double X_n1, X_n2;
    
    double X_n = setpoint - inputValue;
    double Y_n = -7.268431752660472*X_n + 15.040765287341745*X_n1 -7.771048331979746*X_n2 + 1.951219512195122*Y_n1-0.951219512195122*Y_n2;

    if(Y_n < 0)   Y_n = 0;
    if(Y_n > 100) Y_n = 100;
    
    X_n2 = X_n1;
    X_n1 = X_n; 
    Y_n2 = Y_n1;
    Y_n1 = Y_n;
    return Y_n;
}

void setup() {
    pinMode(ACTUATOR_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);
    
    Serial.begin(9600);
    setOutput(0);

    Serial.println("INICIANDO...");
    delay(2000);
}

void loop() {
    static unsigned long startTime = millis();

    if(millis() - startTime < 50)
        return;
    
    startTime = millis();

    float setpoint = 50;            // ESCALA 0 - 100
    float input    = getInput();    // SENSOR

    float output = compute(input, setpoint);
    setOutput(output);              // ATUADOR
    plot(input, output, setpoint);
}

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

int mapInt(int x, int Xo, int X, int Yo, int Y){
    int minVal = min(Yo, Y);
    int maxVal = max(Yo, Y);
    int func   = map(x, Xo, X, Yo, Y);

    if(func < minVal)
        return minVal;

    if(func > maxVal)
        return maxVal;

    return func;
}

void plot(int input, int output, int setpoint){
    Serial.print(input);  
    Serial.print(",");
    Serial.print(output);
    Serial.print(",");
    Serial.print(setpoint); 
    Serial.println(",0,100");
}