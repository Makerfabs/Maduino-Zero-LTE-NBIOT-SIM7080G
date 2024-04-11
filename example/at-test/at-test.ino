#include <stdio.h>
#include <string.h>
#include <SPI.h>
#include <SD.h>

#define DEBUG true
#define LTE_RESET_PIN -1
#define LTE_PWRKEY_PIN 5

// set up variables using the SD utility library functions:
// Sd2Card card;
// SdVolume volume;
// SdFile root;

// change this to match your SD shield or module;
// Maduino zero 4G LTE: pin 4
const int PIN_SD_SELECT = 4;

#define UART_BAUD 115200

#define MODEM_RXD 0
#define MODEM_TXD 1

void setup()
{

    SerialUSB.begin(115200);
    Serial1.begin(115200);
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

    sendData("AT", 1000, DEBUG);
    sendData("AT+CPIN?", 1000, DEBUG);
    sendData("AT+CCID", 1000, DEBUG);
    // sendData("AT+CSQ", 1000, DEBUG);
    // sendData("AT+COPS?", 1000, DEBUG);
    // sendData("AT+CGATT?", 1000, DEBUG);
    // sendData("AT+CNACT=0,1", 1000, DEBUG);
    // sendData("AT+CNACT?", 1000, DEBUG);

    // sendData("AT+SHCONF=\"URL\",\"http://api.thingspeak.com\"", 1000, DEBUG);
    // sendData("AT+SHCONF=\"BODYLEN\",1024", 1000, DEBUG);
    // sendData("AT+SHCONF=\"HEADERLEN\",350", 1000, DEBUG);
    // sendData("AT+SHCONN", 1000, DEBUG);
    // delay(5000);
    // sendData("AT+SHSTATE?", 1000, DEBUG);
    // sendData("AT+SHCHEAD", 1000, DEBUG);
    // sendData("AT+SHAHEAD=\"User-Agent\",\"curl/7.47.0\"", 1000, DEBUG);
    // sendData("AT+SHAHEAD=\"Cache-control\",\"no-cache\"", 1000, DEBUG);
    // sendData("AT+SHAHEAD=\"Connection\",\"keep-alive\"", 1000, DEBUG);
    // sendData("AT+SHAHEAD=\"Accept\",\"*/*\"", 1000, DEBUG);
    // sendData("AT+SHREQ=\"/update?api_key=5WOQ0JZURMWBTRF4&field1=123\",1", 1000, DEBUG);
    // sendData("AT+SHREAD=0,386", 1000, DEBUG);
    // sendData("AT+SHDISC", 1000, DEBUG);
    // sendData("AT+CNACT=0,0", 1000, DEBUG);
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
