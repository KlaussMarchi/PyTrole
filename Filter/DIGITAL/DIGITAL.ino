
float compute(const float newValue){
    static unsigned long startTime;
    static const byte xSize=2, ySize=2;
    static float Xn[xSize] = {0};
    static float Yn[ySize] = {0};

    if(millis() - startTime < 30)
        return Yn[0];

    startTime = millis();

    for(byte n=xSize-1; n>0; n--)
        Xn[n] = Xn[n-1];

    for(byte n=ySize-1; n>0; n--) 
        Yn[n] = Yn[n-1];

    Xn[0] = newValue;
    Yn[0] = Xn[0]*(0.009337) + Xn[1]*(0.009337) + Yn[1]*(0.981326);
    return Yn[0];
}

void setup(){
    Serial.begin(9600);
}

void loop(){
    static float setpoint = 0.0;

    if(Serial.available())
        setpoint = Serial.readString().toFloat();

    float filtered = compute(setpoint);
    plotStates(setpoint, filtered);
}

void plotStates(float setpoint, float filtered){
    static unsigned long startTime;
    static unsigned long attTime;

    if(millis() - startTime < 150)
        return;

    Serial.println(String(setpoint) + "," + String(filtered) + ",0");
}
