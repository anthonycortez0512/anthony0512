
#include <Servo.h>

Servo myservo; // creates servo objects
Servo myservo2;

int forcePin = 0; // analog pin 0 connected to force sensor
int valF;  // value of force sensor
bool TightenReadyState = 1; //lacing postition
bool edgestate = 0;
int Fval = 0;
int Fvalprev = 0;
int th1 = 500; // defines force sensor pressure threshold
int tapcount = 0;
int servoUnlock = 0; // sets main servo to neutral unlaced position (0 degrees)
int servoLock = 180;  // sets main servo to laced position (180 degrees)
int servoUnlock2 = 180; // sets auxillary servo to neutral unlaced position (0 degrees)
int servoLock2 = 0; // sets auxillary servo to laced position (180 degrees)
int delaytime = 3000;
long timelast = millis();

void setup() {
  Serial.begin(9600);

  myservo.attach(10); //attaches the servos to pin 9 and 10
  myservo2.attach(11);
  myservo.write(servoUnlock);  // move both servos into unlaced positions(inital)
  myservo2.write(servoUnlock2);

  //Loosen Laces
}

void loop()
{
  Fvalprev = Fval;
  Fval = analogRead(forcePin); // read value of force sensor
  Serial.print("FVAL ");
  Serial.print(Fval);
  Serial.print(" TightenReadyState ");
  Serial.print(TightenReadyState);
  Serial.print(" edgestate ");
  Serial.println(edgestate );

  if ((Fval > th1) && (Fvalprev < th1)) {
    //edge detected
    edgestate = 1;
  }

  if ( edgestate == 1 && (TightenReadyState == 1)) { //detected a high force
    edgestate = 0;
    TightenReadyState = 0;
    delay(1000); // waits for foot to settle into place in shoe
    myservo2.write(servoLock2); // sets auxillary servo to locked position
    delay(1000);  // waits one second
    myservo.write(servoLock);  // sets main servo to locked position
    delay(1000);  // waits one second
    Serial.println("tighten");
    delay(1000);
  }

  if (millis() - timelast > delaytime) {
    if (edgestate == 1 && (TightenReadyState == 0)) {
      edgestate = 0;

      tapcount++; //increment
      if (tapcount == 3) {
        myservo2.write(servoUnlock2); // unlocks auxillary servo
        myservo.write(servoUnlock);  // unlocks main servo
        Serial.println(" timelast ");
        Serial.println(timelast);
        Serial.println("loosen");
        TightenReadyState = 1;
        tapcount = 0;
      }
    }
    timelast = millis();
  }
}





