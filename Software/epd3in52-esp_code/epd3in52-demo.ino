#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>

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
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  //
}
