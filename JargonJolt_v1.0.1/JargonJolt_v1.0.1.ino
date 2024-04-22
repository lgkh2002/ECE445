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


#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
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
#define hcs2 47

#define button1 8
#define button2 9
#define button3 20

#define I2S_DOUT      19
#define I2S_BCLK      18
#define I2S_LRC       17

#define WORDSPERROW 50

#define PET_WELLDONE 0
#define PET_NORMAL 1
#define PET_BAD 2
#define PET_HEALTH1 3
#define PET_HEALTH2 4
#define PET_HEALTH3 5
#define PET_HEALTH4 6

#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define VSPI 0
#define HSPI 1
#endif


String ssid =     "Luke_samsung";
String password = "jvpb9532";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -21600;
const int   daylightOffset_sec = 3600;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


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


void drawtext(UBYTE * image, int x, int y, char * message, int CS){
  digitalWrite(CS, LOW);
  Paint_SelectImage(image);
  Paint_Clear(WHITE);
  Paint_DrawString_EN(x, y, message, &Font12, WHITE, BLACK);
  EPD_3IN52_display(image);
  //EPD_3IN52_lut_GC();
  //EPD_3IN52_refresh();
  DEV_Delay_ms(500);
  digitalWrite(CS,HIGH);
}

void drawwraptext(UBYTE * image, int x, int y, char * message, int CS){
  digitalWrite(CS, LOW);
  Paint_SelectImage(image);
  Paint_Clear(WHITE);

  int start = 0;
  int tot = strlen(message); // Total length of the string
  int remain = tot;
  int row = 0; 
  int lastSpace = 0;
  char rowstr[WORDSPERROW];

  while (remain > WORDSPERROW) { // Assuming width = 180 and char width = 7
      lastSpace = copySubstring(message, rowstr, start, start + WORDSPERROW - 1);
      Serial.print(rowstr);
      Serial.print("\r\n");
      remain -= lastSpace - start;
      start = lastSpace + 1;     
      Paint_DrawString_EN(5, row * 12, rowstr, &Font12, WHITE, BLACK);
      row++; 
  }
  //last line
  copySubstring(message, rowstr, start, start + remain);
  Paint_DrawString_EN(5, row * 12, rowstr, &Font12, WHITE, BLACK);
  EPD_3IN52_display(image);
  //EPD_3IN52_lut_GC();
  //EPD_3IN52_refresh();
  digitalWrite(CS, HIGH);
  DEV_Delay_ms(500);
}

void drawpet(UBYTE * image, UWORD status, int CS){
  digitalWrite(CS,LOW);
  Paint_SelectImage(image);
  Paint_Clear(WHITE);
  switch (status){
    case 0:
        Paint_DrawBitMap(gImage_welldone);
        break;
    case 1:
        Paint_DrawBitMap(gImage_normal);
        break;
    case 2:
        Paint_DrawBitMap(gImage_bad);
        break;
    case 3:
        Paint_DrawBitMap(gImage_hs1);
        break;
    case 4:
        Paint_DrawBitMap(gImage_hs2);
        break;
    case 5:
        Paint_DrawBitMap(gImage_hs3);
        break;
    case 6:
        Paint_DrawBitMap(gImage_hs4);
        break;
  }
  EPD_3IN52_display(image);
  DEV_Delay_ms(1000);
  digitalWrite(CS,HIGH);
}

int copySubstring(char* source, char* destination, int start, int end) {
    int j = 0; // Index for destination
    int lastSpaceIdx = 0;
    int lsi = 0;

    //copied and find last space position
    for (int i = start; i <= end && source[i] != '\0'; i++) {
        if (source[i] == ' '){
          lastSpaceIdx = i;
          lsi = j;
        }
        destination[j++] = source[i];
    }
    destination[j] = '\0'; // Null-terminate the destination

    if (lsi != -1) {
        for (int i = lsi; i < end; i++) {
            destination[i] = '\0';
        }
    }

    return lastSpaceIdx;
}


static const int spiClk = 1000000;
SPIClass * vspi = NULL; 
SPIClass * hspi = NULL;


