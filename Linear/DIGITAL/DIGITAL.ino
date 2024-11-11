#define SENSOR_PIN   A0
#define ACTUATOR_PIN 5

float getInput(){
    int analog = analogRead(SENSOR_PIN);
    return analog;
}

void setOutput(int output){
    analogWrite(ACTUATOR_PIN, output);
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
    float setpoint = 10; 

    float input  = getInput(); 
    float output = compute(input, setpoint);

    setOutput(output); 
    plot(input, output, setpoint);
}

void plot(float input, float output, float setpoint){
    Serial.print(input);  
    Serial.print(",");
    Serial.print(output);
    Serial.print(",");
    Serial.print(setpoint); 
    Serial.println(",0,100");
}

