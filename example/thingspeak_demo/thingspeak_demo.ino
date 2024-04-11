#include <stdio.h>
#include <string.h>
#include <SPI.h>
#include <SD.h>
#include "DHT.h"

#define DEBUG true
#define LTE_RESET_PIN -1
#define LTE_PWRKEY_PIN 5

#define DHTPIN 7      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

char msg[80] = "";

void setup()
{

    SerialUSB.begin(115200);
    Serial1.begin(115200);
    dht.begin();
    delay(1000);

    pinMode(LTE_RESET_PIN, OUTPUT);
    digitalWrite(LTE_RESET_PIN, LOW);
    pinMode(LTE_PWRKEY_PIN, OUTPUT);
    digitalWrite(LTE_RESET_PIN, LOW);
    delay(100);
    digitalWrite(LTE_PWRKEY_PIN, HIGH);
    delay(3000);
    digitalWrite(LTE_PWRKEY_PIN, LOW);

    SerialUSB.println("Maduino Zero LTE CAT-M1 CAT1 NB1/NB2 Test Start!");
    SerialUSB.println("Wait a few minutes for LTE star");
    delay(3000);

    read_dht(msg);

    // while(1)
    // {
    //     read_dht();
    //     delay(2000);
    // }

    sendData("AT", 1000, DEBUG);
    sendData("AT+CPIN?", 1000, DEBUG);
    sendData("AT+CCID", 1000, DEBUG);
    sendData("AT+CSQ", 1000, DEBUG);
    sendData("AT+COPS?", 1000, DEBUG);
    sendData("AT+CGATT?", 1000, DEBUG);
    sendData("AT+CNACT=0,1", 1000, DEBUG);
    sendData("AT+CNACT?", 1000, DEBUG);

    sendData("AT+SHCONF=\"URL\",\"http://api.thingspeak.com\"", 1000, DEBUG);
    sendData("AT+SHCONF=\"BODYLEN\",1024", 1000, DEBUG);
    sendData("AT+SHCONF=\"HEADERLEN\",350", 1000, DEBUG);
    sendData("AT+SHCONN", 5000, DEBUG);
    // delay(5000);
    sendData("AT+SHSTATE?", 1000, DEBUG);
    sendData("AT+SHCHEAD", 1000, DEBUG);
    sendData("AT+SHAHEAD=\"User-Agent\",\"curl/7.47.0\"", 1000, DEBUG);
    sendData("AT+SHAHEAD=\"Cache-control\",\"no-cache\"", 1000, DEBUG);
    sendData("AT+SHAHEAD=\"Connection\",\"keep-alive\"", 1000, DEBUG);
    sendData("AT+SHAHEAD=\"Accept\",\"*/*\"", 1000, DEBUG);
    // sendData("AT+SHREQ=\"/update?api_key=5WOQ0JZURMWBTRF4&field1=123\",1", 1000, DEBUG);
    read_dht(msg);
    sendData(msg, 1000, DEBUG);
    sendData("AT+SHREAD=0,386", 1000, DEBUG);
    sendData("AT+SHDISC", 1000, DEBUG);
    sendData("AT+CNACT=0,0", 1000, DEBUG);
}

void loop()
{
    while (Serial1.available() > 0)
    {
        SerialUSB.write(Serial1.read());
        yield();
    }
    while (SerialUSB.available() > 0)
    {
        Serial1.write(SerialUSB.read());
        yield();
    }
}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    Serial1.println(command);
    long int time = millis();
    while ((time + timeout) > millis())
    {
        while (Serial1.available())
        {
            char c = Serial1.read();
            response += c;
        }
    }
    if (debug)
    {
        SerialUSB.print(response);
    }
    return response;
}

void read_dht(char * msg)
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    SerialUSB.println(h);
    SerialUSB.println(t);

    
    sprintf(msg, "AT+SHREQ=\"/update?api_key=5WOQ0JZURMWBTRF4&field1=%.1f&field2=%.1f\",1", h, t);
    SerialUSB.println(msg);
}