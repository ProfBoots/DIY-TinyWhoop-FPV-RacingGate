void beginWifi()
{
  if (!WiFi.softAP(ssid)) {
    log_e("Soft AP creation failed.");
    Serial.println("Soft AP creation failed.");
    while (1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void displayHttp( void * pvParameters)
{
  vTaskDelay(1);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    String html = "<!DOCTYPE html><html><head><style>"
                  "body {"
                  "  display: flex;"
                  "  flex-direction: column;"
                  "  justify-content: center;"
                  "  align-items: center;"
                  "  height: 100vh;"
                  "  margin: 0;"
                  "  background-color: #f2f2f2;"
                  "}"
                  ".timer {"
                  "  text-align: center;"
                  "  font-size: 200px;"
                  "  font-family: Arial, sans-serif;"
                  "  color: red;"
                  "  padding: 20px;"
                  "  background-color: #ccc;"
                  "}"
                  ".info {"
                  "  font-size: 96px;"
                  "  font-family: Arial, sans-serif;"
                  "  margin-bottom: 10px;"
                  "}"
                  "</style></head><body>"
                  "<div class='info'>LAP:<span id='lap'>" + String(lapNum) + "</span></div>"
                  "<div class='info'>Racer:<span id='racer'>" + String(racerNum) + "</span></div>"
                  "<div class='timer' id='timer'>" + String(currentTime) + "</div>"
                  "<div class='info'>Recent Time: <span id='recentTime'>" + String(racerTime) + "</span></div>"
                  "<script>"
                  "function updateTimer() {"
                  "  var xhrTimer = new XMLHttpRequest();"
                  "  xhrTimer.onreadystatechange = function() {"
                  "    if (xhrTimer.readyState === 4 && xhrTimer.status === 200) {"
                  "      document.getElementById('timer').innerHTML = xhrTimer.responseText;"
                  "    }"
                  "  };"
                  "  xhrTimer.open('GET', '/getTimer', true);"
                  "  xhrTimer.send();"
                  "}"
                  "function updateRacer() {"
                  "  var xhrRacer = new XMLHttpRequest();"
                  "  xhrRacer.onreadystatechange = function() {"
                  "    if (xhrRacer.readyState === 4 && xhrRacer.status === 200) {"
                  "      document.getElementById('racer').innerHTML = xhrRacer.responseText;"
                  "    }"
                  "  };"
                  "  xhrRacer.open('GET', '/getRacer', true);"
                  "  xhrRacer.send();"
                  "}"
                  "function updateLap() {"
                  "  var xhrLap = new XMLHttpRequest();"
                  "  xhrLap.onreadystatechange = function() {"
                  "    if (xhrLap.readyState === 4 && xhrLap.status === 200) {"
                  "      document.getElementById('lap').innerHTML = xhrLap.responseText;"
                  "    }"
                  "  };"
                  "  xhrLap.open('GET', '/getLap', true);"
                  "  xhrLap.send();"
                  "}"
                  "function updateRecentTime() {"
                  "  var xhrRecentTime = new XMLHttpRequest();"
                  "  xhrRecentTime.onreadystatechange = function() {"
                  "    if (xhrRecentTime.readyState === 4 && xhrRecentTime.status === 200) {"
                  "      document.getElementById('recentTime').innerHTML = xhrRecentTime.responseText;"
                  "    }"
                  "  };"
                  "  xhrRecentTime.open('GET', '/getRecentTime', true);"
                  "  xhrRecentTime.send();"
                  "}"
                  "setInterval(updateTimer, 100);"
                  "setInterval(updateRacer, 100);"
                  "setInterval(updateLap, 100);"
                  "setInterval(updateRecentTime, 100);"
                  "</script>"
                  "</body></html>";
    request->send(200, "text/html", html);
  });

  // Route for getting the timer value
  server.on("/getTimer", HTTP_GET, [](AsyncWebServerRequest * request) {
    String timer = String(currentTime);
    request->send(200, "text/plain", timer);
  });
  server.on("/getRacer", HTTP_GET, [](AsyncWebServerRequest * request) {
    String racer = String(webRacer);
    if(!(firstPass)){
    request->send(200, "text/plain", racer);
    }
  });

  // Route for getting the lap value
  server.on("/getLap", HTTP_GET, [](AsyncWebServerRequest * request) {
    String lap = String(lapNum);
    request->send(200, "text/plain", lap);
  });

  // Route for getting the recent time value
  server.on("/getRecentTime", HTTP_GET, [](AsyncWebServerRequest * request) {
    String recentTime = String(racerTime);
    if(!(firstPass)){
    request->send(200, "text/plain", recentTime);
    }
    else
    {
      request->send(200, "text/plain", String(""));
    }
  });

  // Start server
  server.begin();
  vTaskDelete(NULL);
}
