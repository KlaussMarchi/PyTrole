#define TAM 300

unsigned long startProgram;
int timeArray[TAM];
int outputArray[TAM];


void printData(){
    for(int x=0; x<TAM; x++){
        float currInput  = 5.0; 
        float currOutput = outputArray[x] * (5.0 / 1024.0);
        float currTime   = timeArray[x]   / 1000.0;
        
        Serial.print(currTime, 3);
        Serial.print(",");
        Serial.print(currInput);
        Serial.print(",");
        Serial.print(currOutput);
        Serial.println();
        delay(10);
    }
}

void setup(){
    Serial.begin(9600);
    pinMode(12, OUTPUT);
    digitalWrite(12, LOW);
    delay(2000);

    while(!Serial.available())
        continue;

    Serial.readString();
    startProgram = millis();
    digitalWrite(12, HIGH);
}

void loop(){
    static unsigned long startTime = micros();
    static int i = 0;

    if(micros() - startTime < 1000)
        return;

    startTime = micros();
    outputArray[i] = analogRead(A0);
    timeArray[i]   = (millis() - startProgram);
    i = (i + 1 < TAM) ? (i + 1) : 0;

    if(i > 0)
        return;

    printData();
    setup();
}

