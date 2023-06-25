/*
 void batLvlGood()
{
    int sensorValue = readBatSensor();
    Serial.println(sensorValue);
    float voltage = sensorValue * (7.00 / 1023.00);
    Serial.print("Voltage = ");
    Serial.println(voltage);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
 // tft.setTextSize(1);
//  tft.setFreeFont(&FreeMonoBold9pt7b);
  String displayVoltage = String(voltage);
  int lastIndex = displayVoltage.length() - 1;
  displayVoltage.remove(lastIndex);
 // tft.fillRect(260, 5, 51, 16, TFT_BLACK);
 // tft.drawString(displayVoltage,260,5);
}
int readBatSensor(){
  int sval = 0;
  int ival = 0;
  for (int i = 0; i < 20; i++){
    ival = analogRead(35);
    sval = sval + ival;
    delay (2);
  }
  return (sval/20);
}
*/
void declareArrayNull() {
  for (int i = 0; i < 64; i++) {
    infoArray[i] = "";
  }
  infoArrayNum = 0;
  tft.fillRect(155,0,165,100, TFT_BLACK);
}
