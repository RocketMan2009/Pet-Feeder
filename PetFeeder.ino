//การเปิด ปิดสวิทซ์ ผ่าน WiFi โดยใช้ MQTT เพื่อการใช้การอบรม บ้านสัตว์อัจฉริยะด้วยเทคโนโลยี IoT พัฒนาจาก UTCC Makerspace//
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "...............";
const char* password = ".............";

// MQTT broker
const char* mqtt_server = ".............";//มีหลายค่ายที่ให้บริการbroker.hivemq.com,broker.mqtt.cool,test.mosquitto.org
const char* topic = "UTCC/Makerspace/Feeder1";//ตั้ง topic ให้แตกต่างกัน

// Pin to control (e.g., an LED)
const int ledPin = 2;

// Initialize WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (message == "ON") {
    digitalWrite(ledPin, HIGH);
    delay(5000);
    digitalWrite(ledPin, LOW);
  } else if (message == "OFF") {
    digitalWrite(ledPin, LOW);
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Client")) {
      client.subscribe(topic);
    } else {
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
