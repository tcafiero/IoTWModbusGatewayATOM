
#include <Arduino.h>
#include "M5Atom.h"
#define RX_PIN  (22)
#define TX_PIN  (19)
#include "contract.h"
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
//define your default values here, if there are different values in config.json, they are overwritten.
char site[50] = "Greenhouse A";
char section[50] = "section 1 - Section 2";

// For demonstration, use the LOG statements for output
#include "Logging.h"

// Include the header for the ModbusClient RTU style
#include "ModbusClientRTU.h"

// Create a ModbusRTU client instance
// In my case the RS485 module had auto halfduplex, so no second parameter with the DE/RE pin is required!

ModbusClientRTU MB(Serial2, -1);

enum whichsend {
  next = 0,
  current = 1,
};

void send(int i) {
  static int device = contract.num_servers;
  static int retry = 0;
  Error err;
  if (i == next) {
    retry = 0;
    if ( ++device >= contract.num_servers) device = 0;
  } else {
    if (++retry >= 2) {
      if ( ++device >= contract.num_servers) {
        device = 0;
        retry = 0;
      }
    }
  }
  //Serial.printf("device: %d, server: %d, function: %d, sensoradd: %d, num: %d\n", device, contract.server[device].address, contract.server[device].function, contract.server[device].sensors_address, contract.server[device].num_sensors);
  err = MB.addRequest(device, contract.server[device].address, contract.server[device].function, contract.server[device].sensors_address, contract.server[device].num_sensors);
  if (err != SUCCESS) {
    ModbusError e(err);
    LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
  }
}

void udpPublish(char* message);

// Define an onData handler function to receive the regular responses
// Arguments are Modbus server ID, the function code requested, the message data and length of it,
// plus a user-supplied token to identify the causing request
void handleData(ModbusMessage response, uint32_t device) {
  float value;
  char output[256];
  if (response.getError() == SUCCESS) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["site"] = site;
    json["section"] = section;
    for (int i = 0; i < contract.server[device].num_sensors ; i++) {
      value = (((response[i * 2 + 3] << 8) & 0xff00) + (response[i * 2 + 4] & 0xFF)) * contract.server[device].sensor[i].factor;
      //Serial.printf("%s=%f\n", contract.server[device].sensor[i].name, value);
      json[contract.server[device].sensor[i].name] = value;
    }
    json.printTo(output);
    Serial.println(output);
    udpPublish(output);
    send(next);
  } else send(current);
}



#include "version.h"

#include <WiFi.h>
#include <WiFiUdp.h>
IPAddress broadcastIP(255, 255, 255, 255);
//IPAddress broadcastIP(192, 168, 0, 107);
const unsigned int receiverPort = 8888;
const unsigned int localPort = 8889;

#include <DNSServer.h>
#include <PubSubClient.h>

#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <esp32fota.h>
esp32FOTA esp32FOTA("esp32-fota-http", FIRMWARE_VERSION);

#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include "SPIFFS.h"



//The udp library class
WiFiUDP udp;

WiFiManager wifiManager;
// The extra parameters to be configured (can be either global or just in the setup)
// After connecting, parameter.getValue() will get you the configured value
// id/name placeholder/prompt default length
WiFiManagerParameter custom_site("site", "site", site, 50);
WiFiManagerParameter custom_section("section", "section", section, 50);

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}


void manageConfigFile() {

  //read configuration from FS json
  Serial.println("mounting FS...");
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    //save the custom parameters to FS
    if (shouldSaveConfig) {
      shouldSaveConfig = false;
      Serial.println("saving config");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      json["site"] = custom_site.getValue();
      json["section"] = custom_section.getValue();
      File configFile = SPIFFS.open("/config.json", "w");
      if (!configFile) {
        Serial.println("failed to open config file for writing");
      }
      json.printTo(Serial);
      json.printTo(configFile);
      configFile.close();
    }
    //end save
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          strcpy(site, json["site"]);
          strcpy(section, json["section"]);
        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
}

void udpPublish(char* message)
{
  udp.beginPacket(broadcastIP, receiverPort);
  udp.write((uint8_t*)message, strlen(message));
  udp.endPacket();
}

void setup() {
  //void M5.begin(bool SerialEnable = true, bool I2CEnable = true, bool DisplayEnable = false);
  M5.begin(true, false, false);
  Serial.println();
  Serial.println("Service started.");

  // Set up Serial2 connected to Modbus RTU
  // (Fill in your data here!)
  //Serial2.begin(19200, SERIAL_8N1, GPIO_NUM_17, GPIO_NUM_16);
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
#define BUTTON (39)
  pinMode(BUTTON, INPUT);
  if (digitalRead(BUTTON) == LOW) {
    Serial.println("Format memory.");
    wifiManager.resetSettings();
    SPIFFS.format();
    Serial.println("Restart.");
    ESP.restart();
  }

  // Set up ModbusRTU client.
  MB.onResponseHandler(&handleData);
  // - provide onData handler function
  //MB.onDataHandler(&handleData);
  // - provide onError handler function
  //MB.onErrorHandler(&handleError);
  // Set message timeout to 2000ms
  MB.setTimeout(100);
  // Start ModbusRTU background task
  MB.begin();
  // void begin(int coreID, uint32_t interval)
  //MB.begin(1, 2000);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  //add all your parameters here
  wifiManager.addParameter(&custom_site);
  wifiManager.addParameter(&custom_section);
  wifiManager.setConfigPortalTimeout(180);
  shouldSaveConfig = false;
  wifiManager.autoConnect("IoThingsWare WiFi Manager");
  Serial.println("connected :)");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (i++ > 20) {
      delay(60 * 1000);
      ESP.restart();
    }
    delay(500);
    Serial.print(".");
  }
  //read updated parameters
  manageConfigFile();
  esp32FOTA.checkURL = "http://s3.eu-central-1.amazonaws.com/deployment.iothingsware.com/IoTWModbusGatewayATOM.ino.pico32.json";
  bool updatedNeeded = esp32FOTA.execHTTPcheck();
  if (updatedNeeded)
  {
    esp32FOTA.execOTA();
    ESP.restart();
  }
  udp.begin(localPort);
  send(next);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }
  delay(1);
}
