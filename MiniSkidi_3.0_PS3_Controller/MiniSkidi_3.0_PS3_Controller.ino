#include <Ps3Controller.h>
#include <ESP32Servo.h> // by Kevin Harrington

#define rightMotor0 25
#define rightMotor1 26

#define leftMotor0 33
#define leftMotor1 32

#define armMotor0 21
#define armMotor1 19

#define bucketServoPin  23
#define clawServoPin 22

#define auxLights0 18
#define auxLights1 5

Servo bucketServo;
Servo clawServo;

int bucketServoValue = 140;
int clawServoValue = 150;
int servoDelay = 0;

bool auxLightsOn = false;
bool moveClawServoUp = false;
bool moveClawServoDown = false;
bool moveBucketServoUp = false;
bool moveBucketServoDown = false;


void notify()
{
  //--------------- Digital D-pad button events --------------
  if ( Ps3.event.button_down.up )
  {
    Serial.println("Started pressing the up button");
  }
  if ( Ps3.event.button_up.up )
  {
    Serial.println("Released the up button");
  }
  if ( Ps3.event.button_down.down )
  {
    Serial.println("Started pressing the down button");
  }
  if ( Ps3.event.button_up.down )
  {
    Serial.println("Released the down button");
  }
  if ( Ps3.event.button_down.right )
  {
    Serial.println("Started pressing the right button");
  }
  if ( Ps3.event.button_up.right )
  {
    Serial.println("Released the right button");
  }
  if ( Ps3.event.button_down.left )
  {
    Serial.println("Started pressing the left button");
  }
  if ( Ps3.event.button_up.left )
  {
    Serial.println("Released the left button");
  }
  //---------------- Analog stick value events ---------------
  if ( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ) {
    Serial.print("Moved the left stick:");
    Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
    Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
    Serial.println();
    int LXValue = Ps3.data.analog.stick.lx;
    Serial.print("LXValue ="); Serial.print(LXValue);
    if (LXValue > 115)
    {
      digitalWrite(rightMotor0, LOW);
      digitalWrite(rightMotor1, HIGH);
      delay(10);
      digitalWrite(leftMotor0, HIGH);
      digitalWrite(leftMotor1, LOW);
      Serial.print("Made to into Positive");
    }
    if (LXValue < -115)
    {
      digitalWrite(rightMotor0, HIGH);
      digitalWrite(rightMotor1, LOW);
      delay(10);
      digitalWrite(leftMotor0, LOW);
      digitalWrite(leftMotor1, HIGH);
      Serial.print("Made to into negative");
    }

    int LYValue = Ps3.data.analog.stick.ly;
    if (LYValue > 115)
    {
      digitalWrite(rightMotor0, LOW);
      digitalWrite(rightMotor1, HIGH);
      delay(10);
      digitalWrite(leftMotor0, LOW);
      digitalWrite(leftMotor1, HIGH);
    }
    if (LYValue < -115)
    {
      digitalWrite(rightMotor0, HIGH);
      digitalWrite(rightMotor1, LOW);
      delay(10);
      digitalWrite(leftMotor0, HIGH);
      digitalWrite(leftMotor1, LOW);
    }
    if (LXValue > -30 && LXValue < 30 && LYValue > -30 && LYValue < 30)
    {
      digitalWrite(rightMotor0, LOW);
      digitalWrite(rightMotor1, LOW);
      delay(10);
      digitalWrite(leftMotor0, LOW);
      digitalWrite(leftMotor1, LOW);
    }
  }

  if ( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ) {
    Serial.print("Moved the right stick:");
    Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
    Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
    Serial.println();
    int RXValue = (Ps3.data.analog.stick.rx);
    if (RXValue > 115)
    {
    }
    if (RXValue < -115)
    {
    }

    int RYValue = (Ps3.data.analog.stick.ry);
    if (RYValue > 115)
    {
      digitalWrite(armMotor0, LOW);
      digitalWrite(armMotor1, HIGH);
      delay(10);
    }
    if (RYValue < -115)
    {
      digitalWrite(armMotor0, HIGH);
      digitalWrite(armMotor1, LOW);
      delay(10);
    }
    if (RYValue > -30 && RYValue < 30)
    {
      digitalWrite(armMotor0, LOW);
      digitalWrite(armMotor1, LOW);
    }
  }
  //------------- Digital shoulder button events -------------
  if ( Ps3.event.button_down.l1 )
  {
    moveClawServoUp = true;
    delay(10);
    Serial.println("Started pressing the left shoulder button");
  }
  if ( Ps3.event.button_up.l1 )
  {
    moveClawServoUp = false;
    delay(10);
    Serial.println("Released the left shoulder button");
  }
  if ( Ps3.event.button_down.r1 )
  {
    moveClawServoDown = true;
    delay(10);
    Serial.println("Started pressing the right shoulder button");
  }
  if ( Ps3.event.button_up.r1 )
  {
    moveClawServoDown  = false;
    delay(10);
    Serial.println("Released the right shoulder button");
  }
  //-------------- Digital trigger button events -------------
  if ( Ps3.event.button_down.l2 )
  {
    moveBucketServoDown = true;
    delay(10);
    Serial.println("Started pressing the left trigger button");
  }
  if ( Ps3.event.button_up.l2 )
  {
    moveBucketServoDown = false;
    delay(10);
    Serial.println("Released the left trigger button");
  }
  if ( Ps3.event.button_down.r2 )
  {
    moveBucketServoUp = true;
    delay(10);
    Serial.println("Started pressing the right trigger button");
  }
  if ( Ps3.event.button_up.r2 )
  {
    moveBucketServoUp = false;
    delay(10);
    Serial.println("Released the right trigger button");
  }
  //--- Digital cross/square/triangle/circle button events ---
  if ( Ps3.event.button_down.cross )
  {
    Serial.println("Started pressing the cross button");
  }
  if ( Ps3.event.button_up.cross )
  {
    Serial.println("Released the cross button");
  }
  if ( Ps3.event.button_down.square )
  {
    Serial.println("Started pressing the square button");
  }
  if ( Ps3.event.button_up.square )
  {
    Serial.println("Released the square button");
  }
  if ( Ps3.event.button_down.triangle )
  {
    Serial.println("Started pressing the triangle button");
  }
  if ( Ps3.event.button_up.triangle )
  {
    Serial.println("Released the triangle button");
  }
  if ( Ps3.event.button_down.circle )
  {
    Serial.println("Started pressing the circle button");
  }
  if ( Ps3.event.button_up.circle )
  {
    Serial.println("Released the circle button");
  }
  //--------------- Digital stick button events --------------
  if ( Ps3.event.button_down.l3 )
  {
    Serial.println("Started pressing the left stick button");
  }
  if ( Ps3.event.button_up.l3 )
    Serial.println("Released the left stick button");

  if ( Ps3.event.button_down.r3 )
  {
    if (!auxLightsOn)
    {
      digitalWrite(auxLights0, HIGH);
      digitalWrite(auxLights1, LOW);
      auxLightsOn = true;
    }
    else
    {
      digitalWrite(auxLights0, LOW);
      digitalWrite(auxLights1, LOW);
      auxLightsOn = false;
    }
    Serial.println("Started pressing the right stick button");
  }
  if ( Ps3.event.button_up.r3 )
  {
    Serial.println("Released the right stick button");
  }
  if(moveClawServoUp)
  {
    if(servoDelay == 2)
    {
    if (clawServoValue >= 10 && clawServoValue < 170)
    {
      clawServoValue = clawServoValue + 1;
      clawServo.write(clawServoValue);
    }
    servoDelay = 0;
    }
    servoDelay++;
  }
  if(moveClawServoDown)
  {
    if(servoDelay == 2)
    {
    if (clawServoValue <= 170 && clawServoValue > 10)
    {
      clawServoValue = clawServoValue - 1;
      clawServo.write(clawServoValue);
    }
    servoDelay = 0;
    }
    servoDelay++;
  }
  if(moveBucketServoUp)
  {
    if(servoDelay == 2)
    {
      if(bucketServoValue >= 10 && bucketServoValue < 170)
      {
        bucketServoValue = bucketServoValue + 1;
        bucketServo.write(bucketServoValue);
      }
      servoDelay = 0;
    }
    servoDelay++;
  }
  if(moveBucketServoDown)
  {
    if(servoDelay == 2)
    {
      if(bucketServoValue <= 170 && bucketServoValue > 10)
      {
        bucketServoValue = bucketServoValue - 1;
        bucketServo.write(bucketServoValue);
      }
      servoDelay = 0;
    }
    servoDelay++;
  }
}

void onConnect() {
  Serial.println("Connected.");
}

void setup() {

  Serial.begin(115200);

  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("a0:5a:5a:a0:0f:98");

  Serial.println("Ready.");
  
  pinMode(rightMotor0, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(leftMotor0, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(armMotor0, OUTPUT);
  pinMode(armMotor1, OUTPUT);

  pinMode(auxLights0, OUTPUT);
  pinMode(auxLights1, OUTPUT);
  
  bucketServo.attach(bucketServoPin);
  clawServo.attach(clawServoPin);

  bucketServo.write(bucketServoValue);
  clawServo.write(clawServoValue);

}

void loop() {
  if (!Ps3.isConnected())
    return;
  delay(500);

}
