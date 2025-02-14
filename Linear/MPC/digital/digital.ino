#define SENSOR_PIN   A0
#define ACTUATOR_PIN 2
#define dt 1000
#define M 10

float outputs[M];
int outputIndex;
bool newOutputs;


void handleSerialMsg(String msg){
    int start = msg.indexOf("$");
    int end = msg.indexOf("!");

    if(start == -1 || end == -1)
        return;

    String content = msg.substring(start+1, end);

    if(content.indexOf("outputs") != -1)
        return handleGetOutputs(content);
}

void requestOutputs(){
    int value = analogRead(SENSOR_PIN);
    String request = "input:" + String(map(value, 0, 1024, 0, 100));
    setSerialMsg(request);
}

void handleGetOutputs(String array){
    array = array.substring(array.indexOf('[')+1, array.indexOf(']'));
    int start = 0;
    int index = array.indexOf(',', start);

    for(int x=0; x<M; x++){
        float value = array.substring(start, index).toFloat();
        outputs[x]  = value;

        start = index + 1;
        index = array.indexOf(',', start);

        if(index == -1)
            break;
    }

    newOutputs  = true;
    outputIndex = 0;
}

void handleSetOutputs(){
    static unsigned long startTime = millis();

    if(millis() - startTime < dt)
        return;

    setSerialMsg("escrevendo: " + String(outputs[outputIndex]));
    startTime = millis();
    float output = map(outputs[outputIndex], -100, 100, 0, 255);
    analogWrite(ACTUATOR_PIN, output);
    outputIndex++;
    
    if(outputIndex < M)
        return;

    newOutputs = false;
    requestOutputs();
}

void setup(){
    Serial.begin(9600);
    requestOutputs();
}

void loop(){
    if(newOutputs)
        return handleSetOutputs();

    if(!Serial.available())
        return;

    String response = getSerialMsg();

    if(response == "-1")
        return;
    
    handleSerialMsg(response);
}

String getSerialMsg(){
    const unsigned long startTime = millis();
    String response = Serial.readStringUntil('!') + '!';
    response.trim();
    return (response.length() < 5) ? "-1" : response;
}

void setSerialMsg(String msg){
    msg.trim();

    if(msg.length() == 0)
        return;

    Serial.println("$" + msg + "!");
}
