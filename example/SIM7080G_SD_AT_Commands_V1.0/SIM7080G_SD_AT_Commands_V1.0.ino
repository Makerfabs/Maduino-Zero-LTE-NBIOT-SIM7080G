#include<stdio.h>
#include<string.h>
#include<string.h>

#define DEBUG true

//include the SD library:
#include <SPI.h>
#include <SD.h>

//www.makerfabs.com
//Test SD card
//Send AT Command via serial tool
//version:v1.1
//author:Charlin
//date:2023/10/08

#define LTE_RESET_PIN -1

#define LTE_PWRKEY_PIN 5

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// change this to match your SD shield or module;
// Maduino zero 4G LTE: pin 4
const int PIN_SD_SELECT = 4;

#define UART_BAUD           115200

#define MODEM_RXD          0
#define MODEM_TXD          1


unsigned long currentTime;

void setup()
{

  SerialUSB.begin(115200);
  //while (!SerialUSB)
  {
    ; // wait for Arduino serial Monitor port to connect
  }
  Serial1.begin(115200);
  delay(1000);
  //Serial1.begin(UART_BAUD, SERIAL_8N1, MODEM_RXD, MODEM_TXD);

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
  sendData("AT+CSQ", 1000, DEBUG);
  sendData("AT+CCID", 1000, DEBUG);
  sendData("AT+SIMCOMATI", 1000, DEBUG);
  sendData("AT+COPS?", 1000, DEBUG); 
  sendData("AT+GMR", 1000, DEBUG); 

  sendData("AT+CPSI?", 1000, DEBUG);
  
  #if 1
  //Get GNSS location  //GPS test
  sendData("AT+CGNSPWR=1", 12000, DEBUG);// turn the GPS on //must wait about 10s for GPS on
 

  sendData("AT+CGNSINF", 1000, DEBUG);//get GPS infomation
  #endif

  #if 1
  SerialUSB.print("\nInitializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega, 14 on Leonardo) must be left as an output
  // or the SD library functions will not work.


  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, PIN_SD_SELECT)) {
    SerialUSB.println("initialization failed. Things to check:");
    SerialUSB.println("* is a card is inserted?");
    SerialUSB.println("* Is your wiring correct?");
    SerialUSB.println("* did you change the chipSelect pin to match your shield or module?");
    //return;
  } else {
    SerialUSB.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  SerialUSB.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      SerialUSB.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      SerialUSB.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      SerialUSB.println("SDHC");
      break;
    default:
      SerialUSB.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    SerialUSB.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    //return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  SerialUSB.print("\nVolume type is FAT");
  SerialUSB.println(volume.fatType(), DEC);
  SerialUSB.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  SerialUSB.print("Volume size (bytes): ");
  SerialUSB.println(volumesize);
  SerialUSB.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  SerialUSB.println(volumesize);
  SerialUSB.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  SerialUSB.println(volumesize);


  SerialUSB.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  //root.ls(LS_R | LS_DATE | LS_SIZE);
  #endif

#if 1
//  delay(2000);
//  delay(2000);
  delay(1000);
  sendData("AT+CGNAPN", 1000, DEBUG);
  sendData("AT+CNMP=38", 1000, DEBUG);
  sendData("AT+CMNB=2", 1000, DEBUG);  
  delay(1000);
  sendData("AT", 1000, DEBUG);
  delay(1000);
  sendData("AT+SIMCOMATI", 1000, DEBUG);
  sendData("AT+CNACT=0,1", 1000, DEBUG);  
  
  delay(1000);
  sendData("AT+CCID", 1000, DEBUG);
  sendData("AT+CSQ", 1000, DEBUG);
  sendData("AT+CPSI?", 1000, DEBUG);
  //sendData("AT+CNUM", 1000, DEBUG);
  sendData("AT+COPS?", 1000, DEBUG);    
  sendData("AT+CNMP?", 1000, DEBUG);

  sendData("AT+CNACT?", 1000, DEBUG);
  
  //Http test

  sendData("AT+SHCONF=\"URL\",\"http://httpbin.org\"", 1000, DEBUG);
  sendData("AT+SHCONF=\"BODYLEN\",1024", 1000, DEBUG);
  sendData("AT+SHCONF=\"HEADERLEN\",350", 1000, DEBUG);
  sendData("AT+SHCONN", 1000, DEBUG);
  delay(10000);
  sendData("AT+SHSTATE?", 1000, DEBUG);
  sendData("AT+SHCHEAD", 1000, DEBUG);
  sendData("AT+SHAHEAD=\"User-Agent\",\"curl/7.47.0\"", 1000, DEBUG);
  sendData("AT+SHAHEAD=\"Cache-control\",\"no-cache\"", 1000, DEBUG);
  sendData("AT+SHAHEAD=\"Connection\",\"keep-alive\"", 1000, DEBUG);
  sendData("AT+SHAHEAD=\"Accept\",\"*/*\"", 1000, DEBUG);
  sendData("AT+SHREQ=\"/get?user=jack&password=123\",1", 1000, DEBUG);
  sendData("AT+SHREAD=0,386", 1000, DEBUG);
  sendData("AT+SHDISC", 1000, DEBUG); 
  sendData("AT+CNACT=0,0", 1000, DEBUG);  
#endif

  sendData("AT+CCID", 1000, DEBUG);
 
   currentTime = millis();
}

void loop()
{
  if(millis()-currentTime>50000)
  {
      currentTime = millis();//refresh
      //sendData("AT+CGNSINFO", 1000, DEBUG);//get GPS infomation
  }
  //SerialUSB.println("echo test!");
  while (Serial1.available() > 0) {
    SerialUSB.write(Serial1.read());
    yield();
  }
  while (SerialUSB.available() > 0) {
    Serial1.write(SerialUSB.read());
    yield();
  }
}


String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  Serial1.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
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
