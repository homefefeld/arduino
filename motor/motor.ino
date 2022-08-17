
const int PulsePin = 27;

const int dirPin = 33 ;
const int enaPin = 15;
const int BlockLeft  = 13; 
const int BlockRight = 12; 
char Orientation = 'S';
int  IsLeft=0;
int  IsRight=0;
int  incomingByte = 0; 
int  PulsePerCycle=1000;


int setOrientationViaSerialMessage(){
    if (Serial.available() > 0) {  
        incomingByte = Serial.read();
        if ((char)incomingByte == 'L') {
            Orientation = 'L';
            digitalWrite(dirPin, LOW);
            Serial.println("I received: L");
            Serial.println("Turning Left");
            return 1; 
        }
        if ((char)incomingByte == 'R') {
            Orientation = 'R';
            digitalWrite(dirPin, HIGH);
            Serial.println("I received: R");
            Serial.println("Turning Right");
            return 1; 
        }
        if ((char)incomingByte == 'E') {
            Orientation = 'E';
            Serial.println("I received: E");
            Serial.println("Enabling Motor");
            return 1; 
        }
        if ((char)incomingByte == 'D') {
            Orientation = 'D';
            Serial.println("I received: D");
            Serial.println("Disabling Motor");
            return 1; 
        }
    }
    return 0;
}
void EnaMotor() {
    digitalWrite(enaPin, LOW);
    delayMicroseconds(500000);
    Serial.println("Enabled");
    Orientation='S';
}
void DisaMotor() {
    delayMicroseconds(1000000);
    digitalWrite(enaPin, HIGH);
    Serial.println("Disabled");
    Orientation='S';
}
void StopMotor() {
    digitalWrite(enaPin, HIGH);
    Orientation='S';
    Serial.println("Stop Motor");
}
void StepMotor() {
    Serial.print(" Orientation = ");
    Serial.println(Orientation);
    Serial.print(" IsLeft = ");
    Serial.println(IsLeft);
    Serial.print(" IsRight = ");
    Serial.println(IsRight);

    digitalWrite(enaPin, LOW);
    delayMicroseconds(500000);
    double FinalWaitingTime = 75.0; 
    double StartWaitingTime = 250.0; 
    double CurrentWaitingTime = StartWaitingTime;
   
    double iteration = 22000.0;
    double iteration2=   400.0;


    double incrementdouble=(iteration/2)/(StartWaitingTime-FinalWaitingTime);
    int increment=incrementdouble;
    Serial.print(" Current");
    Serial.print(CurrentWaitingTime);
    Serial.print(" increment");
    Serial.print(increment);

    // loop increasing speed up to "iteration"
    
    for (int i = 1; i <= iteration + 1; i++) { 
        if ((i % increment) == 0) {
             CurrentWaitingTime=CurrentWaitingTime-1;
        }
        if (CurrentWaitingTime<FinalWaitingTime) {
             CurrentWaitingTime=FinalWaitingTime;
        }
        digitalWrite(PulsePin, HIGH);
        delayMicroseconds(CurrentWaitingTime);
        digitalWrite(PulsePin, LOW);
        delayMicroseconds(CurrentWaitingTime);
        IsLeft=    digitalRead(BlockLeft);
        IsRight=   digitalRead(BlockRight);
 
        if ((Orientation=='L' and IsLeft == HIGH) or 
           (Orientation=='R' and IsRight == HIGH)) {
             Serial.print(" Stop Early");
             Serial.print(" steps = ");
             Serial.println(i);
             break;
         }
    }
    
    // loop at slow speed up to "iteration2" to reach end of course

    for (int i = 1; i <= iteration2 + 1; i++) { 
        digitalWrite(PulsePin, HIGH);
        delayMicroseconds(StartWaitingTime);
        digitalWrite(PulsePin, LOW);
        delayMicroseconds(StartWaitingTime);
        IsLeft=    digitalRead(BlockLeft);
        IsRight=   digitalRead(BlockRight);
       
        if ((Orientation=='L' and IsLeft == HIGH) or 
           (Orientation=='R' and IsRight == HIGH)) {
             Serial.print(" Stop Rotation");
             Serial.print(" steps = ");
             Serial.print(iteration);
             Serial.print(" + ");
             Serial.println(i);
             break;
         }
    }
    Serial.print(" Orientation = ");
    Serial.println(Orientation);
    Serial.print(" IsLeft = ");
    Serial.println(IsLeft);
    Serial.print(" IsRight = ");
    Serial.println(IsRight);
    Serial.print(" Current");
    Serial.print(CurrentWaitingTime);
    StopMotor();
}


void setup()
{
    // Declare Serial baud
    Serial.begin(9600);
    
    // Declare pins as Outputs
    pinMode(PulsePin, OUTPUT);
    pinMode(dirPin,  OUTPUT);
    pinMode(enaPin,  OUTPUT);


    // Declare pins as Inputs
    pinMode(BlockLeft,  INPUT);
    pinMode(BlockRight, INPUT);

}


void loop()
{
    setOrientationViaSerialMessage();
    IsLeft=    digitalRead(BlockLeft);
    IsRight=   digitalRead(BlockRight);
    


    if ((Orientation=='L' and IsLeft != HIGH) or 
        (Orientation=='R' and IsRight != HIGH)) {
        Serial.println("calling StepMotor ()" );
        Serial.print(" IsLeft = ");
        Serial.println(IsLeft);
        Serial.print(" IsRight = ");
        Serial.println(IsRight);
        StepMotor();
    }
    if (Orientation=='E') {
        EnaMotor();
    }
    if (Orientation=='D') {
        DisaMotor();
    }
    //if (Orientation =='L' or Orientation =='R' or Orientation =='E'){ 
    ////  StopMotor();
    //}
    
    
//    if ((Orientation=='L' and IsLeft != HIGH) or 
//        (Orientation=='R' and IsRight != HIGH)) {
//       StepMotor();
//    }
}
