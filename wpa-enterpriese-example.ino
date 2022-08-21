#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "wpa2_enterprise.h"

char ssid[] = "Hana";
char username[] = "******";
char password[] = "********";

void setup()
{
  Serial.begin(9600);

  wifi_set_opmode(STATION_MODE);

  // Configure SSID
  struct station_config wifi_config;

  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char *)wifi_config.ssid, ssid);

  wifi_station_set_config(&wifi_config);

  // DO NOT use authentication using certificates
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();

  // Authenticate using username/password
  wifi_station_set_wpa2_enterprise_auth(1);
  wifi_station_set_enterprise_identity((uint8 *)username, strlen(username));
  wifi_station_set_enterprise_username((uint8 *)username, strlen(username));
  wifi_station_set_enterprise_password((uint8 *)password, strlen(password));

  // Connect
  wifi_station_connect();

  // Wait for connect
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi connecting...");
    delay(500);
  }
  // Print wifi IP addess
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // put your main code here, to run repeatedly:


  WiFiClient client;
  HTTPClient http;


  if (http.begin(client, "http://10.26.177.42:8000/")) {
            int httpCode = http.GET();
            if (httpCode > 0) {
                String payload = http.getString();
                Serial.println(payload);
            } else {
                Serial.println("Error on HTTP request");
            }
            http.end();
        } else {
            Serial.println("Error on HTTP begin");
        }

  delay(1000);
}
