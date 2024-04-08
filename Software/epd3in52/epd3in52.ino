/**
    @filename   :   epd3in52-demo.ino
    @brief      :   3.52inch e-paper display demo
    @author     :   Waveshare

    Copyright (C) Waveshare     2022/7/22

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documnetation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to  whom the Software is
   furished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#include <SPI.h>
#include "EPD_3in52.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

#define WORDSPERROW 25

// Memory buffer storing content to be displayed (360x240/8bits = 10800), 1 pixel = 8 bits
// unsigned char image[980]; 
// Paint paint(image, 40, 180);
char str1[] = "An apple is a round, ";

int copySubstring(char* source, char* destination, int start, int end) {
    int j = 0; // Index for destination
    int lastSpaceIdx = 0;

    //copied and find last space position
    for (int i = start; i <= end && source[i] != '\0'; i++) {
        if (source[i] == ' '){
          lastSpaceIdx = i;
        }
        destination[j++] = source[i];
    }
    destination[j] = '\0'; // Null-terminate the destination

    destination[lastSpaceIdx] = '\0'; //end early due to space

    return lastSpaceIdx;
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Epd epd;
    if (epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
    //Serial.print(strlen(str1));
    Serial.print("e-Paper Clear\r\n ");

    epd.display_NUM(EPD_3IN52_WHITE);
    epd.lut_GC();
    epd.refresh();

    epd.SendCommand(0x50);
    epd.SendData(0x17);

    delay(2000);

    UBYTE image[980];
    Paint paint(image, 40, 180);    // width should be the multiple of 8   

    paint.SetRotate(ROTATE_270);
    paint.Clear(COLORED);

    Serial.print("Drawing:BlackImage\r\n ");
    //paint.DrawStringAt(0, 0, "Apple", &Font12, UNCOLORED);
    //paint.DrawStringAt(0, 12, "n. A kind of fruit.", &Font12, UNCOLORED); //x, y

    int start = 0;
    int tot = strlen(str1); // Total length of the string
    int remain = tot;
    int row = 0; 
    int lastSpace = 0;
    char rowstr[WORDSPERROW];

    while (remain > WORDSPERROW) { // Assuming width = 180 and char width = 7
        lastSpace = copySubstring(str1, rowstr, start, start + WORDSPERROW - 1);
        Serial.print(rowstr);
        remain -= lastSpace - start;
        start = lastSpace + 1;
        
        // Use c_str() to convert the String to a const char* for DrawStringAt
        paint.DrawStringAt(2, row * 12, rowstr, &Font12, UNCOLORED);
        
        row++; // Increment row for next drawing position
    }
    //last line
    copySubstring(str1, rowstr, start, start + remain);
    paint.DrawStringAt(2, row * 12, rowstr, &Font12, UNCOLORED);

    epd.display_part(paint.GetImage(), 20, 160, paint.GetWidth(), paint.GetHeight());
    
    epd.lut_GC();
    epd.refresh();
    delay(2000);
    
    
    // epd.display(IMAGE_DATA);
    // epd.lut_GC();
    // epd.refresh();
    // delay(2000);
    
    Serial.print("clear and sleep......\r\n ");
    // epd.Clear();
    // delay(2000);
    epd.sleep();
    Serial.print("end\r\n ");
}

void loop() {
    // put your main code here, to run repeatedly:

}
