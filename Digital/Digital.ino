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

float control(const float input, const float setpoint){
    static unsigned long startTime = millis();
    static const float bounds[] = {-1e9, 1e9};
    static const int dt = 50; // ms
    static float Xn1, Xn2;
    static float Yn1, Yn2;

    if(millis() - startTime < dt)
        return Yn1;

    startTime = millis();
    const float Xn = (setpoint - input);
    const float Yn = Xn*(0.004683) + Xn1*(0.004381) + Yn1*(1.818731) + Yn2*(-0.818731);

    const float output = (Yn < bounds[0]) ? bounds[0] :
                         (Yn > bounds[1]) ? bounds[1] : Yn;
    Xn2 = Xn1; Xn1 = Xn;
    Yn2 = Yn1; Yn1 = output;
    return Yn;
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
    float output = control(input, setpoint);

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

