#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "wpa2_enterprise.h"

char ssid[] = "Hana";
char username[] = "hana03";
char password[] = "hana1234";

void setup() {
    Serial.begin(9600);
    wifi_set_opmode(STATION_MODE);

    struct station_config wifi_config;

    memset(&wifi_config, 0, sizeof(wifi_config));
    strcpy((char *) wifi_config.ssid, ssid);

    wifi_station_set_config(&wifi_config);

    wifi_station_clear_cert_key();
    wifi_station_clear_enterprise_ca_cert();

    wifi_station_set_wpa2_enterprise_auth(1);
    wifi_station_set_enterprise_identity((uint8 *) username, strlen(username));
    wifi_station_set_enterprise_username((uint8 *) username, strlen(username));
    wifi_station_set_enterprise_password((uint8 *) password, strlen(password));

    wifi_station_connect();

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wifi connecting...");
        delay(500);
    }
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    WiFiClient client;
    HTTPClient http;

    if (http.begin(client, "http://141.164.43.236/")) {
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
}

void loop() {
    // put your main code here, to run repeatedly:
    int measurement[2];
    measurement[0] = analogRead(A0);
    measurement[1] = digitalRead(5);
    Serial.println(measurement[0]);
    Serial.println(measurement[1]);

    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http, http2;

        if (http.begin(client,
                       "http://141.164.43.236/measurements/1/?auth_key=p5ssw0rd&level=" + String(measurement[0]))) {
            int httpCode = http.POST("");
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

        if (http.begin(client,
                       "http://141.164.43.236/measurements/2/?auth_key=p5ssw0rd&level=" + String(measurement[1]))) {
            int httpCode = http.POST("");
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
    }

    delay(5000);
}
