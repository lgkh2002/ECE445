/*
 * pin 1 - not used          |  Micro SD card     |
 * pin 2 - CS (SS)           |                   /
 * pin 3 - DI (MOSI)         |                  |__
 * pin 4 - VDD (3.3V)        |                    |
 * pin 5 - SCK (SCLK)        | 8 7 6 5 4 3 2 1   /
 * pin 6 - VSS (GND)         | ▄ ▄ ▄ ▄ ▄ ▄ ▄ ▄  /
 * pin 7 - DO (MISO)         | ▀ ▀ █ ▀ █ ▀ ▀ ▀ |
 * pin 8 - not used          |_________________|
 *                             ║ ║ ║ ║ ║ ║ ║ ║
 *                     ╔═══════╝ ║ ║ ║ ║ ║ ║ ╚═════════╗
 *                     ║         ║ ║ ║ ║ ║ ╚══════╗    ║
 *                     ║   ╔═════╝ ║ ║ ║ ╚═════╗  ║    ║
 * Connections for     ║   ║   ╔═══╩═║═║═══╗   ║  ║    ║
 * full-sized          ║   ║   ║   ╔═╝ ║   ║   ║  ║    ║
 * SD card             ║   ║   ║   ║   ║   ║   ║  ║    ║
 * Pin name         |  -  DO  VSS SCK VDD VSS DI CS    -  |
 * SD pin number    |  8   7   6   5   4   3   2   1   9 /
 *                  |                                  █/
 *                  |__▍___▊___█___█___█___█___█___█___/
 *
 * Note:  The SPI pins can be manually configured by using `SPI.begin(sck, miso, mosi, cs).`
 *        Alternatively, you can change the CS pin and use the other default settings by using `SD.begin(cs)`.
 *
 * +--------------+---------+-------+----------+----------+----------+
 * | SPI Pin Name | ESP8266 | ESP32 | ESP32-S2 | ESP32-C3 | ESP32-S3 |
 * +==============+=========+=======+==========+==========+==========+
 * | CS (SS)      | GPIO15  | GPIO5 | GPIO5    | GPIO13   | GPIO13   |
 * +--------------+---------+-------+----------+----------+----------+
 * | DI (MOSI)    | GPIO13  | GPIO23| GPIO24   | GPIO14   | GPIO14   |
 * +--------------+---------+-------+----------+----------+----------+
 * | DO (MISO)    | GPIO12  | GPIO19| GPIO25   | GPIO15   | GPIO15   |
 * +--------------+---------+-------+----------+----------+----------+
 * | SCK (SCLK)   | GPIO14  | GPIO18| GPIO19   | GPIO16   | GPIO16   |
 * +--------------+---------+-------+----------+----------+----------+
 *
 * For more info see file README.md in this library or on URL:
 * https://github.com/espressif/arduino-esp32/tree/master/libraries/SD
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define vsck 40
#define vmiso 41
#define vmosi 42
#define vcs 1

#define hsck 10
#define hmiso 11
#define hmosi 12
#define hcs 14

#define button1 8
#define button2 9
#define button3 20


#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define VSPI 0
#define HSPI 1
#endif



void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

String readFile(fs::FS &fs, const char * path){
    String message;
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return message;
    }


    Serial.print("Read from file: ");
    while(file.available()){
        char charRead=file.read();
        message+=charRead;
        //Serial.write(file.read());
    }
    file.close();
    Serial.println(message);
    return message;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %lu ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %lu ms\n", 2048 * 512, end);
    file.close();
}





static const int spiClk = 1000000;
SPIClass * vspi = NULL; 
SPIClass * hspi = NULL;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  while(!Serial) { delay (10); }

  Serial.println("SETUP START");  
  vspi = new SPIClass(VSPI); 
  hspi = new SPIClass(HSPI);

  //inputs
  pinMode(7, INPUT); //BUSY
  pinMode(button1, INPUT); //BUTTON1
  pinMode(button2, INPUT); //BUTTON2
  pinMode(button3, INPUT); //BUTTON3
  
  //outputs


  pinMode(14, OUTPUT); //CS1
  pinMode(47, OUTPUT); //CS2
  pinMode(48, OUTPUT); //RST

  pinMode(17, OUTPUT); //I2S_LRCLK
  pinMode(18, OUTPUT); //I2S_BCLK
  pinMode(19, OUTPUT); //I2S_DATA

  pinMode(8,INPUT); //BUTTON1





  //--------------------------------------------------------------------------------------------------------------


  vspi->begin(vsck, vmiso, vmosi, vcs);
  hspi->begin(hsck, hmiso, hmosi, hcs);

  pinMode(vspi->pinSS(), OUTPUT); //CS0
  pinMode(hspi->pinSS(), OUTPUT); //CS1


  /*Serial.println(sck);
  Serial.println(miso);
  Serial.println(mosi);
  Serial.println(cs);*/

  digitalWrite(vspi->pinSS(), LOW);
  if(!SD.begin(vspi->pinSS(),*vspi)){
      Serial.println("Card Mount Failed");
      return;
  }
