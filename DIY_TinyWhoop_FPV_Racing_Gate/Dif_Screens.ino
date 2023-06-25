void drawMain()
{

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setFreeFont(&FreeMonoBold9pt7b);
  //tft.drawString("RACERS:", 10, 5, 2);

 // tft.drawRect(258, 3, 55, 20, TFT_WHITE);
 // tft.drawRect(312, 8, 4, 10, TFT_WHITE);

  tft.setFreeFont(LOAD_FONT2);
  key[1].initButton(&tft, 240, 218, 130, 40, TFT_WHITE, TFT_DARKGREY, TFT_WHITE, "SETTINGS", 2);
  key[1].drawButton();
  key[7].initButton(&tft, 50, 20, 120, 30, TFT_WHITE, TFT_DARKGREY, TFT_WHITE, "RESULTS", 2);
  key[7].drawButton();
  startResetStop(state);


}
void drawResults()
{
  tft.fillScreen(TFT_BLACK);

  key[6].initButton(&tft, 50, 25, 135, 50, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "", 4);
  key[6].drawButton();
  tft.setFreeFont(&FreeSansBold18pt7b);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.drawString("MAIN", 10, 10);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setFreeFont(&FreeMonoBold9pt7b);
  int ii = 0;
  bool firstRow = true;
  for (int xx = 0; xx < 240; xx = xx + 145)
  {
    for (int yy = 5; yy < 240; yy = yy + 20)
    {
      if (firstRow)
      {
        yy = yy + 55;
        firstRow = false;
      }
      Serial.print(yy);
      String result = infoArray[ii];
      tft.drawString(result, xx, yy);
      ii++;
    }
  }
  browsingResults = true;
  while (browsingResults)
  {


    uint16_t t_x = 0, t_y = 0;
    // Pressed will be set true is there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);

    // / Check if any key coordinate boxes contain the touch coordinates
    if (pressed && key[6].contains(t_x, t_y)) {
      key[6].press(true);  // tell the button it is pressed
    } else {
      key[6].press(false);  // tell the button it is NOT pressed
    }


    if (key[6].justPressed())
    {
      key[6].drawButton(true);
      drawMain();
      browsingResults = false;
      delay(10);
    }
  }
}

void drawSettings()
{
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(35, 105, 50, 50, TFT_WHITE);
  tft.drawRect(235, 105, 50, 50, TFT_WHITE);
  tft.drawRect(130, -2, 192, 52, TFT_WHITE);
  tft.fillRect(131, -2, 192, 51, TFT_DARKGREY);
  tft.setFreeFont(LOAD_FONT2);
  key[2].initButton(&tft, 60, 78, 50, 34, TFT_WHITE, TFT_RED, TFT_WHITE, "+", 3);
  key[2].drawButton();
  key[3].initButton(&tft, 60, 182, 50, 34, TFT_WHITE, TFT_DARKGREY, TFT_WHITE, "-", 3);
  key[3].drawButton();
  key[4].initButton(&tft, 260, 78, 50, 34, TFT_WHITE, TFT_RED, TFT_WHITE, "+", 3);
  key[4].drawButton();
  key[5].initButton(&tft, 260, 182, 50, 34, TFT_WHITE, TFT_DARKGREY, TFT_WHITE, "-", 3);
  key[5].drawButton();

  key[6].initButton(&tft, 50, 25, 135, 50, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "", 4);
  key[6].drawButton();

  tft.setFreeFont(&FreeSansBold18pt7b);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.drawString("SETTINGS", 135, 10);

  tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.drawString("MAIN", 10, 10);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&FreeMonoBold9pt7b);
  tft.drawString("NUMBER", 28, 205);
  tft.drawString("OF RACERS", 12, 220);
  tft.drawString("NUMBER OF", 210, 205);
  tft.drawString("LAPS", 238, 220);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString("IP ADDRESS:", 100, 110);
  tft.drawString(ssid, 98, 130);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  browsingSettings = true;
  while (browsingSettings)
  {
    uint16_t t_x = 0, t_y = 0;
    // Pressed will be set true is there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);

    // / Check if any key coordinate boxes contain the touch coordinates

    for (int x = 2; x < 7; x++)
    {
      if (pressed && key[x].contains(t_x, t_y)) {
        key[x].press(true);  // tell the button it is pressed
      } else {
        key[x].press(false);  // tell the button it is NOT pressed
      }
    }
    tft.setFreeFont(LOAD_FONT2);
    for (int y = 2; y < 6; y++)
    {
      if (key[y].justReleased()) key[y].drawButton();     // draw normal
    }

    if (key[6].justPressed())
    {
      key[6].drawButton(true);
      drawMain();
      browsingSettings = false;
      delay(10);
    }
    tft.setFreeFont(LOAD_FONT2);
    if (key[2].justPressed())
    {
      key[2].drawButton(true);
      selectedRacers++;
      delay(10);
    }
    if (key[3].justPressed())
    {
      key[3].drawButton(true);
      selectedRacers--;
      delay(10);
    }

    if (key[4].justPressed())
    {
      key[4].drawButton(true);
      selectedLaps++;
      delay(10);
    }

    if (key[5].justPressed())
    {
      key[5].drawButton(true);
      selectedLaps--;
      delay(10);
    }
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setFreeFont(&FreeSansBold18pt7b);
    tft.setTextSize(1);
    tft.fillRect(40, 110, 40, 40, TFT_BLACK);
    tft.fillRect(240, 110, 40, 40, TFT_BLACK);

    tft.drawString(String(selectedRacers), 50, 115);
    tft.drawString(String(selectedLaps), 250, 115);
    delay(10);
  }
  tft.fillRect(40, 110, 40, 40, TFT_BLACK);
  tft.fillRect(240, 110, 40, 40, TFT_BLACK);
}
String displayTime(float currentTime)
{
  String placeHolder = "";
  if (currentTime > 60)
  {
    int minutes = currentTime / 60;
    float secondsAndMilliSeconds = currentTime - (minutes * 60);
    if (secondsAndMilliSeconds < 10.0)
    {
      placeHolder = ":0";
    }
    else {
      placeHolder = ":";
    }
    printTime = String(minutes) + placeHolder + String(secondsAndMilliSeconds);
    printTime.replace(".", ":");
  }
  else
  {
    printTime = String(currentTime);
    printTime.replace(".", ":");
  }
  tft.setTextSize(1);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  if (printTime.length() < 5)tft.drawString(String(printTime), 66, 100, 8);

  else if (printTime.length() == 7)
  {
    printTime.remove(printTime.length() - 1);
    tft.drawString(String(printTime), 20, 100, 8);
  }
  else if (printTime.length() == 8)
  {
    printTime.remove(printTime.length() - 1);
    tft.drawString(String(printTime), 5, 100, 8);
  }

  else tft.drawString(printTime, 40, 100, 8);

  printLapRacers();
  return printTime;
}

void printLapRacers()
{
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  String laps = "Lap " + String(lapNum);
  // String racers = "Racer:" + String(racerNum);

  tft.drawString(laps, 20, 45, 4);
  //tft.drawString(racers, 10, 5, 1);
  tft.setTextSize(1);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  int ii = infoArrayNum;
  if(changed)
  {
  if(!(firstPass))
  {
      tft.fillRect(155,0,165,100, TFT_BLACK);

    for (int yy = 3; yy >= 0; yy--)
    {
      String result = infoArray[ii];
      int printVert = (yy*30) + 5;
      tft.drawString(result, 160, printVert, 4);
      if(ii >=0)
      {
      ii--;
      }
      else break;
    }
  }
  }
    
}
