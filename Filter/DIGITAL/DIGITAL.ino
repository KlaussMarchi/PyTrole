
float compute(const float Xn){
    static unsigned long startTime = millis();
    static const int dt = 100; // ms
    static float Xn1, Xn2;
    static float Yn1, Yn2;

    if(millis() - startTime < dt)
        return Yn1;

    startTime = millis();
    const float Yn = Xn*(0.000944) + Xn1*(0.001888) + Xn2*(0.000944) + Yn1*(1.911226) + Yn2*(-0.915003);
    Xn2 = Xn1; Xn1 = Xn;
    Yn2 = Yn1; Yn1 = Yn;
    return Yn;
}

void setup(){
    Serial.begin(9600);
}

void loop(){
    static float setpoint = 0.00;

    if(Serial.available())
        setpoint = Serial.readString().toFloat();

    const float output = compute(setpoint);
    plotStates(setpoint, output);
}

void plotStates(float setpoint, float output){
    static unsigned long startTime;
    static const int interval = 200;

    if(millis() - startTime < interval)
        return;

    startTime = millis();
    Serial.println(String(setpoint) + "," + String(output) + ",0");
}
