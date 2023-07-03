#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoBLE.h>

const char ssid[] = ""; // your network SSID (name)
const char pass[] = "";

#define BLE_UUID_HEART_RATE_SERVICE "180D"
#define BLE_UUID_HEART_RATE_MEASUREMENT "2A37"

WiFiServer server(6547);
BLEService heartRateService(BLE_UUID_HEART_RATE_SERVICE); // BLE Heart Rate Service
// add the heart rate characteristic with the flags
BLECharacteristic heartRateChar(BLE_UUID_HEART_RATE_MEASUREMENT, BLENotify | BLERead, 2);

void setupBLE();
void serverLoop();

void blePeripheralConnectHandler(BLEDevice central)
{
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central)
{
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  setupBLE();

  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  server.begin();
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
}

void loop()
{
  // put your main code here, to run repeatedly:
  serverLoop();
  BLE.poll();
}

void serverLoop()
{
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  // Wait until the client sends some data
  // Serial.println("new client");

  // verify that it is a post request
  if (!client.readStringUntil('\r').startsWith("POST"))
  {
    Serial.println("invalid request");
    client.println("HTTP/1.1 405 Method Not Allowed");
    client.println("Allow: POST");
    client.println("Connection: close");
    client.println();
    client.stop();
    return; // we only handle POST requests
  }

  // read only the post body data without the headers (post body is a number)
  String request = client.readString();
  // find the index of the first occurrence of the double newline character
  int index = request.indexOf("\r\n\r\n");

  // extract the substring starting from the index of the double newline character
  String postBody = request.substring(index + 4);
  // rate=90 extract the number from the string
  int rate = postBody.substring(5).toInt();

  // check whether the rate is valid else set it to 0
  if (rate < 0 || rate > 250)
  {
    rate = 0;
  }

  // update the heart rate characteristic with the new rate using the struct
  uint8_t heartRateData[2] = {0x00, uint8_t(rate)};
  int success = heartRateChar.writeValue(heartRateData, 2);

  if (success == 0)
  {
    Serial.println("Failed to update heart rate characteristic");
  }
  Serial.print("rate:");
  Serial.println(rate);

  // Return the response with the body
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close"); // the connection will be closed after completion of the response
  client.println();
  client.println("OK");
  client.stop();
};

void setupBLE()
{

  if (!BLE.begin())
  {
    Serial.println("starting Bluetooth® Low Energy failed!");

    while (1)
      ;
  }
  BLE.setDeviceName("Heart Rate Monitor");
  BLE.setLocalName("Heart Rate Monitor");
  BLE.setAdvertisedService(heartRateService); // add the service UUID

  heartRateService.addCharacteristic(heartRateChar); // add the heart rate characteristic

  BLE.addService(heartRateService); // start advertising

  // TODO: somehow add contact sensor and body sensor location

  uint8_t *initialHeartRate = new uint8_t[2]{0x00, 0x00};
  heartRateChar.writeValue(initialHeartRate, 2); // set initial value for this characteristic
  delete[] initialHeartRate;
  BLE.advertise();
  // print the ble address
  Serial.print("BLE Address: ");
  Serial.println(BLE.address());
}
