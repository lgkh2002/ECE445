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
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>


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
    Serial.print(message);
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



  //START OF WAVESHARE CODE::: TESTING



printf("EPD_3IN52_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_3IN52_Init();

    EPD_3IN52_display_NUM(EPD_3IN52_WHITE);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();

    EPD_3IN52_SendCommand(0x50);
    EPD_3IN52_SendData(0x17);

    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_3IN52_WIDTH % 8 == 0)? (EPD_3IN52_WIDTH / 8 ): (EPD_3IN52_WIDTH / 8 + 1)) * EPD_3IN52_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_3IN52_WIDTH, EPD_3IN52_HEIGHT, 270, WHITE);
    Paint_Clear(WHITE);
    
#if 1   // GC waveform refresh 
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	Paint_DrawBitMap(gImage_3in52);
		
    EPD_3IN52_display(BlackImage);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();
    DEV_Delay_ms(2000);


#endif

#if 0  //DU waveform refresh
    printf("Quick refresh is supported, but the refresh effect is not good, but it is not recommended\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_3in52);
		
    EPD_3IN52_display(BlackImage);
    EPD_3IN52_lut_DU();
    EPD_3IN52_refresh();
    DEV_Delay_ms(2000);

#endif

#if 1 
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    printf("EPD_Display\r\n");
    EPD_3IN52_display(BlackImage);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();
    DEV_Delay_ms(2000);
#endif

    printf("Clear...\r\n");
    EPD_3IN52_Clear();
    
    // Sleep & close 5V
    printf("Goto Sleep...\r\n");
    EPD_3IN52_sleep();

    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    printf("close 5V, Module enters 0 power consumption ...\r\n");
















////////////////start of SD CODE!!!!!!!!! this is known to be working!!!
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

  srand(time(NULL));





}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.printf("TEST");
  //delay(500);
  //chip select high
  spiCommand(hspi, 0b01010101);

  int randval;
  String newstring;
  String revstring;

  unsigned int cardidx =0 ;
  unsigned int newcardcnt = 0;
  unsigned int totalcards = 0;
  unsigned int activecardcnt = 0;

  unsigned int activecards[100];
  unsigned int finished[100] = {0};


  //get total card number from parser

  unsigned int newstate[totalcards];
  unsigned int revstate[totalcards];

  if(SD.exists("/newstatus.txt")){                         //check for newstatus file
    newstring = readFile(SD, "/newstatus.txt");     //if it exists, read it
  }

  else{
    writeFile(SD, "/newstatus.txt","");               //otherwise, create it
    while(cardidx<totalcards){
      appendFile(SD,"/newstatus.txt","0");             //set all new status to 0
      cardidx+=1;
    }
    newstring = readFile(SD, "/newstatus.txt");  //get string for newstatus we just created
  }

  if(SD.exists("/revstatus.txt")){                         //check for revstatus file
    revstring = readFile(SD, "/revstatus.txt");     //if it exists, read it
  }

  else{
    writeFile(SD, "/revstatus.txt","");               //otherwise, create it
    while(cardidx<totalcards){
      appendFile(SD,"/newstatus.txt","0");             //set all rev status to 0
      cardidx+=1;
    }
    revstring = readFile(SD, "/revstatus.txt");  //get string for newstatus we just created
  }



  cardidx = 0;
  char * s;

  for ( s=&newstring[0]; *s != '\0'; s++ ){      //for every character in newstring file
    newstate[cardidx]=atoi(s);                  //read character into integer array
    cardidx+=1;
    s+=1;                                     
  }

  cardidx = 0;

  for ( s=&revstring[0]; *s != '\0'; s++ ){      //for every character in newstring file
    revstate[cardidx]=atoi(s);                  //read character into integer array
    s+=1;               
    cardidx+=1;                      
  }


  while(newcardcnt<20 && cardidx<totalcards){    //while less than 20 new cards selected and still have cards to search
    if (newstate[cardidx]==0){                    //if card is new
      activecards[activecardcnt]=cardidx;        //add to active cards
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

  while(randval!=-1){
      randval=getrandomnumber(finished, activecardcnt);
      if(randval==-1){
        continue;
      }

      cardidx=randval;
      //show question to card with index cardidx


      int b1state = digitalRead(button1);  //read all buttons
      int b2state = digitalRead(button2);
      int b3state = digitalRead(button3);

      while(b1state==0 && b2state==0 && b3state==0){       //while no buttons pressed
        delay(10);
        b1state = digitalRead(button1);
        b2state = digitalRead(button2);
        b3state = digitalRead(button3);
      }

      //update to answer

      b1state = 0;         //set all button reads back to unpressed
      b2state = 0;
      b3state = 0;

      //TODO: once a button is pressed, move to next card and update if a card is finished or not
  }

  //TODO: add end state stuff once all cards finished

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
  for(int i = 0; i<=maxval; i++){   //check every value between 0 and maxval
    if(disallowed[i]==0){          //if any not disallowed, set allblocked false
      allblocked = false;
    }
  }

  if (allblocked == true){       //if all are blocked
    return -1;
  }

  unsigned int retval=rand()%(maxval+1);          //pick random number from 0-maxval
  while(disallowed[retval]==1){
    retval=rand()%(maxval+1);
  }
  return retval;
}
