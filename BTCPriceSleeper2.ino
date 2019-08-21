#define IOTWEBCONF_DEBUG_PWD_TO_SERIAL 1
#define IOTWEBCONF_DEBUG_TO_SERIAL 1

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <IotWebConf.h>
#include "BitcoinLogo.h"

#include <Timezone.h>   // https://github.com/JChristensen/Timezone

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240};    // Daylight time = UTC - 4 hours
TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300};     // Standard time = UTC - 5 hours
Timezone myTZ(myDST, mySTD);

TimeChangeRule *tcr;        // pointer to the time change rule, use to get TZ abbrev


#include <ArduinoJson.h> 
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] = "BitcoinPrice";

// -- Initial password to connect to the Thing, when it creates an own Access Point.
const char wifiInitialApPassword[] = "ToTheMoon";

DNSServer dnsServer;
WebServer server(80);

IotWebConf iotWebConf(thingName, &dnsServer, &server, wifiInitialApPassword);


// Variables to save date and time
String dayStamp;
String timeStamp;

GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 22));

String price;

void setup() {
  initSerial();
  initDisplay();
  setupWiFi();
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  //initIotWebConf();
  //connectToWifi();
  initTime();
  setTimeZone(); 
  getPrice();
  showPrice();
  deepSleep();
}

void getDateTime(time_t t, const char *tz, char *buf)
{
    //char buf[32];
    char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
    strcpy(m, monthShortStr(month(t)));
    sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d %s",
        hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t), tz);
}

void getCurrentDate(time_t t, const char *tz, char *buf)
{
    char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
    strcpy(m, monthShortStr(month(t)));
    sprintf(buf, "%s %.2d %s %d",
        dayShortStr(weekday(t)), day(t), m, year(t));
}

void getCurrentTime(time_t t, const char *tz, char *buf)
{
    sprintf(buf, "%.2d:%.2d:%.2d %s",
        hour(t), minute(t), second(t), tz);
}

void loop() {
  //delay(1000);
  //Serial.println(".");
}

void initSerial() {
  Serial.begin(921600); 
  while (!Serial);
  delay(3000);
}

void setupWiFi () {
  int i;
  
  /* start SmartConfig */
  WiFi.begin();
  delay(3000);
  i=1;
  displayInfo("Waiting for WiFi.","","");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Waiting for WiFi. Status = "); Serial.println(WiFi.status());
    if( WiFi.status() == WL_NO_SSID_AVAIL || !(i++ % 10)) {
      configureWifi();
      WiFi.printDiag(Serial); 
      Serial.println(WiFi.localIP());
      displayInfo("Waiting for WiFi.","","");
    }
    delay(3000);
  }
    Serial.println( "reconnected from stored credentials");
    WiFi.printDiag(Serial); 
    Serial.println(WiFi.localIP());
    Serial.println("Connected to wifi");
    delay(3000);
    return;

  
}

void configureWifi() {
  iotWebConf.init();

  // -- Set up required URL handlers on the web server.
  server.on("/", handleRoot);
  server.on("/config", []{ iotWebConf.handleConfig(); });
  server.onNotFound([](){ iotWebConf.handleNotFound(); });
  
  Serial.println("SSID: Bitcoin Price Pwd: ToTheMoon  Then Then http://192.168.4.1/config");
  displayInfo("SSID: Bitcoin Price", "Pwd: ToTheMoon", "URL: http://192.168.4.1/config");

  while (1) {
    iotWebConf.doLoop();
    if( WiFi.status() == WL_CONNECTED) break;
  }
  
}

void handleRoot()
{
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    // -- Captive portal request were already served.
    return;
  }
  String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
  s += "<title>IotWebConf 01 Minimal</title></head><body>Hello world!";
  s += "Go to <a href='config'>configure page</a> to change settings.";
  s += "</body></html>\n";

  server.send(200, "text/html", s);
}


void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.println(wakeup_reason);
}

String getWakeupReason(esp_sleep_wakeup_cause_t reason) {
  switch(reason)
  {
    case 1  : return "Wakeup caused by external signal using RTC_IO";
    case 2  : return "Wakeup caused by external signal using RTC_CNTL";
    case 3  : return "Wakeup caused by timer";
    case 4  : return "Wakeup caused by touchpad";
    case 5  : return "Wakeup caused by ULP program";
    default : return "Wakeup was not caused by deep sleep";
  }
}

void wifiConnected() {
  initTime();  
  getPrice();
  showPrice();
  deepSleep();  
}

void initTime() {
    timeClient.begin();
    //timeClient.setTimeOffset(-14400); // GMT - 4 hours (3600 * -4)
    timeClient.update();
}

void setTimeZone() {
  char buf[32];
  
  //setTime(01, 55, 00, 11, 3, 2012);
  time_t currentTime = timeClient.getEpochTime();
  setTime(currentTime);
  time_t utc = now();
  time_t local = myTZ.toLocal(utc, &tcr);
  getDateTime(utc, "UTC", buf);
  Serial.println(buf);
  getDateTime(local, tcr -> abbrev, buf);
  Serial.println(buf);
}

void initDisplay() {
  display.init(115200);
  display.setRotation(1);
  display.setFont(&FreeSansBold18pt7b);
  display.setTextColor(GxEPD_BLACK);
}

const char* host = "api.coindesk.com";
const int httpsPort = 443;
void getPrice(){
  WiFiClientSecure client;
  String on_currency = "USD";
  
  if (!client.connect(host, httpsPort)) {
    Serial.print("Unable to connect: IP = " + WiFi.localIP());
    price = "can't connect";
    return;
  }
  String url = "/v1/bpi/currentprice.json";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP32\r\n" +
               "Connection: close\r\n\r\n");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  String line = client.readStringUntil('\n');
  const size_t capacity = 169*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(168) + 3800;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);

  String temp = doc["bpi"]["USD"]["rate"];
  int pos = temp.indexOf('.');
  
  price = temp.substring(0,pos);
  Serial.println("Current price is " + price);
}

void showPrice() {
  String  formattedDate;
  char    buf[32];
  time_t utc = now();
  time_t local = myTZ.toLocal(utc, &tcr);

  formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");

  // Extract date
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");  Serial.println(dayStamp);

  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");  Serial.println(timeStamp);

  display.setFont(&FreeSansBold18pt7b);
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.drawInvertedBitmap(0, 0, logo, 200, 42, GxEPD_BLACK);
    display.setCursor(65, 70);
    Serial.println("Sending price to display " + price);
    display.print("$" + price);
    display.setCursor(65, 99);
    display.setFont(&FreeSans12pt7b);
    //display.print(dayStamp);
    getCurrentDate(local, tcr -> abbrev, buf);
    display.print(buf);
    //display.print(timeStamp);
    display.setCursor(65, 121);
    getCurrentTime(local, tcr -> abbrev, buf);
    display.print(buf);
    //display.setFont(&FreeSans9pt7b);
    //display.setCursor(244, 121);
    //display.print("8/8/93");
  } while (display.nextPage());
}

void displayInfo(String text1, String text2, String text3) {
  display.setFont(&FreeSans9pt7b);
  display.fillScreen(GxEPD_WHITE);
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(15, 15);
    display.print(text1);
    display.setCursor(15, 35);
    display.print(text2);
    display.setCursor(15, 55);
    display.print(text3);
  } while (display.nextPage());
}
void deepSleep() {
  WiFi.disconnect();
  
  esp_sleep_enable_timer_wakeup(900000000); // check every 15 minutes
  Serial.println("SETUP: GOODNIGHT!");
  esp_deep_sleep_start();
}
