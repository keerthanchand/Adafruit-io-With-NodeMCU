
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "Cortana"
#define WLAN_PASS       "daddy@6T7"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "Arduino_kc"
#define AIO_KEY         "022f64d4381c472ea9a4d14b91b5851f"


WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe relay1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light");
Adafruit_MQTT_Subscribe relay2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/fan");

int lightpin = 5;
int lightstate = LOW;
int fanpin = 4;
int fanstate = LOW;
void MQTT_connect();



void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  mqtt.subscribe(&relay1);
  mqtt.subscribe(&relay2);
pinMode(lightpin, OUTPUT);
pinMode(fanpin, OUTPUT);

}

uint32_t x=0;

void loop() {
  
  MQTT_connect();
 

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &relay1) {
      Serial.print(F("Got: "));
      Serial.println((char *)relay1.lastread);
      lightstate = !lightstate;
        digitalWrite(lightpin, lightstate);

    
    }
    if (subscription == &relay2) {
      Serial.print(F("Got: "));
      Serial.println((char *)relay2.lastread);
      fanstate = !fanstate;
        digitalWrite(fanpin, fanstate);
    }
  }

  // Now we can publish stuff!
  /*Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}


void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 

       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000); 
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
