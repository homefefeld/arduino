
const int PulsePin = 21;

const int dirPin = 19 ;
const int enaPin = 18;
const int BlockLeft  = 23; 
const int BlockRight = 22; 
char Orientation = 'S';
int  IsLeft=0;
int  IsRight=0;
int incomingByte = 0; 
int PulsePerCycle=1000;
int iteration = 22600;


int setOrientationViaSerialMessage(){
    if (Serial.available() > 0) {  
        incomingByte = Serial.read();
        if ((char)incomingByte == 'L') {
            Orientation = 'L';
            digitalWrite(dirPin, LOW);
            Serial.println("I received: L");
            Serial.println("Turning Left");
            return -1; 
        }
        if ((char)incomingByte == 'R') {
            Orientation = 'R';
            digitalWrite(dirPin, HIGH);
            Serial.println("I received: R");
            Serial.println("Turning Right");
            return 1; 
        }
        if ((char)incomingByte == '+') {
            Serial.println("I received: +");
            Serial.println("increase iteration + 10");
            iteration=iteration+10;
             Serial.println(iteration);
            return 0; 
        }
         
        if ((char)incomingByte == '-') {
            Serial.println("I received: -");
            Serial.println("decrease iteration -10");
            iteration=iteration-10;
             Serial.println(iteration);
            return 0; 
        }
    }
    return 0;
}
void EnaMotor() {
    digitalWrite(enaPin, LOW);
    delayMicroseconds(500000);
}
void DisaMotor() {
   delayMicroseconds(1000000);
   digitalWrite(enaPin, HIGH);
}
void StopMotor() {
   DisaMotor();
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

    EnaMotor();
   double waittime = 300.0; 
   double waittime2 = 150.0 ;
   double waittime3 = 75.0; 
   iteration = 22400;
   double increment=(waittime-waittime3)/(iteration/2);
   for (int i = 0; i <= iteration; i++) { 
       waittime=waittime-increment;
       if (waittime<75) {
             waittime=75;
       }
       digitalWrite(PulsePin, HIGH);
       delayMicroseconds(int(waittime));
       digitalWrite(PulsePin, LOW);
       delayMicroseconds(waittime);
       IsLeft=    digitalRead(BlockLeft);
       IsRight=   digitalRead(BlockRight);
 
       
       if ((Orientation=='L' and IsLeft == HIGH) or 
           (Orientation=='R' and IsRight == HIGH)) {
    Serial.print(" Stop Early");
    Serial.print(" Orientation = ");
    Serial.println(Orientation);
    Serial.print(" IsLeft = ");
    Serial.println(IsLeft);
    Serial.print(" IsRight = ");
    Serial.println(IsRight);
    i=iteration;
    }
   }
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
    if (Orientation =='L' or Orientation =='R' or Orientation =='E'){ 
      StopMotor();
    }
    
    
//    if ((Orientation=='L' and IsLeft != HIGH) or 
//        (Orientation=='R' and IsRight != HIGH)) {
//       StepMotor();
//    }
}
