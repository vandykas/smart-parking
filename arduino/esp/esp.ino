#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

SoftwareSerial xbee(12, 14);  // RX, TX

WiFiClientSecure espClient;
PubSubClient client(espClient);

const char* ssid = "kecap";
const char* password = "kecap123";

const char* mqtt_server = "nfe9a120.ala.asia-southeast1.emqxsl.com";
const int mqtt_port = 8883;

const char* mqtt_username = "mqtt";
const char* mqtt_password = "mqtt123";

char rxBuff[80];
bool started = false;
byte idx = 0;

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  espClient.setInsecure(); // TLS
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // ===== BACA DARI XBEE =====
  while (xbee.available()) {
    char c = xbee.read();

    if (c == '<') {
      started = true;
      idx = 0;
    }
    else if (c == '>' && started) {
      rxBuff[idx] = '\0';
      processMessage(rxBuff);
      started = false;
    }
    else if (started && idx < sizeof(rxBuff) - 1) {
      rxBuff[idx++] = c;
    }
  }
}

// ===== PARSE & PUBLISH =====
void processMessage(char* msg) {
  char* sep = strchr(msg, '|');
  if (!sep) return;

  *sep = '\0';
  char* topic = msg;
  char* payload = sep + 1;

  bool ok = client.publish(topic, payload, true);
  Serial.print("PUBLISH STATUS = ");
  Serial.println(ok ? "SUCCESS" : "FAILED");

  Serial.print("Published → ");
  Serial.print(topic);
  Serial.print(" : ");
  Serial.println(payload);
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "esp8266-" + String(ESP.getChipId());
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("MQTT Connected");
    } else {
      delay(2000);
    }
  }
}