digitalWrite(vspi->pinSS(),HIGH);

  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
      return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
      Serial.println("MMC");
  } else if(cardType == CARD_SD){
      Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
      Serial.println("SDHC");
  } else {
      Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
/*
  //START OF SD CARD TESTS
  listDir(SD, "/", 0);
  createDir(SD, "/mydir");
  listDir(SD, "/", 0);
  //removeDir(SD, "/mydir");
  listDir(SD, "/", 2);
  writeFile(SD, "/hello.txt", "Hello ");
  appendFile(SD, "/hello.txt", "World!\n");
  readFile(SD, "/hello.txt");
  //deleteFile(SD, "/foo.txt");
  renameFile(SD, "/hello.txt", "/foo.txt");
  readFile(SD, "/foo.txt");
  testFileIO(SD, "/test.txt");
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
*/
  srand(12);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.printf("TEST");
  //delay(500);
  //chip select high
  //spiCommand(hspi, 0b01010101);   //some garbage command for testing, will be commented out or deleted

  int cardval = 0;             //indexes cards within activecards[], not indexes of cards in parsed text input
  char * s;                //character pointer used to convert strings stored in meteadata text files to and from integers

  String newstring;        //pulls string from metadata file \newstatus.txt which in order contains 0 for a new card and 1 for a seen card. no separation between characters.
  String revstring;        //pulls string from metadata file \revstatus.txt which in order contains the days until review for a card. no separation between characters.
  String learnstring;      //pulls string from metedata file \learnstatus.txt which in order contains a score of 0-9 for the aptitude of a learner for a card. no separation between characters.

  unsigned int cardidx =0 ;        //indexes cards from the output of the text parser
  unsigned int newcardcnt = 0;     //counts how many new cards have been added to activecards[]
  unsigned int totalcards = 10;    //contains the total cards in the set. will be set by the text parser.
  unsigned int activecardcnt = 0;  //counts how many cards have been added to activecards[]

  unsigned int activecards[100];   //contains the indexes of all active cards
  unsigned int finished[100] = {0};//contains info if a card has been finished for the day. 1 is finished, 0 is not finished
  int revbuffer[2];

  int b1state = 0;
  int b2state = 0;
  int b3state = 0;



  //get total card number from parser, NOT IMPLEMENTED

  unsigned int newstate[totalcards];    //integer array to hold data parsed from newstring
  unsigned int revstate[totalcards];    //integer array to hold data parsed from revstring
  unsigned int learnstate[totalcards];  //integer array to hold data parsed from learnstring

  if(SD.exists("/newstatus.txt")){                         //check for newstatus file
    newstring = readFile(SD, "/newstatus.txt");     //if it exists, read it
  }

  else{
    //writeFile(SD, "/newstatus.txt","");               //otherwise, create it
    while(cardidx<totalcards){
      appendFile(SD,"/newstatus.txt","0");             //set all new status to 0
      cardidx+=1;
    }
    newstring = readFile(SD, "/newstatus.txt");  //get string for newstatus we just created
  }

  cardidx=0;

  if(SD.exists("/revstatus.txt")){                         //check for revstatus file
    revstring = readFile(SD, "/revstatus.txt");     //if it exists, read it
  }

  else{
    //writeFile(SD, "/revstatus.txt","");               //otherwise, create it
    while(cardidx<totalcards){
      appendFile(SD,"/revstatus.txt","00");             //set all rev status to 00
      cardidx+=1;
    }
    revstring = readFile(SD, "/revstatus.txt");  //get string for newstatus we just created
  }

  cardidx=0;


  if(SD.exists("/learnstatus.txt")){                         //check for newstatus file
    learnstring = readFile(SD, "/learnstatus.txt");     //if it exists, read it
  }

  else{
    //writeFile(SD, "/newstatus.txt","");               //otherwise, create it
    while(cardidx<totalcards){
      appendFile(SD,"/learnstatus.txt","0");             //set all new status to 0
      cardidx+=1;
    }
    learnstring = readFile(SD, "/learnstatus.txt");  //get string for newstatus we just created
  }


  cardidx = 0;

  for ( s=&newstring[0]; *s != '\0'; s++ ){      //for every character in newstring file
    newstate[cardidx]=((int) *s) - 48;                  //read character into integer array
    cardidx+=1;
  }

  cardidx = 0;

  for ( s=&revstring[0]; *s != '\0'; s++ ){      //for every character in newstring file
    revbuffer[0]=((int) *s) - 48;                        //read first digit
    s+=1;
    revbuffer[1]=((int) *s) - 48;                        //read second digit
    revstate[cardidx]=revbuffer[0]*10+revbuffer[1];    //combine digits into signle integer to store in array
    cardidx+=1;                      
  }

  cardidx=0;

  for ( s=&learnstring[0]; *s != '\0'; s++ ){      //for every character in newstring file
    learnstate[cardidx]=((int) *s) - 48;                  //read character into integer array
    cardidx+=1;                      
  }

  cardidx=0;

  while(newcardcnt<20 && cardidx<totalcards){    //while less than 20 new cards selected and still have cards to search
    if (newstate[cardidx]==0){                    //if card is new
      activecards[activecardcnt]=cardidx;        //add to active cards
      //Serial.println(cardidx);
      activecardcnt+=1;
      newcardcnt+=1;
    }
    cardidx+=1;
  }

  cardidx=0;

  while(activecardcnt<100 && cardidx<totalcards){     //while less than 100 total active cards and still have cards to search
    if(newstate[cardidx]==1 && revstate[cardidx]==0){ //if card not new and revstate says 0 days to review
      activecards[activecardcnt]=cardidx;             //add to active cards
      activecardcnt+=1;
    }
    cardidx+=1;
  }

  //now, activecards[] should contain all the active cards, with activecardcnt telling how many spaces are fill out of the possible 100

  while(cardval!=-1){
      cardval=getrandomnumber(finished, activecardcnt);
      if(cardval==-1){
        continue;
      }


      //show question to card with index cardval in activecards[], NOT IMPLEMENTED
      Serial.print("cardidx:");
      Serial.println(activecards[cardval]);
      Serial.println(cardval);


      

      while(b1state==0 && b2state==0 && b3state==0){       //while no buttons pressed
        delay(10);
        b1state = digitalRead(button1);
        b2state = digitalRead(button2);
        b3state = digitalRead(button3);
      }

      Serial.println("Flipped!");
      
      //update to answer (flip the card)

      b1state = 0;         //set all button reads back to unpressed
      b2state = 0;
      b3state = 0;

      delay(1000);

      while(b1state==0 && b2state==0 && b3state==0){       //while no buttons pressed
        delay(10);
        b1state = digitalRead(button1);
        b2state = digitalRead(button2);
        b3state = digitalRead(button3);
      }

      if(b1state){
        finished[cardval]=1;
        Serial.println("Button1");
        newstate[activecards[cardval]]=1;
        if(learnstate[activecards[cardval]]<9){
          learnstate[activecards[cardval]]+=1;
        }
        revstate[activecards[cardval]]+=learnstate[activecards[cardval]]*4-3;   //THE -3 IS FOR TESTING, KILL IT LATER
        //update pet happy!!
      }

      else if(b3state){
        //update pet sad
        if(learnstate[activecards[cardval]]>0){
          learnstate[activecards[cardval]]-=1;
        }
        Serial.println("Button3");
      }

      else if(b2state){
        Serial.println("Button2");
      }

      b1state = 0;         //set all button reads back to unpressed
      b2state = 0;
      b3state = 0;

      delay(1000);


  }

  //after all cards completed
  Serial.println("wrapping up");

  cardidx=0;

  while(cardidx<totalcards){
    if(revstate[cardidx]>0){          //decrement review counter for all cards
      revstate[cardidx]-=1;
      cardidx+=1;
    }
  }
  
  deleteFile(SD, "/newstatus.txt");
  deleteFile(SD, "/revstatus.txt");
  deleteFile(SD, "/learnstatus.txt");

  cardidx=0;

  for (int i = 0; i<totalcards; i++){

    appendFile(SD,"/newstatus.txt",itoa(newstate[cardidx],s,10));
    if(revstate[cardidx]<10){
      appendFile(SD,"/revstatus.txt","0");                                  
    }                               
    appendFile(SD,"/revstatus.txt",itoa(revstate[cardidx],s,10));                                  
    appendFile(SD,"/learnstatus.txt",itoa(learnstate[cardidx],s,10));
    cardidx+=1;                                 
  }

  Serial.println("Finished");

  while(1){
    delay(1000);
  }


}

void spiCommand(SPIClass *spi, byte data) {
  //use it as you would the regular arduino SPI API
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(spi->pinSS(), LOW); //pull SS slow to prep other end for transfer
  spi->transfer(data);
  digitalWrite(spi->pinSS(), HIGH); //pull ss high to signify end of data transfer
  spi->endTransaction();
}

int getrandomnumber(unsigned int disallowed[], unsigned int maxval) {   
  bool allblocked = true;          //assume all values disallowed
  for(int i = 0; i<maxval; i++){   //check every value between 0 and maxval
    if(disallowed[i]==0){          //if any not disallowed, set allblocked false
      allblocked = false;
    }
  }

  if (allblocked == true){       //if all are blocked
    return -1;
  }

  unsigned int retval=rand()%(maxval);          //pick random number from 0-maxval
  while(disallowed[retval]==1){
    retval=rand()%(maxval);
  }
  return retval;
}
