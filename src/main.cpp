#include <Arduino.h>

#define RED_LED1_PIN 16
#define GREEN_LED1_PIN 17
#define YELLOW_LED1_PIN 18
#define RED_LED2_PIN 19
#define GREEN_LED2_PIN 21
#define YELLOW_LED2_PIN 22
#define LDR_PIN 34

#define PUSH_BUTTON 23
#define A_RED_CHANNEL 0
#define A_GREEN_CHANNEL 1
#define A_YELLOW_CHANNEL 2
#define B_RED_CHANNEL 3
#define B_GREEN_CHANNEL 4
#define B_YELLOW_CHANNEL 5
#define PWM_RESOLUTION 8
#define PWM_FREQUENCY 5000

//Defining the brightness levels
const int DAY_BRIGHTNESS = 255;
const int NIGHT_BRIGHTNESS = 80;

//Defining the Brightness Control
int currentBrightness = 255;
int targetBrightness = 255;

//Defining the Threshold
const int NIGHT_THRESHOLD = 1500;
const int DAY_THRESHOLD = 1800;

//Defining the time the traffic lights are on
const unsigned long allRedTime = 5000;
const unsigned long greenTime = 5000;
const unsigned long yellowTime = 2000;


//Defining the different Traffic Light States to be controlled
enum State{ALLRED1, A_GREEN, A_YELLOW, ALLRED2, B_GREEN, B_YELLOW, EMERGENCY_RED};
State currentState = A_GREEN;

//Defining the Mode
enum Mode{DAY, NIGHT};
Mode currentMode = DAY;

unsigned long previousTime = 0;

