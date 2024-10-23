//การเปิด ปิดสวิทซ์ ผ่าน WiFi โดยใช้ MQTT เพื่อการใช้การอบรม บ้านสัตว์อัจฉริยะด้วยเทคโนโลยี IoT พัฒนาจาก ดร วรภัทร กอแก้ว UTCC Makerspace//
#include <WiFi.h>
#include <PubSubClient.h> // ต้องลง Library เพิ่มของ PubSubClient

// WiFi credentials
const char* ssid = "...............";//ชื่อของ WiFi ที่ต้องการเชื่อมต่อ
const char* password = ".............";// รหัสของ WiFi

// MQTT broker
const char* mqtt_server = ".............";//มีหลายค่ายที่ให้บริการbroker.hivemq.com,broker.mqtt.cool,test.mosquitto.org
const char* topic = "UTCC/Makerspace/Feeder1";//ตั้ง topic ให้แตกต่างกัน

// Pin to control (e.g., an LED)
const int ledPin = 2;//เลือก pin ที่ต้องการให้คำสั่ง มอเตอร์ทำงาน สามารถดูได้จาก datasheet ของ ESP32

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
    //delay(5000);//ตั้งให้เหมาะสมกับระยะเวลาของมอเตอร์ทำงานให้อาหารสัตว์เลี้ยง
    //digitalWrite(ledPin, LOW);
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