void setup() {
  // put your setup code here, to run once:

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
  if(!SD.begin(vspi->pinSS(),*vspi,4000000, "/sd", 10)){
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


  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");


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
  String cardstring;       //holds raw data from flashcards.txt
  String datestring;

  String question;
  String answer;

  unsigned int cardidx =0 ;        //indexes cards from the output of the text parser
  unsigned int newcardcnt = 0;     //counts how many new cards have been added to activecards[]
  unsigned int totalcards = 1;    //contains the total cards in the set. will be set by the text parser.
  unsigned int activecardcnt = 0;  //counts how many cards have been added to activecards[]

  unsigned int activecards[100];   //contains the indexes of all active cards
  unsigned int finished[100] = {0};//contains info if a card has been finished for the day. 1 is finished, 0 is not finished
  int revbuffer[2];
  int day;
  int dayspast;
  int pethealth = 0;

  int b1state = 0;
  int b2state = 0;
  int b3state = 0;

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    day = 0;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  
  day=(timeinfo.tm_yday+365*timeinfo.tm_year);

  srand(day);

    //START OF WAVESHARE SETUP


printf("EPD_3IN52_setup\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");

    digitalWrite(hcs, LOW);
    digitalWrite(hcs2, LOW);
    EPD_3IN52_Init();

    EPD_3IN52_display_NUM(EPD_3IN52_WHITE);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();

    EPD_3IN52_SendCommand(0x50);
    EPD_3IN52_SendData(0x17);

    digitalWrite(hcs, HIGH);
    digitalWrite(hcs2, HIGH);

    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *Image;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_3IN52_WIDTH % 8 == 0)? (EPD_3IN52_WIDTH / 8 ): (EPD_3IN52_WIDTH / 8 + 1)) * EPD_3IN52_HEIGHT;
    if((Image = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

    printf("Paint_NewImage\r\n");

    digitalWrite(hcs, LOW);
    digitalWrite(hcs2, LOW);

    Paint_NewImage(Image, EPD_3IN52_WIDTH, EPD_3IN52_HEIGHT, 270, WHITE);
    Paint_Clear(WHITE);

    digitalWrite(hcs, HIGH);
    digitalWrite(hcs2, HIGH);
    
/*
#if 1   // GC waveform refresh 
    Paint_SelectImage(Image);
    Paint_Clear(WHITE);
//	  Paint_DrawBitMap(gImage_3in52);	
    EPD_3IN52_display(Image);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();
    
#endif
*/

/*
#if 0  //DU waveform refresh
    printf("Quick refresh is supported, but the refresh effect is not good, but it is not recommended\r\n");
    Paint_SelectImage(Image);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_3in52);
		
    EPD_3IN52_display(Image);
    EPD_3IN52_lut_DU();
    EPD_3IN52_refresh();
    DEV_Delay_ms(2000);

#endif
*/

/*
#if 1 
    printf("SelectImage:Image\r\n");
    Paint_SelectImage(Image);
    Paint_Clear(WHITE);

    printf("Drawing:Image\r\n");
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
    EPD_3IN52_display(Image);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();
    DEV_Delay_ms(2000);
#endif
*/

/*
    printf("Clear...\r\n");
    EPD_3IN52_Clear();
    
    // Sleep & close 5V
    printf("Goto Sleep...\r\n");
    EPD_3IN52_sleep();

    free(Image);
    Image = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    printf("close 5V, Module enters 0 power consumption ...\r\n");

*/

  if(SD.exists("/dateinfo.txt")){
    datestring=readFile(SD,"/dateinfo.txt");
    s=&datestring[0];
    dayspast+=10000* *s;
    s+=1;
    dayspast+=1000* *s;
    s+=1;
    dayspast+=100* *s;
    s+=1;
    dayspast+=10* *s;
    s+=1;
    dayspast+=*s;
  }
  else{
    dayspast=day-1;
  }

  dayspast=day-dayspast;

  if(dayspast==0){
    drawtext(Image, 50,100, "Finished for the day, come back tomorrow!", hcs);
    digitalWrite(hcs,LOW);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();
    digitalWrite(hcs,HIGH);
    while(1){
      delay(1000);
    }
  }


  if(SD.exists("/flashcards.txt")){
    cardstring = readFile(SD, "/flashcards.txt"); 
  }

  else{
    drawtext(Image, 200,200, "Please add flashcard file to SD card, then restart the device", hcs);
    digitalWrite(hcs,LOW);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();
    digitalWrite(hcs,HIGH);
    while(1){
      delay(1000);
    }
  }

  for ( s=&cardstring[0]; *s!='\0'; s++){
    if(*s=='\n'){
      totalcards+=1;
    }
  }

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
    revstate[cardidx]=revbuffer[0]*10+revbuffer[1];    //combine digits into single integer to store in array
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
      question = getquestion(activecards[cardval],cardstring);
      drawtext(Image, 100 ,100, &(question[0]), hcs);
      digitalWrite(hcs,LOW);
      EPD_3IN52_lut_GC();
      EPD_3IN52_refresh();
      digitalWrite(hcs,HIGH);


      

      while(b1state==0 && b2state==0 && b3state==0){       //while no buttons pressed
        delay(10);
        b1state = digitalRead(button1);
        b2state = digitalRead(button2);
        b3state = digitalRead(button3);
      }

      Serial.println("Flipped!");
      answer=getanswer(activecards[cardval],cardstring);
      drawtext(Image, 100 ,100,&(answer[0]), hcs);
      digitalWrite(hcs,LOW);
      EPD_3IN52_lut_GC();
      EPD_3IN52_refresh();
      digitalWrite(hcs,HIGH);



      

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
        drawtext(Image, 10 ,20,"Button 1", hcs);
        drawpet(Image, PET_WELLDONE, hcs2);
      }

      else if(b3state){
        //update pet sad
        if(learnstate[activecards[cardval]]>0){
          learnstate[activecards[cardval]]-=1;
        }
        Serial.println("Button3");
        drawtext(Image, 10 ,20,"Button 3", hcs);
        drawpet(Image, PET_BAD, hcs2);

      }

      else if(b2state){
        Serial.println("Button2");
        drawtext(Image, 10 ,20,"Button 2", hcs);
        drawpet(Image, PET_NORMAL, hcs2);
      }

      digitalWrite(hcs,LOW);
      digitalWrite(hcs2,LOW);
      EPD_3IN52_lut_GC();
      EPD_3IN52_refresh();
      digitalWrite(hcs2,HIGH);
      digitalWrite(hcs,HIGH);

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
    }
    cardidx+=1;
  }
  
  deleteFile(SD, "/newstatus.txt");
  deleteFile(SD, "/revstatus.txt");
  deleteFile(SD, "/learnstatus.txt");
  deleteFile(SD, "/dateinfo.txt");

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

  appendFile(SD,"/dateinfo.txt", itoa(day,s,10));

  Serial.println("Finished");
  drawtext(Image, 10 ,20,"Finished", hcs);
  digitalWrite(hcs,LOW);
  EPD_3IN52_lut_GC();
  EPD_3IN52_refresh();
  digitalWrite(hcs,HIGH);

  // Sleep & close 5V
  printf("Goto Sleep...\r\n");
  digitalWrite(hcs, LOW);
  digitalWrite(hcs2, LOW);
  EPD_3IN52_sleep();
  digitalWrite(hcs, HIGH);
  digitalWrite(hcs2, HIGH);

  free(Image);
  Image = NULL;
  DEV_Delay_ms(2000);//important, at least 2s
  printf("close 5V, Module enters 0 power consumption ...\r\n");

  

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

String getquestion(unsigned int cardidx, String cards){
  char * c;
  c = &cards[0];
  String question;

  for(int cardnum = 0; cardnum<cardidx; cardnum++){
    while(*c != '\n'){
      c+=1;               //step to end of the line
    }
    c+=1;
  }

  //c should now point to start of question

  while(*c != 9){
    question+=*c;
    c+=1;
  }

  return question;

}

String getanswer(unsigned int cardidx, String cards){
  char * c;
  c = &cards[0];
  String answer;

  for(int cardnum = 0; cardnum<cardidx; cardnum++){
    while(*c != '\n'){
      c+=1;               //step to end of the line
    }
    c+=1;
  }

  while(*c != 9){
    c+=1;
  }

  c+=1;

  //c should now point to start of answer

  while(*c != 9 && *c != '\n' && *c != 13 && *c != 10){
    answer+=*c;
    c+=1;
  }

  return answer;

}

