void startResetStop(int num){
  state = num;
        Serial.print("State2: ");
      Serial.println(state);

  if(state == 0)
  {
  key[0].initButton(&tft,80,218,130,40,TFT_WHITE,TFT_GREEN,TFT_WHITE,"Start", 2);
  tft.fillRect(0, 90, 320, 95, TFT_BLACK);
  key[0].drawButton();
  
  Serial.println("SHIII");
  return;
  }
  else if(state == 1)
  {
  key[0].initButton(&tft,80,218,130,40,TFT_WHITE,TFT_RED,TFT_WHITE,"Stop", 2);
  key[0].drawButton();
  Serial.println("SHIII222222");
  return;
  }
  else if(state == 2);
  {
  key[0].initButton(&tft,80,218,130,40,TFT_WHITE,TFT_RED,TFT_WHITE,"Reset", 2);
  key[0].drawButton();
    Serial.println("SHIII333333");
    return;

  }
        Serial.print("State3: ");
      Serial.println(state);
}
