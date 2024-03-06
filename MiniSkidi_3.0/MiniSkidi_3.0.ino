/*

*/
#include <Arduino.h>

#include <ESP32Servo.h> // by Kevin Harrington
#include <iostream>
#include <sstream>
#include <vector>
#include <Bluepad32.h>

// defines
#define bucketServoPin  23
#define auxServoPin 22
#define lightPin1 18
#define lightPin2 5
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ARMUP 5
#define ARMDOWN 6
#define STOP 0

#define RIGHT_MOTOR 1
#define LEFT_MOTOR 0
#define ARM_MOTOR 2

#define FORWARD 1
#define BACKWARD -1

#define DEAD_ZONE 10

#define MOTOR_PWM_FREQ 5000
#define MOTOR_PWM_RES 8
// global constants

// global variables

Servo bucketServo; // Grabs PWM Channel 0
Servo auxServo; // Grabs PWM Channel 1

bool horizontalScreen;//When screen orientation is locked vertically this rotates the D-Pad controls so that forward would now be left.
bool removeArmMomentum = false;
bool light = false;
int bucket_pos = 140;
int claw_pos = 150;

struct MOTOR_PINS
{
  int pinIN1;
  int pinIN2;
  int channel1;
  int channel2;
};

std::vector<MOTOR_PINS> motorPins =
{
  {25, 26, 2, 3},  //RIGHT_MOTOR Pins
  {33, 32, 4, 5},  //LEFT_MOTOR Pins
  {21, 19, 6, 7},  //ARM_MOTOR Pins
};


void bucketTilt(int bucketServoValue)
{
  bucketServo.write(bucketServoValue);
}
void auxControl(int auxServoValue)
{
  auxServo.write(auxServoValue);
}
void lightControl()
{
  if (!light)
  {
    digitalWrite(lightPin1, HIGH);
    digitalWrite(lightPin2, LOW);
    light = true;
  }
  else
  {
    digitalWrite(lightPin1, LOW);
    digitalWrite(lightPin2, LOW);
    light = false;
  }
}

void setUpPinModes()
{

  for (int i = 0; i < motorPins.size(); i++)
  {
    ledcSetup(motorPins[i].channel1, MOTOR_PWM_FREQ, MOTOR_PWM_RES);
    ledcAttachPin(motorPins[i].pinIN1, motorPins[i].channel1);
    ledcWrite(motorPins[i].channel1, 0);

    ledcSetup(motorPins[i].channel2, MOTOR_PWM_FREQ, MOTOR_PWM_RES);
    ledcAttachPin(motorPins[i].pinIN2, motorPins[i].channel2);
    ledcWrite(motorPins[i].channel2, 0);
  }

  bucketServo.attach(bucketServoPin);
  auxServo.attach(auxServoPin);
  Serial.println(bucketServo.attached());
  Serial.println(auxServo.attached());
  auxControl(claw_pos);
  bucketTilt(bucket_pos);

  pinMode(lightPin1, OUTPUT);
  pinMode(lightPin2, OUTPUT);
}


 /////////////////////////////////


ControllerPtr myControllers[BP32_MAX_CONTROLLERS];

// Arduino setup function. Runs in CPU 1
void setup() {
  // Initialize serial
  Serial.begin(115200);
  while (!Serial) {
    // wait for serial port to connect.
    // You don't have to do this in your game. This is only for debugging
    // purposes, so that you can see the output in the serial console.
    ;
  }
  // Setup IO
  setUpPinModes();
  String fv = BP32.firmwareVersion();
  Serial.print("Firmware version installed: ");
  Serial.println(fv);

  // To get the BD Address (MAC address) call:
  const uint8_t* addr = BP32.localBdAddress();
  Serial.print("BD Address: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(addr[i], HEX);
    if (i < 5)
      Serial.print(":");
    else
      Serial.println();
  }

  // BP32.pinMode(27, OUTPUT);
  // BP32.digitalWrite(27, 0);

  // This call is mandatory. It setups Bluepad32 and creates the callbacks.
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();
}

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.print("CALLBACK: Controller is connected, index=");
      Serial.println(i);
      myControllers[i] = ctl;
      foundEmptySlot = true;

      // Optional, once the gamepad is connected, request further info about the
      // gamepad.
      ControllerProperties properties = ctl->getProperties();
      char buf[80];
      sprintf(buf,
              "BTAddr: %02x:%02x:%02x:%02x:%02x:%02x, VID/PID: %04x:%04x, "
              "flags: 0x%02x",
              properties.btaddr[0], properties.btaddr[1], properties.btaddr[2],
              properties.btaddr[3], properties.btaddr[4], properties.btaddr[5],
              properties.vendor_id, properties.product_id, properties.flags);
      Serial.println(buf);
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(
        "CALLBACK: Controller connected, but could not found empty slot");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundGamepad = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.print("CALLBACK: Controller is disconnected from index=");
      Serial.println(i);
      myControllers[i] = nullptr;
      foundGamepad = true;
      break;
    }
  }

  if (!foundGamepad) {
    Serial.println(
        "CALLBACK: Controller disconnected, but not found in myControllers");
  }
}

void stopAllMotors() {
  for (int i = 0; i < motorPins.size(); i++)
  {
    ledcWrite(motorPins[i].channel1, 0);
    ledcWrite(motorPins[i].channel2, 0);
  }
}

