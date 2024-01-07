/* ProfessorBoots
   John Cheroske 1/6/2024
   MiniSkidi 3.0

   Thank you to the following people for contributing to this sketch
   -TomVerr99 "Excellent Job organizing what was a very messy intial sketch"
   -CrabRC "I dont even know where to start, but thank you for making numerous improvemnts/suggestions
   across both mechanical designs and software."
   -Fortinbra "Always willing to provide the discord group with a good meme or two, as well as lend a helping hand
   in multiple ways."

  Some tidbits to check

  -Install the esp32 boards manager into the arduino IDE"
  Programming Electronics Academy has a good tutorial: https://youtu.be/py91SMg_TeY?si=m1OWPBPlK-QHJ2Xx"
  -Select "ESP32 Dev Module" under tools>Board>ESP32 Arduino before uploading sketch.
  -The following include statements with comments "by -----" are libraries that can be installed
  directly inside the arduino IDE under Sketch>Include Library>Manage Libraries
*/
#include <Arduino.h>

#include <ESP32Servo.h> // by Kevin Harrington
#include <ESPAsyncWebSrv.h> // by dvarrel
#include <iostream>
#include <sstream>

#if defined(ESP32)
#include <AsyncTCP.h> // by dvarrel
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESPAsyncTCP.h> // by dvarrel
#endif


// defines
#define bucketServoPin  23
#define auxServoPin 22
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

// global constants
const char* ssid     = "ProfBoots MiniSkidi";

// global variables

Servo bucketServo;
Servo auxServo;

bool horizontalScreen;//When screen orientation is locked vertically this rotates the D-Pad controls so that forward would now be left.
bool removeArmMomentum = false;

struct MOTOR_PINS
{
  int pinIN1;
  int pinIN2;
};

std::vector<MOTOR_PINS> motorPins =
{
  {32, 33},  //RIGHT_MOTOR Pins (IN1, IN2)
  {26, 25},  //LEFT_MOTOR  Pins
  {19, 21}, //ARM_MOTOR pins
};

AsyncWebServer server(80);
AsyncWebSocket wsCarInput("/CarInput");


void rotateMotor(int motorNumber, int motorDirection)
{
  if (motorDirection == FORWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  }
  else if (motorDirection == BACKWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);
  }
  else
  {
    if (removeArmMomentum)
    {
      digitalWrite(motorPins[ARM_MOTOR].pinIN1, HIGH);
      digitalWrite(motorPins[ARM_MOTOR].pinIN2, LOW);
      delay(10);
      digitalWrite(motorPins[motorNumber].pinIN1, LOW);
      digitalWrite(motorPins[motorNumber].pinIN2, LOW);
      delay(5);
      digitalWrite(motorPins[ARM_MOTOR].pinIN1, HIGH);
      digitalWrite(motorPins[ARM_MOTOR].pinIN2, LOW);
      delay(10);
      removeArmMomentum = false;
    }
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  }
}