void setup()
{
  Serial.begin(115200);

  ledcSetup(A_RED_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(RED_LED1_PIN, A_RED_CHANNEL);

  ledcSetup(A_GREEN_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(GREEN_LED1_PIN, A_GREEN_CHANNEL);

  ledcSetup(A_YELLOW_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(YELLOW_LED1_PIN, A_YELLOW_CHANNEL);

  ledcSetup(B_RED_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(RED_LED2_PIN, B_RED_CHANNEL);

  ledcSetup(B_GREEN_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(GREEN_LED2_PIN, B_GREEN_CHANNEL);

  ledcSetup(B_YELLOW_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(YELLOW_LED2_PIN, B_YELLOW_CHANNEL);

  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  //Initialize the first sequence
  ledcWrite(A_RED_CHANNEL, 0);
  ledcWrite(A_GREEN_CHANNEL, currentBrightness);
  ledcWrite(A_YELLOW_CHANNEL, 0);
  ledcWrite(B_RED_CHANNEL, currentBrightness);
  ledcWrite(B_GREEN_CHANNEL, 0);
  ledcWrite(B_YELLOW_CHANNEL, 0);
  previousTime = millis();
}

void loop()
{
  //Read the sensor value
  int lightValue = analogRead(LDR_PIN);
  unsigned long currentTime = millis();
  int buttonState = digitalRead(PUSH_BUTTON);
  if(buttonState == LOW)
  {
    currentState = EMERGENCY_RED;
    ledcWrite(A_RED_CHANNEL, currentBrightness);
    ledcWrite(A_GREEN_CHANNEL, 0);
    ledcWrite(A_YELLOW_CHANNEL, 0);
    ledcWrite(B_RED_CHANNEL, currentBrightness);
    ledcWrite(B_GREEN_CHANNEL, 0);
    ledcWrite(B_YELLOW_CHANNEL, 0);
    previousTime = currentTime;

  }

  if(currentMode == DAY && lightValue < NIGHT_THRESHOLD)
  {
    currentMode = NIGHT;
    targetBrightness = NIGHT_BRIGHTNESS;
    Serial.println("Switching to NIGHT MODE");
  }
  else if(currentMode == NIGHT && lightValue > DAY_THRESHOLD)
  {
    currentMode = DAY;
    targetBrightness = DAY_BRIGHTNESS;
    Serial.println("Switching to DAY MODE");
  }

  //Smooth Transition and avoid abrupt changes
  if(currentBrightness < targetBrightness)
  {
    currentBrightness++;
  }
  else if(currentBrightness > targetBrightness)
  {
    currentBrightness-- ;
  }
  //Control of the Traffic Light
  switch(currentState)
  {
    case A_GREEN:
    if(currentTime - previousTime >= greenTime)
    {
      currentState = A_YELLOW;
      ledcWrite(A_YELLOW_CHANNEL, currentBrightness);
      ledcWrite(A_GREEN_CHANNEL, 0);
      ledcWrite(A_RED_CHANNEL, 0);
      ledcWrite(B_RED_CHANNEL, currentBrightness);
      ledcWrite(B_GREEN_CHANNEL, 0);
      ledcWrite(B_YELLOW_CHANNEL, 0);
      previousTime = currentTime;
    }
    break;

    case A_YELLOW:
    if(currentTime - previousTime >= yellowTime)
    {
      currentState = ALLRED1;
      ledcWrite(A_RED_CHANNEL, currentBrightness);
      ledcWrite(A_GREEN_CHANNEL, 0);
      ledcWrite(A_YELLOW_CHANNEL, 0);
      ledcWrite(B_RED_CHANNEL, currentBrightness);
      ledcWrite(B_GREEN_CHANNEL, 0);
      ledcWrite(B_YELLOW_CHANNEL, 0);
      previousTime = currentTime;
    }
    break;

    case ALLRED1:
    if(currentTime - previousTime >= allRedTime)
    {
      currentState = B_GREEN;
      ledcWrite(A_RED_CHANNEL, currentBrightness);
      ledcWrite(A_GREEN_CHANNEL, 0);
      ledcWrite(A_YELLOW_CHANNEL, 0);
      ledcWrite(B_RED_CHANNEL, 0);
      ledcWrite(B_GREEN_CHANNEL, currentBrightness);
      ledcWrite(B_YELLOW_CHANNEL, 0);
      previousTime = currentTime;
    }
    break;

    case B_GREEN:
    if(currentTime - previousTime >= greenTime)
    {
      currentState = B_YELLOW;
      ledcWrite(A_RED_CHANNEL, currentBrightness);
      ledcWrite(A_GREEN_CHANNEL, 0);
      ledcWrite(A_YELLOW_CHANNEL, 0);
      ledcWrite(B_RED_CHANNEL, 0);
      ledcWrite(B_GREEN_CHANNEL, 0);
      ledcWrite(B_YELLOW_CHANNEL, currentBrightness);
      previousTime = currentTime;
    }
    break;

    case B_YELLOW:
    if(currentTime - previousTime >= yellowTime)
    {
      currentState = ALLRED2;
      ledcWrite(A_RED_CHANNEL, currentBrightness);
      ledcWrite(A_GREEN_CHANNEL, 0);
      ledcWrite(A_YELLOW_CHANNEL, 0);
      ledcWrite(B_RED_CHANNEL, currentBrightness);
      ledcWrite(B_GREEN_CHANNEL, 0);
      ledcWrite(B_YELLOW_CHANNEL, 0);
      previousTime = currentTime;
    }
    break;

    case ALLRED2:
    if(currentTime - previousTime >= allRedTime)
    {
      currentState = A_GREEN;
      ledcWrite(A_RED_CHANNEL, 0);
      ledcWrite(A_GREEN_CHANNEL, currentBrightness);
      ledcWrite(A_YELLOW_CHANNEL, 0);
      ledcWrite(B_RED_CHANNEL, currentBrightness);
      ledcWrite(B_GREEN_CHANNEL, 0);
      ledcWrite(B_YELLOW_CHANNEL, 0);
      previousTime = currentTime;
    }
    break;

     case EMERGENCY_RED:
    if(currentTime - previousTime >= allRedTime)
    {
      currentState = A_GREEN;
      ledcWrite(A_RED_CHANNEL, 0);
      ledcWrite(A_GREEN_CHANNEL, currentBrightness);
      ledcWrite(A_YELLOW_CHANNEL, 0);
      ledcWrite(B_RED_CHANNEL, currentBrightness);
      ledcWrite(B_GREEN_CHANNEL, 0);
      ledcWrite(B_YELLOW_CHANNEL, 0);
      previousTime = currentTime;
    }
    break;


  }
}


