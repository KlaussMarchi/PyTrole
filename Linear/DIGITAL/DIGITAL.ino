#define SENSOR_PIN   A0
#define ACTUATOR_PIN 12

float getInput(){
    float analog = analogRead(SENSOR_PIN);
    return analog * (5.0/1023.0);
}

void setOutput(float output){
    int magnitude = output * 255.0/5.0;
    analogWrite(ACTUATOR_PIN, magnitude);
}

double compute(double input, double setpoint){
    static double Y_n1, Y_n2, Y_n3, Y_n4;
    static double X_n1, X_n2, X_n3, X_n4;
    
    double X_n = setpoint - input;
    double Y_n = 0.00212942176167763*X_n -0.002664690122474056*X_n1 - 0.0015464657755570599*X_n2 + 0.0026694083963456627*X_n3 - 0.000578237712266505*X_n4 + 3.949464543967902*Y_n1 - 5.852100723930363*Y_n2 + 3.8557349659892473*Y_n3-0.9530987860267869*Y_n4;

    if(Y_n < 0) Y_n = 0;
    if(Y_n > 5) Y_n = 5;

    X_n4 = X_n3;
    X_n3 = X_n2;
    X_n2 = X_n1;
    X_n1 = X_n; 

    Y_n4 = Y_n3;
    Y_n3 = Y_n2;
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

    if(millis() - startTime < 15)
        return;
    
    startTime = millis();
    float setpoint = 3; 

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
    Serial.println(",0,5");
}

