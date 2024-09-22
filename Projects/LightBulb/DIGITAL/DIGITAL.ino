#define LED1 11
#define LED2 10
#define LED3 9

#define LED4 6
#define LED5 5
#define LED6 3
#define LDR A3


void applyStep(int step){
    analogWrite(LED1, step);
    analogWrite(LED2, step);
    analogWrite(LED3, step);
    analogWrite(LED4, step);
    analogWrite(LED5, step);
    analogWrite(LED6, step);
}

double getInput(){
    return analogRead(LDR);
}

double computeController(double inputValue, double setpoint){
    static double Y_n1, Y_n2;
    static double X_n1, X_n2;
    
    double X_n = setpoint - inputValue;
    double Y_n = 8.183009481022019e-06*X_n + 1.3187427003735763e-06*X_n1 -8.942801943412348e-06*X_n2 + 2.0002723822628474*Y_n1-1.0002723991581097*Y_n2;
    
    X_n2 = X_n1;
    X_n1 = X_n; 
    Y_n2 = Y_n1;
    Y_n1 = Y_n;
    return Y_n;
}

void setup(){
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);
    pinMode(LED6, OUTPUT);

    int step = 0;
    analogWrite(LED1, step);
    analogWrite(LED2, step);
    analogWrite(LED3, step);
    analogWrite(LED4, step);
    analogWrite(LED5, step);
    analogWrite(LED6, step);
    
    Serial.begin(9600);
    Serial.println("INICIANDO...");
    delay(2000);
}

void loop() {
    static unsigned long startTime = millis();

    if(millis() - startTime < 5)
        return;

    double setpoint = 800;
    double input  = getInput();

    double output = computeController(input, setpoint);
    applyStep(output);

    Serial.println(String(input) + "," + String(setpoint));

    //Serial.println("input:  " + String(input));
    //Serial.println("output: " + String(output));
    //Serial.println("erro:   " + String(setpoint-input));
    //Serial.println();
    startTime = millis();
}
