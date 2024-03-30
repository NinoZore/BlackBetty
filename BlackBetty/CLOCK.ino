const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

void TimeOverNtp() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  //rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
  //rtc.offset = 7200; // change offset value

/*---------set with NTP---------------*/
 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 struct tm timeinfo;
 if (getLocalTime(&timeinfo)){
   rtc.setTimeStruct(timeinfo); 
 }
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}