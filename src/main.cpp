/*
  ESP8266-based MQTT switch
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MQTT_VERSION MQTT_VERSION_3_1_1

#include "configuration.h"

boolean m_switch_state = false; // switch is turned off by default

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// function called to publish the state of the switch (on/off)
void publishSwitchState() {
  if (m_switch_state) {
    client.publish(MQTT_SWITCH_STATE_TOPIC, SWITCH_ON, true);
  } else {
    client.publish(MQTT_SWITCH_STATE_TOPIC, SWITCH_OFF, true);
  }
}

// function called to turn on/off the switch
void setSwitchState() {
  if (m_switch_state) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("INFO: Turn switch on...");
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("INFO: Turn switch off...");
  }
}

// function called when a MQTT message arrived
void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
  // concat the payload into a string
  String payload;
  for (uint8_t i = 0; i < p_length; i++) {
    payload.concat((char)p_payload[i]);
  }

  // handle message topic
  if (String(MQTT_SWITCH_COMMAND_TOPIC).equals(p_topic)) {
    // test if the payload is equal to "ON" or "OFF"
    if (payload.equals(String(SWITCH_ON))) {
      if (m_switch_state != true) {
        m_switch_state = true;
        setSwitchState();
        publishSwitchState();
      }
    } else if (payload.equals(String(SWITCH_OFF))) {
      if (m_switch_state != false) {
        m_switch_state = false;
        setSwitchState();
        publishSwitchState();
      }
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("INFO: Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("INFO: connected");
      // Once connected, publish an announcement...
      publishSwitchState();
      // ... and resubscribe
      client.subscribe(MQTT_SWITCH_COMMAND_TOPIC);
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(client.state());
      Serial.println("DEBUG: try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // init the serial
  Serial.begin(115200);

  // init the pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  analogWriteRange(255);
  setSwitchState();

  // init the WiFi connection
  Serial.println();
  Serial.print("INFO: Connecting to ");
  WiFi.mode(WIFI_STA);
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("INFO: WiFi connected");
  Serial.print("INFO: IP address: ");
  Serial.println(WiFi.localIP());

  // init the MQTT connection
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
