#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>

#define WORDSPERROW 50

#define PET_EXCELLENT 0
#define PET_WELLDONE 1
#define PET_NORMAL 2
#define PET_BAD 3
#define PET_TERRIBLE 4

char str1[] = "An apple is a round, edible fruit produced by an apple tree (Malus spp., among them the domestic or orchard apple; Malus domestica). Apple trees are cultivated worldwide and are the most widely grown species in the genus Malus. ";

void drawpet(UBYTE * image, UWORD status){
  Paint_SelectImage(image);
  Paint_Clear(WHITE);
  switch (status){
    case 0:
        Paint_DrawBitMap(gImage_excellent);
        break;
    case 1:
        Paint_DrawBitMap(gImage_welldone);
        break;
    case 2:
        Paint_DrawBitMap(gImage_normal);
        break;
    case 3:
        Paint_DrawBitMap(gImage_bad);
        break;
    case 4:
        Paint_DrawBitMap(gImage_terrible);
        break;
  }
  EPD_3IN52_display(image);
  EPD_3IN52_lut_DU();
  EPD_3IN52_refresh();
  DEV_Delay_ms(1000);
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

void setup() {
    Serial.print("e-Paper Clear\r\n ");
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
    
#if 0   // GC waveform refresh 
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	Paint_DrawBitMap(gImage_3in52);
		
    EPD_3IN52_display(BlackImage);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();
    DEV_Delay_ms(2000);


#endif

#if 1  //DU waveform refresh
    // printf("Quick refresh is supported, but the refresh effect is not good, but it is not recommended\r\n");
    // Paint_SelectImage(BlackImage);
    // Paint_Clear(WHITE);
    // Paint_DrawBitMap(gImage_excellent);
    // EPD_3IN52_display(BlackImage);
    // EPD_3IN52_lut_DU();
    // EPD_3IN52_refresh();
    // DEV_Delay_ms(1000);
    drawpet(BlackImage, PET_BAD);
#endif

// #if 1  //DU waveform refresh
//     printf("Quick refresh is supported, but the refresh effect is not good, but it is not recommended\r\n");
//     Paint_SelectImage(BlackImage);
//     Paint_Clear(WHITE);
//     Paint_DrawBitMap(gImage_welldone);
//     EPD_3IN52_display(BlackImage);
//     EPD_3IN52_lut_DU();
//     EPD_3IN52_refresh();
//     DEV_Delay_ms(1000);
// #endif

// #if 1  //DU waveform refresh
//     printf("Quick refresh is supported, but the refresh effect is not good, but it is not recommended\r\n");
//     Paint_SelectImage(BlackImage);
//     Paint_Clear(WHITE);
//     Paint_DrawBitMap(gImage_normal);
//     EPD_3IN52_display(BlackImage);
//     EPD_3IN52_lut_DU();
//     EPD_3IN52_refresh();
//     DEV_Delay_ms(1000);
// #endif

// #if 1  //DU waveform refresh
//     printf("Quick refresh is supported, but the refresh effect is not good, but it is not recommended\r\n");
//     Paint_SelectImage(BlackImage);
//     Paint_Clear(WHITE);
//     Paint_DrawBitMap(gImage_bad);
//     EPD_3IN52_display(BlackImage);
//     EPD_3IN52_lut_DU();
//     EPD_3IN52_refresh();
//     DEV_Delay_ms(1000);
// #endif

// #if 1  //DU waveform refresh
//     printf("Quick refresh is supported, but the refresh effect is not good, but it is not recommended\r\n");
//     Paint_SelectImage(BlackImage);
//     Paint_Clear(WHITE);
//     Paint_DrawBitMap(gImage_terrible);
//     EPD_3IN52_display(BlackImage);
//     EPD_3IN52_lut_DU();
//     EPD_3IN52_refresh();
//     DEV_Delay_ms(2000);
// #endif


#if 0 
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    printf("Drawing:BlackImage\r\n");

    int start = 0;
    int tot = strlen(str1); // Total length of the string
    int remain = tot;
    int row = 0; 
    int lastSpace = 0;
    char rowstr[WORDSPERROW];

    while (remain > WORDSPERROW) { // Assuming width = 180 and char width = 7
        lastSpace = copySubstring(str1, rowstr, start, start + WORDSPERROW - 1);
        Serial.print(rowstr);
        Serial.print("\r\n");
        remain -= lastSpace - start;
        start = lastSpace + 1;
        
        Paint_DrawString_EN(5, row * 12, rowstr, &Font12, WHITE, BLACK);
        
        row++; 
    }
    //last line
    copySubstring(str1, rowstr, start, start + remain);
    Paint_DrawString_EN(5, row * 12, rowstr, &Font12, WHITE, BLACK);
    
    // Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    // Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    // Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    // Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    // Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    // Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    // Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    // Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    // Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    // Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    // Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    printf("EPD_Display\r\n");
    EPD_3IN52_display(BlackImage);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();
    DEV_Delay_ms(2000);
#endif

    printf("Clear...\r\n");
    //EPD_3IN52_Clear();
    
    // Sleep & close 5V
    printf("Goto Sleep...\r\n");
    EPD_3IN52_sleep();

    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    printf("close 5V, Module enters 0 power consumption ...\r\n");
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  //
}
