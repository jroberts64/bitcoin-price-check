#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <ArduinoJson.h> 
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

char wifiSSID[] = "6787564891";
char wifiPASS[] = "j1a9d6e6";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 22));

String price;

const unsigned char logo [] PROGMEM = {
  // 'BC_Logo_, 200x42px
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x1f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xc7, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x46, 0x3f, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x06, 0x3f, 0xff, 0xf0, 0x00, 0x0f, 0xe0, 0x00, 0x1f, 0xc0, 
  0x7f, 0x00, 0x01, 0xf0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 
  0x3f, 0xff, 0xf0, 0x00, 0x03, 0xe0, 0x00, 0x0f, 0xc0, 0x7c, 0x00, 0x01, 0xf0, 0x00, 0x07, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x03, 0xe0, 0x00, 
  0x07, 0xc0, 0x7c, 0x00, 0x01, 0xe0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xfe, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x03, 0xe0, 0x00, 0x07, 0xc0, 0x78, 0x00, 0x03, 0xe0, 0x00, 
  0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0e, 0x01, 0xff, 0xf0, 0x18, 0x03, 
  0xe0, 0x60, 0x07, 0xc0, 0xf8, 0x00, 0x03, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfc, 0x0f, 0x81, 0xff, 0xf0, 0x3f, 0x03, 0xc0, 0x7c, 0x0f, 0xc0, 0xf8, 0x0f, 0xff, 
  0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xc0, 0xff, 0xe0, 
  0x3f, 0x03, 0xc0, 0x7c, 0x0f, 0xc0, 0xf8, 0x1f, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xc0, 0xff, 0xe0, 0x3f, 0x03, 0xc0, 0xfc, 0x0f, 0xc0, 0xf8, 
  0x1f, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xc0, 
  0xff, 0xe0, 0x3f, 0x03, 0xc0, 0xf8, 0x0f, 0x80, 0xf8, 0x1f, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0x81, 0xff, 0xe0, 0x3f, 0x07, 0xc0, 0xf8, 0x0f, 
  0x80, 0xf0, 0x1f, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 
  0x00, 0x01, 0xff, 0xe0, 0x7e, 0x07, 0x80, 0xf0, 0x1f, 0x80, 0xf0, 0x1f, 0xff, 0xc0, 0x00, 0x0f, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x03, 0xff, 0xe0, 0x30, 0x07, 0x80, 
  0x00, 0x1f, 0x80, 0xf0, 0x3f, 0xff, 0xc0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x0f, 0x80, 0x00, 0x3f, 0x81, 0xf0, 0x3f, 0xff, 0x80, 
  0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x38, 0x07, 0xff, 0xc0, 0x00, 
  0x0f, 0x00, 0x00, 0xff, 0x81, 0xf0, 0x3f, 0xff, 0x80, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf0, 0x7f, 0x03, 0xff, 0xc0, 0x00, 0x1f, 0x00, 0x01, 0xff, 0x01, 0xe0, 0x3f, 
  0xff, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0x81, 0xff, 
  0xc0, 0x00, 0x3f, 0x00, 0x01, 0xff, 0x01, 0xe0, 0x3f, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0x81, 0xff, 0x80, 0xff, 0xfe, 0x00, 0x01, 0xff, 0x03, 
  0xe0, 0x3f, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x7f, 
  0x81, 0xff, 0x80, 0xff, 0xfe, 0x02, 0x01, 0xff, 0x03, 0xe0, 0x3f, 0xff, 0x03, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0xff, 0x81, 0xff, 0x81, 0xff, 0xfe, 0x03, 0x00, 
  0xfe, 0x03, 0xe0, 0x00, 0x0f, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0x00, 0x3f, 0x01, 0xff, 0x01, 0xff, 0xfc, 0x07, 0x80, 0xfe, 0x07, 0xc0, 0x00, 0x1e, 0x03, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x03, 0xff, 0x01, 0xff, 0xfc, 
  0x07, 0x80, 0xfe, 0x07, 0xc0, 0x00, 0x1e, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xf0, 0x00, 0x03, 0xff, 0x03, 0xff, 0xf8, 0x0f, 0x80, 0x7c, 0x07, 0xc0, 0x00, 0x1c, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x07, 0xfe, 0x03, 
  0xff, 0xf8, 0x0f, 0xc0, 0x7c, 0x0f, 0xc0, 0x00, 0x1c, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf1, 0x80, 0x1f, 0xfe, 0x07, 0xff, 0xf0, 0x1f, 0xc0, 0x78, 0x0f, 0xe0, 
  0x00, 0x3c, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x8f, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 
  0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

void setup() {
  delay(1000);
  Serial.begin(115200); 
  while (!Serial);

  print_wakeup_reason();      
  connectToWifi();
  initDisplay();
    
  // Initialize a NTPClient to get time
    timeClient.begin();
    // Set offset time in seconds to adjust for your timezone, for example:
    // GMT +1 = 3600
    // GMT +8 = 28800
    // GMT -1 = -3600
    // GMT 0 = 0
    timeClient.setTimeOffset(-14400);
    timeClient.update();

  ONprice();
  showPrice();
  WiFi.disconnect();
  
  esp_sleep_enable_timer_wakeup(900000000); // check every 15 minutes
  Serial.println("SETUP: GOODNIGHT!");
  esp_deep_sleep_start();
}


void loop() {
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); 
      Serial.print("touch pin = ");
      Serial.println(esp_sleep_get_touchpad_wakeup_status());
      break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

const char* host = "api.coindesk.com";
const int httpsPort = 443;

void ONprice(){
  WiFiClientSecure client;
  String on_currency = "USD";
  
  if (!client.connect(host, httpsPort)) {
    Serial.print("Unable to connect: IP = " + WiFi.localIP());
    
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

void initDisplay() {
  display.init(115200);
  display.setRotation(1);
  display.setFont(&FreeSansBold18pt7b);
  display.setTextColor(GxEPD_BLACK);

}
void showPrice() {

  formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");

  // Extract date
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");  Serial.println(dayStamp);

  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");  Serial.println(timeStamp);

  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.drawInvertedBitmap(0, 0, logo, 200, 42, GxEPD_BLACK);
    display.setCursor(35, 70);
    Serial.println("Sending price to display " + price);
    display.print("$" + price);
    display.setCursor(35, 99);
    display.setFont(&FreeSansBold12pt7b);
    display.print(dayStamp);
    display.setCursor(35, 119);
    display.print(timeStamp);
  } while (display.nextPage());
}


void waitForWifiConnect() {
  int count = 0;
  
  while (WiFi.status() != WL_CONNECTED) {
    count++;
    Serial.print("\t count = "); Serial.println(count);
    if( ! (count % 4) ) {
     Serial.println("\tretry WiFi Connect.");
     break;
    }
    delay(30000); // wait 30 seconds
  }
}

void connectToWifi() {

  Serial.println("CONNECTTOWIFI");

  while (WiFi.status() != WL_CONNECTED) {

    Serial.println("disconnecting from wifi.");
    WiFi.disconnect(true);
    delay(1000);  
    
    Serial.println("\tconnecting to wifi.");
    WiFi.begin(wifiSSID, wifiPASS);   
    delay(1000); // wait 10 seconds

    waitForWifiConnect();
  }
  Serial.println("\tconnected: IP = " + WiFi.SSID());
}