void moveCar(int inputValue)
{
  Serial.printf("Got value as %d\n", inputValue);
  if (!(horizontalScreen))
  {
    switch (inputValue)
    {

      case UP:
        rotateMotor(RIGHT_MOTOR, FORWARD);
        rotateMotor(LEFT_MOTOR, FORWARD);
        break;

      case DOWN:
        rotateMotor(RIGHT_MOTOR, BACKWARD);
        rotateMotor(LEFT_MOTOR, BACKWARD);
        break;

      case LEFT:
        rotateMotor(RIGHT_MOTOR, BACKWARD);
        rotateMotor(LEFT_MOTOR, FORWARD);
        break;

      case RIGHT:
        rotateMotor(RIGHT_MOTOR, FORWARD);
        rotateMotor(LEFT_MOTOR, BACKWARD);
        break;

      case STOP:
        rotateMotor(ARM_MOTOR, STOP);
        rotateMotor(RIGHT_MOTOR, STOP);
        rotateMotor(LEFT_MOTOR, STOP);
        break;

      case ARMUP:
        rotateMotor(ARM_MOTOR, FORWARD);
        break;

      case ARMDOWN:
        rotateMotor(ARM_MOTOR, BACKWARD);
        removeArmMomentum = true;
        break;

      default:
        rotateMotor(ARM_MOTOR, STOP);
        rotateMotor(RIGHT_MOTOR, STOP);
        rotateMotor(LEFT_MOTOR, STOP);
        break;
    }
  } else {
    switch (inputValue)
    {
      case UP:
        rotateMotor(RIGHT_MOTOR, BACKWARD);
        rotateMotor(LEFT_MOTOR, FORWARD);
        break;

      case DOWN:
        rotateMotor(RIGHT_MOTOR, FORWARD);
        rotateMotor(LEFT_MOTOR, BACKWARD);
        break;

      case LEFT:
        rotateMotor(RIGHT_MOTOR, BACKWARD);
        rotateMotor(LEFT_MOTOR, BACKWARD);
        break;

      case RIGHT:
        rotateMotor(RIGHT_MOTOR, FORWARD);
        rotateMotor(LEFT_MOTOR, FORWARD);
        break;

      case STOP:
        rotateMotor(ARM_MOTOR, STOP);
        rotateMotor(RIGHT_MOTOR, STOP);
        rotateMotor(LEFT_MOTOR, STOP);
        break;

      case ARMUP:
        rotateMotor(ARM_MOTOR, FORWARD);
        break;

      case ARMDOWN:
        rotateMotor(ARM_MOTOR, BACKWARD);
        removeArmMomentum = true;
        break;

      default:
        rotateMotor(ARM_MOTOR, STOP);
        rotateMotor(RIGHT_MOTOR, STOP);
        rotateMotor(LEFT_MOTOR, STOP);
        break;
    }
  }
}

void bucketTilt(int bucketServoValue)
{
  bucketServo.write(bucketServoValue);
}
void auxControl(int auxServoValue)
{
  auxServo.write(auxServoValue);
}

void handleRoot(AsyncWebServerRequest *request)
{
  request->send_P(200, "text/html", htmlHomePage);
}

void handleNotFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "File Not Found");
}

void onCarInputWebSocketEvent(AsyncWebSocket *server,
                              AsyncWebSocketClient *client,
                              AwsEventType type,
                              void *arg,
                              uint8_t *data,
                              size_t len)
{
  switch (type)
  {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      moveCar(STOP);
      break;
    case WS_EVT_DATA:
      AwsFrameInfo *info;
      info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
      {
        std::string myData = "";
        myData.assign((char *)data, len);
        std::istringstream ss(myData);
        std::string key, value;
        std::getline(ss, key, ',');
        std::getline(ss, value, ',');
        Serial.printf("Key [%s] Value[%s]\n", key.c_str(), value.c_str());
        int valueInt = atoi(value.c_str());
        if (key == "MoveCar")
        {
          moveCar(valueInt);
        }
        else if (key == "AUX")
        {
          auxControl(valueInt);
        }
        else if (key == "Bucket")
        {
          bucketTilt(valueInt);
        }
        else if (key == "Switch")
        {
          if (!(horizontalScreen))
          {
            horizontalScreen = true;
          }
          else {
            horizontalScreen = false;
          }
        }
      }
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
    default:
      break;
  }
}

void setUpPinModes()
{

  for (int i = 0; i < motorPins.size(); i++)
  {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);
  }
  moveCar(STOP);
  bucketServo.attach(bucketServoPin);
  auxServo.attach(auxServoPin);
  auxControl(150);
  bucketTilt(140);
}


void setup(void)
{
  setUpPinModes();
  Serial.begin(115200);

  WiFi.softAP(ssid );
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);

  wsCarInput.onEvent(onCarInputWebSocketEvent);
  server.addHandler(&wsCarInput);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  wsCarInput.cleanupClients();
}
