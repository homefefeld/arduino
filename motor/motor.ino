
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

// Number of iteration (steps) 
double interation_phase1_increasing_speed = 21000.0;  // phase 1
double interation_phase2_slow_motion      =   660.0;  // phase 2

// sleeptime between motor pulse smaller means faster.
 
double SleepingTimeCruise = 75.0; 
double SleepingTimeInitial = 250.0; 


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

void EndMotor() {
    //digitalWrite(enaPin, HIGH);
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

    double CurentSleepingTime = SleepingTimeInitial;
   
    double incrementdouble = (interation_phase1_increasing_speed/2)/(SleepingTimeInitial-SleepingTimeCruise);
    int increment = incrementdouble;
    
    Serial.print(" Current");
    Serial.print(CurentSleepingTime);
    Serial.print(" increment");
    Serial.print(increment);

    // loop increasing speed up to "interation_phase1_increasing_speed"
    
    for (int i = 1; i <= interation_phase1_increasing_speed + 1; i++) { 
        if ((i % increment) == 0) {
             CurentSleepingTime=CurentSleepingTime-1;
        }
        if (CurentSleepingTime<SleepingTimeCruise) {
             CurentSleepingTime=SleepingTimeCruise;
        }

        IsLeft=    digitalRead(BlockLeft);
        IsRight=   digitalRead(BlockRight);
 
        if ((Orientation=='L' and IsLeft == HIGH) or 
           (Orientation=='R' and IsRight == HIGH)) {
             Serial.print(" Stop Early in Phase 1");
             Serial.print(" steps = ");
             Serial.println(i);
             break;
         } else {
             digitalWrite(PulsePin, HIGH);
             delayMicroseconds(CurentSleepingTime);
             digitalWrite(PulsePin, LOW);
             delayMicroseconds(CurentSleepingTime);
         }
    }
    
    // loop at slow speed up to "interation_phase2_slow_motion" to reach end of course

    for (int i = 1; i <= interation_phase2_slow_motion + 1; i++) { 
        digitalWrite(PulsePin, HIGH);
        delayMicroseconds(SleepingTimeInitial);
        digitalWrite(PulsePin, LOW);
        delayMicroseconds(SleepingTimeInitial);
        IsLeft=    digitalRead(BlockLeft);
        IsRight=   digitalRead(BlockRight);
       
        if ((Orientation=='L' and IsLeft == HIGH) or 
           (Orientation=='R' and IsRight == HIGH)) {
             Serial.print(" Stop Rotation Phase 2");
             Serial.print(" steps = ");
             Serial.print(interation_phase1_increasing_speed);
             Serial.print(" + ");
             Serial.println(i);
             break;
         } else {
             digitalWrite(PulsePin, HIGH);
             delayMicroseconds(SleepingTimeInitial);
             digitalWrite(PulsePin, LOW);
             delayMicroseconds(SleepingTimeInitial);
         }
    }
    Serial.print(" Orientation = ");
    Serial.println(Orientation);
    Serial.print(" IsLeft = ");
    Serial.println(IsLeft);
    Serial.print(" IsRight = ");
    Serial.println(IsRight);
    Serial.print(" Current");
    Serial.print(CurentSleepingTime);
    EndMotor();
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
}
