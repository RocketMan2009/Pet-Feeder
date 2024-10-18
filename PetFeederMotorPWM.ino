/*โปรแกรมเปิด ปิด DCมอเตอร์ แบบ PWM ด้วย mqtt ผ่าน ESP32 พัฒนาขึ้นเพื่อใช้ในการอบรม นักเรียน นักศึกษา โดย ดร.วรภัทร กอแก้ว จาก UTCC Makerspace */
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h> 

// MQTT broker
const char* mqtt_server = "....................";//มีหลายค่ายที่ให้บริการ broker.hivemq.com,broker.mqtt.cool,test.mosquitto.org
const char* topic = "UTCC/Makerspace/Feeder1";//ตั้ง topic ให้แตกต่างกัน

// Pin to control (e.g., an LED)
const int ledPin = 2;
// Assign output variables to GPIO pins
uint8_t IN1 = 14; //Motor A -
uint8_t IN2 = 12; //Motor A +
uint8_t IN3 = 26; //Motor B -
uint8_t IN4 = 27; //Motor B +

// Initialize WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (message == "ON") {
    digitalWrite(ledPin, HIGH);// แสดงสถานะหลอด led สว่าง
    ledcWrite(1, 0);// Motor A
    ledcWrite(2, 150);// Motor A เดินหน้า สามารถปรับได้ 0-255
    ledcWrite(3, 0);//Motor B
    ledcWrite(4, 150);//Motor B เดินหน้า สามารถปรับได้ 0-255
    //delay(5000);
    //digitalWrite(ledPin, LOW);
  } else if (message == "OFF") {
    digitalWrite(ledPin, LOW);// แสดงสถานะหลอด led ดับ
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, 0);
    ledcWrite(4, 0);
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
  // assign IN1-IN4 pins to channels
  ledcAttachPin(IN1, 1);
  ledcAttachPin(IN2, 2);
  ledcAttachPin(IN3, 3);
  ledcAttachPin(IN4, 4);
  // Initialize channels 
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(1, 1000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 1000, 8);
  ledcSetup(3, 1000, 8);
  ledcSetup(4, 1000, 8);  
  // Set outputs to 0
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);
  pinMode(ledPin, OUTPUT);
    // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.
    // put your setup code here, to run once:
  Serial.begin(115200);
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;
    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    // wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...WiFi)");
    }
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
