#include "parameters.h"
#include "WifiPass.h"
#include <ESP8266WiFi.h>
#include <NTPClient.h> //NTPClient by Arduino
#include <WiFiUdp.h>
#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h> //RTClib by Adafruit
#include "Scheduler.h"
#include "Audio.h"

#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(14, 12, false, 256); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

Scheduler mScheduler;


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

RTC_DS3231 rtc;
WiFiUDP ntpUDP;

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, NTP_SERVER, GMT_TIME_ZONE * 3600 , 60000);

int timeUpdated = 0;
long lastPrintTime = 0;

void setup() {
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  //DFPlayer Startup
  mySoftwareSerial.begin(9600);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  Serial.println("Waiting to Start....");
  delay(5000);

  syncTime();

  //initAudioAnalisys();


  myDFPlayer.volume(MP3_VOLUME);  //Set volume value. From 0 to 30
  myDFPlayer.loop(1);  //Play the first mp3
}


void syncTime(void) {

  //Connect to Wifi
  //SSID and Password on WifiPass.h file
  WiFi.begin(ssid, password);
  delay(10000); //wait 10 secods for connection

  switch (WiFi.status()) {
    case WL_CONNECTED:
      {
        Serial.println("Wifi Connected...");
        Serial.println("Syncing NTP clock");
        timeClient.begin();
        timeClient.update();

        long actualTime = timeClient.getEpochTime();
        Serial.print("Internet Epoch Time: ");
        Serial.println(actualTime);
        rtc.adjust(DateTime(actualTime));

        Serial.println("RTC Synced with NTP time");
      }
      break;

    case WL_NO_SSID_AVAIL:
      {
        Serial.println("No Wifi SSID detected...");
        Serial.println("Time not synced..");
      }
      break;
  }

  //Turn Off WIFI
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();

}

void loop () {

  if (millis() - lastPrintTime > 10000) {
    lastPrintTime = millis();
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.println();
  }

  //audio();
}