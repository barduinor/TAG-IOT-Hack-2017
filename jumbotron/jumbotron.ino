#include <WiFi.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Adafruit_NeoPixel.h>

#include <PubSubClient.h>

// TFT Display Pins
#define TFT_CS    19
#define TFT_DC    22
#define TFT_MOSI  23
#define TFT_CLK   26
#define TFT_RST   21
#define TFT_MISO  25

// NeoPixel Values
#define PIXELPIN   5
#define NUMPIXELS  5
#define pixlux    20  //saturation level for NeoPixels colors



const char* ssid = "ATC Guest";
const char* password = "";
const char* mqttServer = "m10.cloudmqtt.com";
const int mqttPort = 19839;
const char* mqttUser = "tag";
const char* mqttPassword = "atc";

WiFiClient espClient;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);
PubSubClient client(espClient);


void setup() {

  tft.begin();
  tft.setRotation(3); // rotate 3*(pi/2)
  pixels.begin();
  pixels.show();

  Serial.begin(115200);


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");


  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);

    }
  }

  client.subscribe("/tag/led/#");


  //pixels.show();

  pixelInit();

}

void loop() {
  client.loop();

}


void callback(char* topic, byte* payload, unsigned int length) {

  //if (strcmp(topic,"pir1Status")==0)

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");


  payload[length] = '\0';

  //  String stopic = String(topic);
  //  String value = String((char*)payload);
  //
  //  Serial.println(" Topic:"+stopic);
  //  Serial.println(" Value:"+value);



  tugOfWar(payload, length);

}

void tugOfWar(byte* payload , unsigned int length)
{
  int l0r;
  int l0g;
  int l0b;

  int l1r;
  int l1g;
  int l1b;

  int l2r;
  int l2g;
  int l2b;

  int l3r;
  int l3g;
  int l3b;

  int l4r;
  int l4g;
  int l4b;

  int del = 50;

  char * pch;

  //pch = strtok ((char*)payload,",");
  sscanf((char*)payload, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &l0r, &l0g, &l0b, &l1r, &l1g, &l1b, &l2r, &l2g, &l2b, &l3r, &l3g, &l3b, &l4r, &l4g, &l4b);

  Serial.println("LED 2");
  Serial.println(l2r);
  Serial.println(l2g);
  Serial.println(l2b);


  pixels.setPixelColor(0, pixels.Color(l0r, l0g, l0b));
  pixels.show();
  delay(del);

  pixels.setPixelColor(4, pixels.Color(l4r, l4g, l4b));
  pixels.show();
  delay(del);

  pixels.setPixelColor(1, pixels.Color(l1r, l1g, l1b));
  pixels.show();
  delay(del);

  pixels.setPixelColor(3, pixels.Color(l3r, l3g, l3b));
  pixels.show();
  delay(del);

  pixels.setPixelColor(2, pixels.Color(l2r, l2g, l2b));
  delay(del);
  pixels.show();
  delay(del);
}

void RainbowLEDcycle(int cycles)
{
  int i = 0;
  while (cycles)
  {
    pixels.setPixelColor(i, pixels.Color(pixlux, 0, 0));
    i = (i == 4) ? 0 : (i + 1);
    pixels.setPixelColor(i, pixels.Color(pixlux, pixlux, 0));
    i = (i == 4) ? 0 : (i + 1);
    pixels.setPixelColor(i, pixels.Color(0, pixlux, 0));
    i = (i == 4) ? 0 : (i + 1);
    pixels.setPixelColor(i, pixels.Color(0, 0, pixlux));
    i = (i == 4) ? 0 : (i + 1);
    pixels.setPixelColor(i, pixels.Color(pixlux, 0, pixlux));
    i = (i == 4) ? 0 : (i + 1);
    i = (i == 4) ? 0 : (i + 1);
    pixels.show();
    delay(150);
    cycles--;
  }
  pixels.setPixelColor(0, pixels.Color(0,0,0));
  pixels.show();
    delay(150);
  pixels.setPixelColor(1, pixels.Color(0,0,0));
  pixels.show();
    delay(150);
  pixels.setPixelColor(2, pixels.Color(0,0,0));
  pixels.show();
    delay(150);
  pixels.setPixelColor(3, pixels.Color(0,0,0));
  pixels.show();
    delay(150);
  pixels.setPixelColor(4, pixels.Color(0,0,0));
  pixels.show();
    delay(150);
}

void pixelInit() {
  RainbowLEDcycle(20);
}
