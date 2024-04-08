/* *******************************************  LIBRARIES  ******************************************* */
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Ticker.h>
#include <Widget.h>
#include <DS3231.h>
#include <DS1621.h>
#include <TSL2591.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_BME680.h>
#include <Servo.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const String API_KEY = "&appid=0104d96221f41c7697ef725451212428";
const String URL = "http://api.openweathermap.org/data/2.5/weather?q=";
const String UNITS = "&units=metric";

const String LANG = "&lang=fr";

String zeCity = "Paris,FR";

float zeTemperature = 0;

float zePressure = 0;

float zeHumidity = 0;
void action();
void function_connect(WiFiEvent_t event, WiFiEventInfo_t info);
void function_disconnect(WiFiEvent_t event, WiFiEventInfo_t info);
void function_got_ip(WiFiEvent_t event, WiFiEventInfo_t info);
String httpGetRequest(const char *url);

JsonDocument doc;

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C

/* ******************************************* CONSTANTS  & variables ******************************************* */

const char *PARAM_LED = "led";
AsyncWebServer server(80);

uint32_t startMillis; // some global variables available anywhere in the program
uint32_t currentMillis;
const uint16_t SCREEN_period = 2000; // the value is a number of milliseconds
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define A_OLED 0x3C
#define OLED_RESET -1
const uint8_t A_DS3231 = 0x68;  // adresse de DS3231
const uint8_t A_DS1621 = 0x49;  // adresse de DS1621
const uint8_t A_TSL2591 = 0x29; // adresse de TSL2591

#define pressbutton 26
#define ledpin_oled 25
#define pot_analog_pin 34
#define ledpin_wifi 13
#define ledpin_action 5
bool screen_state = 0;
uint16_t pot_value = 0;
int TEMPERATURE = 0;
int LUX = 0;
int PRESSURE = 0;
int HUMIDITY = 0;
String VILLE;

const char *pwd = "3fanq5w4pb";
const char *ssid = "Linksys01370";

Servo myservo; // create servo object to control a servo
int posservo = 0;
/* ************************************* FONCTION PROTOTYPES ************************************** */
void action();
void IRAM_ATTR toggleLED();
bool initSPIFFS();
void function_connect(WiFiEvent_t event, WiFiEventInfo_t info);
void function_disconnect(WiFiEvent_t event, WiFiEventInfo_t info);
void function_got_ip(WiFiEvent_t event, WiFiEventInfo_t info);
void setRoutes();
String api();
String httpGetRequest(const char *url);

/* ******************************************* OTHER ******************************************** */
DS3231 cap_ds3231;
DS1621 cap_ds1621;
TSL2591 cap_tsl2591;

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const uint32_t PERIOD = 500;
// obj
Ticker mticker(action, PERIOD, 0, MILLIS);
GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
AnaClock anaclock(&canvas, 32, 32, 28);
Gauge gauge(&canvas, 64, 64, 60);
Gauge_LUX gauge_lux(&canvas);
Gauge_hum gauge_hum(&canvas);

