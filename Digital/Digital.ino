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

float compute(float input, float setpoint){
    static unsigned long startTime;
    static const byte xSize=2, ySize=2;
    static float bounds[2] = {-500, 500};
    static float Xn[xSize] = {0};
    static float Yn[ySize] = {0};

    if(millis() - startTime < 100)
        return Yn[0];

    startTime = millis();

    for(byte n=xSize-1; n>0; n--) 
        Xn[n] = Xn[n-1];
    
    for(byte n=ySize-1; n>0; n--) 
        Yn[n] = Yn[n-1];

    Xn[0] = (setpoint - input);
    Yn[0] = Xn[0]*(19.863569) + Xn[1]*(-14.769406) + Yn[1]*(0.577497);
    
    if(Yn[0] > bounds[1]) Yn[0] = bounds[1];
    if(Yn[0] < bounds[0]) Yn[0] = bounds[0];
    return Yn[0];
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
    float setpoint = 3;
    float input  = getInput(); 
    float output = compute(input, setpoint);

    setOutput(output); 
    plotStates(input, output, setpoint);
    delay(10);
}

void plotStates(float input, float output, float setpoint){
    Serial.print(input);  
    Serial.print(",");
    Serial.print(output);
    Serial.print(",");
    Serial.print(setpoint); 
    Serial.println(",0,5");
}

