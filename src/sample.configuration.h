// Wifi: SSID and password
const char* WIFI_SSID = "your-ssid";
const char* WIFI_PASSWORD = "your-wifi-password";

// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "your_deviceid";
const PROGMEM char* MQTT_SERVER_IP = "192.168.#.#";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "homeassistant";
const PROGMEM char* MQTT_PASSWORD = "####";

// MQTT: topics
const char* MQTT_SWITCH_STATE_TOPIC = "you/device/status";
const char* MQTT_SWITCH_COMMAND_TOPIC = "you/device/switch";

// payloads by default (on/off)
const char* SWITCH_ON = "ON";
const char* SWITCH_OFF = "OFF";

const PROGMEM uint8_t LED_PIN = 2;
const PROGMEM uint8_t RELAY_PIN = 0;