void processGamepad(ControllerPtr gamepad) {
  // Set motor speeds
  int left_yaxis = gamepad->axisY();
  if (abs(left_yaxis) > DEAD_ZONE) {
    // update left motor speed
    int mapped_ly = map(left_yaxis, -512, 512, -255, 255);
    if (mapped_ly < 0) {
      ledcWrite(motorPins[1].channel1, abs(mapped_ly));
      ledcWrite(motorPins[1].channel2, 0);
    } else {
      ledcWrite(motorPins[1].channel1, 0);
      ledcWrite(motorPins[1].channel2, mapped_ly);
    }
  } else {
    // stop motors
    ledcWrite(motorPins[1].channel1, 0);
    ledcWrite(motorPins[1].channel2, 0);
  }

  int right_yaxis = gamepad->axisRY();
  if (abs(right_yaxis) > DEAD_ZONE) {
    // update right motor speed
    int mapped_ry = map(right_yaxis, -512, 512, -255, 255);
    if (mapped_ry < 0) {
      ledcWrite(motorPins[0].channel1, abs(mapped_ry));
      ledcWrite(motorPins[0].channel2, 0);
    } else {
      ledcWrite(motorPins[0].channel1, 0);
      ledcWrite(motorPins[0].channel2, mapped_ry);
    }
  } else {
    // stop motors
    ledcWrite(motorPins[0].channel1, 0);
    ledcWrite(motorPins[0].channel2, 0);
  }
  // set arm motor
  int right_xaxis = gamepad->axisRX();
  if (abs(right_xaxis) > DEAD_ZONE) {
    int mapped_rx = map(right_xaxis, -512, 512, -255, 255);
    if (mapped_rx < 0) {
      ledcWrite(motorPins[2].channel1, 0);
      ledcWrite(motorPins[2].channel2, abs(mapped_rx));
    } else {
      ledcWrite(motorPins[2].channel1, mapped_rx);
      ledcWrite(motorPins[2].channel2, 0);
    }
  } else {
    // stop motors
    ledcWrite(motorPins[2].channel1, 0);
    ledcWrite(motorPins[2].channel2, 0);
  }
  // set bucket servo
  int left_xaxis = gamepad->axisX();
  if (abs(left_xaxis) > DEAD_ZONE) {
    int temp_bucket_pos = bucket_pos + map(left_xaxis, -512, 512, -2, 2);
    if (temp_bucket_pos > 180 | temp_bucket_pos < 0) {
      gamepad->setRumble(0xc0 /* force */, 0x50 /* duration */);
    }
    int new_bucket_pos = constrain(temp_bucket_pos, 0, 180);
    if (new_bucket_pos != bucket_pos) {
      bucket_pos = new_bucket_pos;
      bucketTilt(bucket_pos);
    }
  } 
  // set claw servo

  // Set lights
  if (gamepad->a()) {
    Serial.println("Toggle lights");
    lightControl();
  }

  if (gamepad->b()) {
    // Turn on the 4 LED. Each bit represents one LED.
    static int led = 0;
    led++;
    // Some gamepads like the DS3, DualSense, Nintendo Wii, Nintendo Switch
    // support changing the "Player LEDs": those 4 LEDs that usually indicate
    // the "gamepad seat".
    // It is possible to change them by calling:
    gamepad->setPlayerLEDs(led & 0x0f);
  }

  if (gamepad->x()) {
    // Duration: 255 is ~2 seconds
    // force: intensity
    // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S support
    // rumble.
    // It is possible to set it by calling:
    gamepad->setRumble(0xc0 /* force */, 0x50 /* duration */);
  }

  // Another way to query the buttons, is by calling buttons(), or
  // miscButtons() which return a bitmask.
  // Some gamepads also have DPAD, axis and more.
  char buf[256];
  snprintf(buf, sizeof(buf) - 1,
           "idx=%d, dpad: 0x%02x, buttons: 0x%04x, "
           "axis L: %4li, %4li, axis R: %4li, %4li, "
           "brake: %4ld, throttle: %4li, misc: 0x%02x, "
           "gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d, "
           "battery: %d",
           gamepad->index(),        // Gamepad Index
           gamepad->dpad(),         // DPAD
           gamepad->buttons(),      // bitmask of pressed buttons
           gamepad->axisX(),        // (-511 - 512) left X Axis
           gamepad->axisY(),        // (-511 - 512) left Y axis
           gamepad->axisRX(),       // (-511 - 512) right X axis
           gamepad->axisRY(),       // (-511 - 512) right Y axis
           gamepad->brake(),        // (0 - 1023): brake button
           gamepad->throttle(),     // (0 - 1023): throttle (AKA gas) button
           gamepad->miscButtons(),  // bitmak of pressed "misc" buttons
           gamepad->gyroX(),      // Gyro X
           gamepad->gyroY(),      // Gyro Y
           gamepad->gyroZ(),      // Gyro Z
           gamepad->accelX(),     // Accelerometer X
           gamepad->accelY(),     // Accelerometer Y
           gamepad->accelZ(),     // Accelerometer Z
           gamepad->battery()       // 0=Unknown, 1=empty, 255=full
  );
  Serial.println(buf);

  // You can query the axis and other properties as well. See
  // Controller.h For all the available functions.
}

// Arduino loop function. Runs in CPU 1
void loop() {
  // This call fetches all the controller info from the NINA (ESP32) module.
  // Just call this function in your main loop.
  // The controllers pointer (the ones received in the callbacks) gets updated
  // automatically.
  BP32.update();

  // It is safe to always do this before using the controller API.
  // This guarantees that the controller is valid and connected.
  for (int i = 0; i < BP32_MAX_CONTROLLERS; i++) {
    ControllerPtr myController = myControllers[i];

    if (myController && myController->isConnected()) {
      if (myController->isGamepad())
        processGamepad(myController);
    } else {
      stopAllMotors();
    }
  }
  vTaskDelay(1);
}
