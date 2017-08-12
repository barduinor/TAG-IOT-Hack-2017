#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "iPhoneRB";
const char* password = "atireiopauaogato";
const char* mqttServer = "m10.cloudmqtt.com";
const int mqttPort = 19839;
const char* mqttUser = "tag";
const char* mqttPassword = "atc";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}

void setup() {

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

  client.subscribe("/tag/#");

}

void loop() {
  client.loop();
}
