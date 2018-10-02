#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "";
const char *password = "";
ESP8266WebServer server(80);

#define NeoPIN D4
#define NUM_LEDS 8
int brightness = 150;
String stateLed[NUM_LEDS] = {};
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NeoPIN, NEO_RGB + NEO_KHZ800);

const int led = 13;


void setup(void) {
  Serial.begin (115200);

  strip.setBrightness(brightness);
  strip.begin();
  strip.show(); 
  delay(50);
  
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("alexring")) {
    Serial.println("MDNS responder started");
  }

  // what to do with requests
  server.on("/", handleRoot);
  server.on("/bright", brightNeo);
  server.on("/snake", snakeNeo);
  server.on("/switch", switchNeo);
  server.on("/blink", blinkNeo);
  server.onNotFound(handleNotFound);
  server.begin();
 
  delay(50);
  Serial.println("HTTP server started");

  snakeNeo();
  snakeNeo();
}

void loop(void) {
  server.handleClient();
}


void handleRoot() {
  Serial.println("Client connected");
  responseReq();
}

void handleNotFound(void) {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void brightNeo(void) {
  String bright = server.arg("bright");

  responseReq();

  strip.setBrightness(bright.toInt());
  strip.show();
}

void snakeNeo() {
  String color = server.arg("color");
  int tim = server.arg("time").toInt();
  int i = 0;
  unsigned long currentMillis = 0;
  unsigned long previousMillis = 0;

  color = (!color || color == "") ? "#ffffff" : color;
  tim = (!tim || tim < 0) ? 100 : tim;
  responseReq();
  
  while (i <= NUM_LEDS) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= tim) {
      if (i > 0) {
        strip.setPixelColor(i - 1, strip.Color(0, 0, 0));
        strip.show();
      }
      previousMillis = currentMillis;
      strip.setPixelColor(i, strip.Color(getColor(color)[1], getColor(color)[0], getColor(color)[2]));
      strip.show();
      yield();

      i++;
    }
  }

  switchNeo();
}

void switchNeo(void) {
  String color = server.arg("color");
  int id = server.arg("id").toInt();
  String savedColor = "";

  Serial.println(server.arg("id"));

  responseReq();

  if (id < NUM_LEDS && id > -1) {
    stateLed[id] = color;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
      if (stateLed[i])
      {
        savedColor = stateLed[i];
      }
      else {
        savedColor = "#000000";
      }
      strip.setPixelColor(i, strip.Color(getColor(savedColor)[1], getColor(savedColor)[0], getColor(savedColor)[2]));
      strip.show();
    }
}

void blinkNeo(void) {
  int tim = server.arg("time").toInt();
  int inter = server.arg("interval").toInt();
  byte tmpBright = brightness;
  int i = 0;
  float ex = 0.8;

  unsigned long currentMillis = 0;
  unsigned long previousMillis = 0;

  tim = (!tim || tim < 0) ? 0 : tim;
  responseReq();

  while (i <= tim) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= inter) {
      if (tmpBright < 10) {
        ex = 1.2;
        yield();
      }
      else if (tmpBright >= 150) {
        ex = 0.8;
        yield();
      }
      previousMillis = currentMillis;
      tmpBright = tmpBright * ex;

      strip.setBrightness(tmpBright);
      strip.show();

      i++;
    }
  }
  strip.setBrightness(brightness);
  strip.show();
}

byte* getColor(String value) {
  int number = (int) strtol( &value[1], NULL, 16);
  static byte col_arr[3];

  col_arr[0] = number >> 16;
  col_arr[1] = number >> 8 & 0xFF;
  col_arr[2] = number & 0xFF;

  return (col_arr);
}

void responseReq(void) {
  char temp[5000];

  snprintf(temp, 5000,
  "<!DOCTYPE html>\n<html>\n\
  <head>\n\
    <title>NeoPixel</title>\n\
    <meta name=\"viewport\" content=\"width=device-width, height=device-height, initial-scale=1.0, user-scalable=0, minimum-scale=1.0, maximum-scale=1.0\" />\n\
  </head>\n\
  <body>\n\
  </body>\
  </html>\n"
  );

  server.send(200, "text/html", temp);
}





