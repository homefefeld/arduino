// Pinout
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

// number of motor steps for each phase
double nb_steps_phase1 = 21000.0;  // phase 1
double nb_steps_phase2 =   660.0;  // phase 2

// waiting time defines the min and max speed
double waiting_time_max_speed = 75.0; 
double waiting_time_min_speed = 250.0; 

int tick_size_phase1 = int((nb_steps_phase1/2)/(waiting_time_min_speed-waiting_time_max_speed));
int tick_size_phase2 = int((nb_steps_phase2/2)/waiting_time_min_speed);

int increment_phase2 = tick_size_phase2;

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

    double CurentSleepingTime = waiting_time_min_speed;
   
    
    Serial.print(" Current");
    Serial.print(CurentSleepingTime);
    Serial.print(" tick_size_phase1");
    Serial.print(tick_size_phase1);

    // loop increasing speed up to "waiting_time_max_speed"
    
    for (int i = 1; i <= nb_steps_phase1 + 1; i++)  { 
        if ((i % tick_size_phase1) == 0) {
             //at Every tick, decrease the sleeptime
             CurentSleepingTime=CurentSleepingTime-1;
        }
        if (CurentSleepingTime<waiting_time_max_speed) {
             // cap the speed
             CurentSleepingTime=waiting_time_max_speed;
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

    // loop decrease speed up to "waiting_time_mainspeed"
    // loop at slow speed up to "nb_steps_phase2" to reach end of course

    for (int i = 1; i <= nb_steps_phase2 + 1; i++) { 
        if ((i % tick_size_phase2) == 0) {
             //at Every tick, increase the sleeptime
             CurentSleepingTime=CurentSleepingTime-1;
        }
        if (CurentSleepingTime>waiting_time_min_speed) {
             CurentSleepingTime=waiting_time_min_speed;
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
