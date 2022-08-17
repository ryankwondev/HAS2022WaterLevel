#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

void setup()
{
    Serial.begin(9600);
    WiFi.begin("ssid", "password");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("connecting...");
    }

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    int measurement[2];
    measurement[0] = analogRead(A0);
    measurement[1] = digitalRead(13);
    Serial.println(measurement[0]);
    Serial.println(measurement[1]);

    if (WiFi.status() == WL_CONNECTED)
    {
        WiFiClient client;
        HTTPClient http;

        if (http.begin(client, "http://0.0.0.0/measurements/1/?auth_key=p5ssw0rd&level="+String(measurement[0]))) {
            int httpCode = http.POST();
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

         if (http.begin(client, "http://0.0.0.0/measurements/2/?auth_key=p5ssw0rd&level="+String(measurement[1]))) {
            int httpCode = http.POST();
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
}
