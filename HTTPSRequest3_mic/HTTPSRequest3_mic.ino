/*
    HTTP over TLS (HTTPS) example sketch

    This example demonstrates how to use
    WiFiClientSecure class to access HTTPS API.
    We fetch and display the status of
    esp8266/Arduino project continuous integration
    build.

    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <SimpleDHT.h>

// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11;


int sensorPin = A0; // select the input pin for the potentiometer
int ledPin = 13; // select the pin for the LED
int sensorValue = 0; // variable to store the value coming from the sensor

int max  = 0;
int counter  =  0;
#define MAXCYLES 2800


#define MIC_ID "A2"

const char* ssid = "ATC Guest";
const char* password = "";

const char* host = "iot-tag17.outsystemscloud.com";
const int httpsPort = 80;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "ea 61 52 b2 fe 0a 40 80 e5 20 8a 30 37 24 a0 ea a6 6c 6a aa";

void setup() {
  Serial.begin(115200);
  pinMode (sensorPin, INPUT);
  pinMode (ledPin, OUTPUT);
}



void loop() {


  
  sensorValue = analogRead (sensorPin);
  /*digitalWrite (ledPin, HIGH);
  
  digitalWrite (ledPin, LOW);*/
  

  if(sensorValue > max )
  {
    max = sensorValue;    
  }

  counter++;
  int value_to_send_to_server;
  if(counter >= MAXCYLES)
  {
    value_to_send_to_server = max;
    Serial.println (max, DEC);
    counter = 0;
    max = 0;
  }
  else
  {
    return;
  }
  
  //delay(1);
  //return;


  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/OutLoudAPI/rest/Event/AddFromArduino?MicId="
               + String(MIC_ID)
               + "&Value="
               + ((int)value_to_send_to_server)
               ;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\0');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");

  Serial.print("disconnecting wifi");
  client.stop();
  Serial.println("... done");

  //delay(60000);
}
