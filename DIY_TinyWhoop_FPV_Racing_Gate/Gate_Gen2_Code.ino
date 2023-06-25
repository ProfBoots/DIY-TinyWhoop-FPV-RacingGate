#include <FastLED.h>
#include <NewPing.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
TFT_eSPI_Button key[9];


#define REPEAT_CAL false
#define CALIBRATION_FILE "/TouchCalData1"

const char *ssid = "192.168.4.1";


//Variables
double raceStart = 0;
float currentTime = 0;
int timeOut = 5;
bool racing = false;
int pingParameter = 29;
bool stateChange = false;
bool firstPass = false;
bool browsingSettings = false;
bool browsingResults = false;
String printTime;
int state = 0;
int testBat = 0;
int pingPause = 0;
bool changed = false;

String infoArray[64];
int infoArrayNum = 0;

int lapNum = 1;
int selectedLaps = 1;
int racerNum = 1;
int webRacer = 1;
int selectedRacers = 1;
String racerTime = "";
//Record time variables

//ULTRASONIC CODE1
#define triggerPin 26
#define echoPin 27
#define maxDistance 26
NewPing sonar(triggerPin, echoPin, maxDistance);

//ULTRASONIC CODE2
#define triggerPin2 32
#define echoPin2 33
NewPing sonar2(triggerPin2, echoPin2, maxDistance);

//LEDCODE
#define LED_PIN 13
#define NUM_LEDS 104
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */


#define LED_BUILTIN 22
int recentTimeValue = 0;

AsyncWebServer server(80);

TaskHandle_t Task1;

void setup() {
  Serial.begin(9600);
  beginWifi();

  //ULTRASONIC CODE
  tft.setRotation(3);
  tft.init();
  touch_calibrate();
  Serial.println("before Main");
  drawMain();
  Serial.println("past Main");
  //LEDCODE
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();

  pinMode(LED_BUILTIN, OUTPUT);

  lightLogic(false);

  xTaskCreatePinnedToCore(
    displayHttp, /* Function to implement the task */
    "Task1", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task1,  /* Task handle. */
    0);
  printLapRacers();
}

void loop() {
  //Serial.println("LOOPING");
  // To store the touch coordinates
  uint16_t t_x = 0, t_y = 0;
  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&t_x, &t_y);

  // / Check if any key coordinate boxes contain the touch coordinates

  // for(int x; x<2; x++)
  //  {
  if(!(racing))
  {
    tft.setFreeFont(LOAD_FONT2);
  if (pressed && key[7].contains(t_x, t_y)) {
    key[7].press(true);
  } else {
    key[7].press(false);
  }
  if (pressed && key[1].contains(t_x, t_y)) {
    key[1].press(true);  // tell the button it is pressed
  } else {
    key[1].press(false);  // tell the button it is NOT pressed
  }
  if (key[1].justPressed())
  {
    key[1].drawButton(true);
    drawSettings();
    delay(10);
    tft.setFreeFont(LOAD_FONT2);
  }
  if (key[7].justPressed())
  {
    key[7].drawButton(true);
    drawResults();
    delay(10);
    tft.setFreeFont(LOAD_FONT2);
  }
  }

  
  if (pressed && key[0].contains(t_x, t_y)) {
    key[0].press(true);  // tell the button it is pressed
  } else {
    key[0].press(false);  // tell the button it is NOT pressed
  }
  // }

 
  if (key[0].justPressed())
  {
    tft.setFreeFont(LOAD_FONT2);
    key[0].drawButton(true);
    Serial.print("State: ");
    Serial.println(state);
    if (state == 2)
    {
      Serial.print("State4: ");
      Serial.println(state);
      startResetStop(0);
      currentTime = 0;
      racerNum = 1;
      lapNum = 1;
      declareArrayNull();
      racerTime = "";
      webRacer = 1;
    }
    else {
      stateChange = true;
    }
    delay(10);

  }
  //   Serial.print(pingPause);
  changed = pingCheck();
  if ((changed || stateChange) && pingPause > 20)

  {

    if (firstPass) {
      if (racerNum == selectedRacers || currentTime >= timeOut)
      {
        firstPass = false;
        racerNum = 1;
        declareArrayNull();
        infoArray[infoArrayNum] = "Lap " + String(lapNum);
        infoArrayNum++;
      }
    }
    if (!racing)
    {
      lapNum = 1;
      racerNum = 1;
      webRacer = 1;
      racerTime = "";
      declareArrayNull();
      startResetStop(1);
      raceStart = millis();
      racing = true;
      lightLogic(true);
      tft.fillRect(0, 90, 320, 95, TFT_BLACK);
      firstPass = true;
    }

    else if ((racing && racerNum == selectedRacers) || stateChange)
    {

      if (lapNum == selectedLaps || stateChange)
      {
        racerTime = displayTime(currentTime);
        infoArray[infoArrayNum] = "Racer" + String(racerNum) + " " + racerTime;
        infoArrayNum++;
        webRacer = racerNum;
        if (!stateChange)
        {
          displayTime(currentTime);
          finishedRaceLed();
        }
        startResetStop(2);
        racing = false;
        lightLogic(false);
      }
      else {
        webRacer = racerNum;
        racerTime = displayTime(currentTime);
        infoArray[infoArrayNum] = "Racer" + String(racerNum) + " " + racerTime;
        infoArrayNum++;
        racerNum++;
        lapNum++;
        racerNum = 1;
        infoArray[infoArrayNum] = "Lap " + String(lapNum);
        infoArrayNum++;

      }
    }
    else {
      webRacer = racerNum;
      racerTime = displayTime(currentTime);
      infoArray[infoArrayNum] = "Racer" + String(racerNum) + " " + racerTime;
      infoArrayNum++;
        racerNum++;
    }
    stateChange = false;
    pingPause = 0;
  }
  if (racing)
  {
    currentTime = (millis() - raceStart) / 1000;
  }
  displayTime(currentTime);
  pingPause++;
}
bool pingCheck() {
  for(int zx = 0; zx < 2; zx++)
  {
  unsigned long pingTime     = sonar.ping();
  unsigned long distanceInCm = sonar.convert_cm(pingTime);
  //Serial.print("ping: ");
  // Serial.println(distanceInCm);
  //delay(20);
  unsigned long pingTime2 = sonar2.ping();
  unsigned long distanceInCm2 = sonar2.convert_cm(pingTime2);
  //Serial.print("ping2: ");
  //Serial.println(distanceInCm2);

  if ((distanceInCm <= pingParameter && pingTime > 0) || (distanceInCm2 <= pingParameter && pingTime2 > 0))
  {
    //Serial.println(currentTime);
    return true;
  }
  else if(zx == 1)
  {
    return false;
  }
}
}
void lightLogic(bool command)
{
  if (command)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB(255, 0, 0);
      FastLED.show();
    }
  }
  else if (!command)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB(0, 255, 0);
      FastLED.show();
    }
  }
}
void finishedRaceLed() {
  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS); /* The higher the value 4 the less fade there is and vice versa */
      delay(.3);
    }
    FastLED.show();
    delay(10); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
  }
}
