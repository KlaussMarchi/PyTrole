#include <PID_v1.h>

double setPoint, input, output;
double kp = 2, ki = 5, kd = 1;
PID myPID(&input, &output, &setPoint, kp, ki, kd, DIRECT);

void setup(){
    Serial.begin(9600);
    Serial.println("starting...");

    input = analogRead(A0);   // Example input from sensor
    setPoint = 100;           // Desired setpoint value
    myPID.SetMode(AUTOMATIC); // Activate PID control
    myPID.SetOutputLimits(0, 255);
}

void loop(){
    static unsigned long startTime = millis();

    if(millis() - startTime < 50)
        return;

    startTime = millis();
    input = analogRead(A0);     // Read current sensor value
    myPID.Compute();            // Calculate PID output
    analogWrite(9, output);     // Apply PID output to the actuator (e.g., motor)

    Serial.println("input: " + String(input) + " | output: " + String(output));
}