/* ******************************************* SETUP ******************************************* */

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  pinMode(ledpin_wifi, OUTPUT);

  myservo.attach(14);
  myservo.write(0);
  delay(1000);

  bool res_spiffs = initSPIFFS();

  WiFi.mode(WIFI_STA);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, A_OLED))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  else
  {
    mticker.start();
  }
  bool res = cap_ds3231.begin(A_DS3231);
  cap_ds1621.begin(A_DS1621);
  // cap_ds3231.setDate(1, 3, 3);
  // cap_ds3231.setTime(17, 20, 59);

  cap_tsl2591.begin(A_TSL2591);
  // cap_tsl2591.getId();
  cap_tsl2591.config(cap_tsl2591.GAIN_MED, cap_tsl2591.ATIME_300);

  if (!bme.begin())
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;

    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
  }

  //
  pinMode(ledpin_oled, OUTPUT);
  pinMode(ledpin_action, OUTPUT);
  digitalWrite(ledpin_oled, 0);
  digitalWrite(ledpin_action, 0);
  pinMode(pressbutton, INPUT);

  WiFi.onEvent(function_connect, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(function_disconnect, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.onEvent(function_got_ip, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  setRoutes();

  WiFi.begin(ssid, pwd);

  delay(2000);
  attachInterrupt(pressbutton, toggleLED, FALLING);
}

/* ******************************************* LOOP ******************************************* */

void loop()
{
  mticker.update();
}

void IRAM_ATTR toggleLED()
{
  screen_state = !screen_state;
  digitalWrite(ledpin_oled, screen_state);
}

/* *********************************** FONCTION DEFINITIONS *********************************** */

void action()
{

  if (!bme.performReading())
  {
    Serial.println("Failed to perform reading :(");
    return;
  }

  PRESSURE = (int)bme.pressure / 100.0;
  // Serial.print("Pressure = ");
  // Serial.print(PRESSURE);
  // Serial.println(" hPa");

  HUMIDITY = (int)bme.humidity;
  // Serial.print("Humidity = ");
  // Serial.print(HUMIDITY);
  // Serial.println(" %");
  // Serial.println();

  //

  TEMPERATURE = (int)cap_ds1621.getTemperature();
  LUX = (int)cap_tsl2591.calculateLux();
  // Serial.println(TEMPERATURE);
  // Serial.println(LUX);
  // uint8_t second = cap_ds3231.getSecond();
  // uint8_t heure = cap_ds3231.getHour();
  // uint8_t minute = cap_ds3231.getMinute();
  // String Day = cap_ds3231.getStringDay();
  // String Month = cap_ds3231.getStringMonth();
  // uint8_t DATE = cap_ds3231.getNumber();
  // Serial.printf(" HH : MM : SS : JOUR : MOIS : DATE = %d : %d : %d : %s : %s : %d  \n \r", heure, minute, second, Day, Month, DATE);
  static uint8_t condition_screen = 0;
  pot_value = analogRead(pot_analog_pin);

  switch (condition_screen)
  {
  case 0:
    anaclock.draw();
    break;
  case 1:
    gauge.draw();
    break;
  case 2:
    gauge_lux.draw();
    break;
  case 3:
    gauge_hum.draw();
    break;
  default:
    condition_screen = 0;
    break;
  }

  if (screen_state)
  {
    currentMillis = millis(); // Get the current time
    if (currentMillis - startMillis >= SCREEN_period)
    {
      condition_screen++;
      if (condition_screen >= 4)
      {
        condition_screen = 0;
      }
      startMillis = currentMillis; // Reset the timer
    }
  }
  else
  {
    // Serial.println(pot_value);
    if (pot_value <= 1023)
    {
      condition_screen = 0;
    }
    if (pot_value <= 2046 & pot_value > 1023)
    {
      condition_screen = 1;
    }
    if (pot_value <= 3069 & pot_value > 2046)
    {
      condition_screen = 2;
    }
    if (pot_value > 3069)
    {
      condition_screen = 3;
    }
  }

  oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0);
  oled.display();
}

bool initSPIFFS()
{
  bool res = false;
  res = SPIFFS.begin();
  if (res == false)
  {
    Serial.println("Error Init SPIFFS");
  }
  else
  {
    Serial.println("Init SPIFFS OK");

    // read data content
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    //
    while (file)
    {
      Serial.print("File : ");
      Serial.println(file.name());
      //
      file.close();
      file = root.openNextFile();
    }
  }
  return res;
}

void function_connect(WiFiEvent_t event, WiFiEventInfo_t info)
{

  Serial.println("Wifi est co");
  digitalWrite(ledpin_wifi, HIGH);
}

void function_disconnect(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Wifi est deco");
  WiFi.begin(ssid, pwd);
  digitalWrite(ledpin_wifi, LOW);
}

void function_got_ip(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println(WiFi.localIP());
  Serial.println("Wifi got IP");

  server.begin();
  Serial.println("Server started on port 80 lmao");
}

void setRoutes()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/w3.css", "text/css"); });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "text/javascript"); });

  server.on("/justgage.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/justgage.js", "text/javascript"); });

  server.on("/raphael.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/raphael.min.js", "text/javascript"); });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) { /* Serial.println("route_temp"); */
               request->send(200, "text/plain", String(TEMPERATURE)); });

  server.on("/Lux", HTTP_GET, [](AsyncWebServerRequest *request) { /* Serial.println("route_temp"); */
               request->send(200, "text/plain", String(LUX)); });

  server.on("/Pressure", HTTP_GET, [](AsyncWebServerRequest *request) { /* Serial.println("route_temp"); */
               request->send(200, "text/plain", String(PRESSURE)); });

  server.on("/Humidity", HTTP_GET, [](AsyncWebServerRequest *request) { /* Serial.println("route_temp"); */
               request->send(200, "text/plain", String(HUMIDITY)); });
  //
  /* Serial.println("route_temp"); */
  server.on("/action", HTTP_GET, [](AsyncWebServerRequest *request)
            {
            Serial.println("ACTION");
                  if (request->hasParam("state"))
                  {
                    String res = request->getParam("state")->value();
                    if (res == "1")
                    {
                      digitalWrite(ledpin_action, HIGH);
                    }
                    else
                    {
                      digitalWrite(ledpin_action, LOW);
                    }
                  }
                  request->send(200); });
  //
  /* Serial.println("route_temp"); */
  server.on("/servo", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
           if (request->hasParam("servo"))
           {
             String res = request->getParam("servo")->value();
             myservo.write(res.toInt());
           }
           request->send(200); });

  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              String theDateValue = "";
              String theHourValue = "";
              String theDayValue = "";
              int params = request->params();
              Serial.print("params = ");
              Serial.println(params);
              //
              for (int i = 0; i < params; i++)
              {
                AsyncWebParameter *p = request->getParam(i);
                if (p->isPost())
                {
                  if (p->name() == "theday")
                  {
                    theDayValue = p->value();
                  }
                  //
                  if (p->name() == "thedate")
                  {
                    theDateValue = p->value();
                  }
                  //
                  if (p->name() == "thetime")
                  {
                    theHourValue = p->value();
                  }
                }
              }

              cap_ds3231.setDate(theDayValue.toInt(), (theDateValue.substring(8, 10)).toInt(), (theDateValue.substring(5, 7)).toInt());
              cap_ds3231.setTime((theHourValue.substring(0, 2)).toInt(), (theHourValue.substring(3, 5)).toInt(), 0);
              Serial.print(" Date = ");
              Serial.println(theDateValue);
              Serial.print(" houeuirs= ");
              Serial.println(theHourValue);
              Serial.print(" Jours = ");
              Serial.println(theDayValue);

              //
              request->redirect("/");
              // mise a jour de la date et heure
              //
            });

  server.on("/weather", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (request->hasParam("city"))
              {
                 VILLE = request->getParam("city")->value();
                Serial.print("=");
                Serial.println(VILLE);
              }
              request->send(200, "text/plain",api() ); });
}

String api()
{
  String jsonBuffer = "";
  String server = "" + URL + VILLE + API_KEY + UNITS + LANG;
  Serial.println(server);
  jsonBuffer = httpGetRequest(server.c_str());
  Serial.println(jsonBuffer);
  // DeserializationError error = deserializeJson(doc, jsonBuffer);
  // if (error)
  // {
  //   Serial.print("deserializeJson() failed: ");
  //   Serial.println(error.c_str());
  //   return 0;
  // }
  return jsonBuffer;

  // JsonObject main = doc["main"];
  // float main_temp = 0["temp"]; // 12.07
  // Serial.printf("la temperature api est %f", main_temp);
  // return main_temp;
}

String httpGetRequest(const char *url)
{
  WiFiClient client;
  HTTPClient http;
  String payload = "";
  //
  http.begin(client, url);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP code get request: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  http.end();
  return payload;
}
