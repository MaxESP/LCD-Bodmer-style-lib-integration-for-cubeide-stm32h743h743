

#include "test_lcd.h"
#include "text_layer.h"
#include "dma2d_driver.h"
#include "text_engine.h"
#include <stdlib.h>
#include "font_arial42.h"
#include "font_arial56.h"
#include "font_arial36.h"
#include "stdio.h"


void TEXT_ColorBarTest(void)
{
    TEXT_LAYER_Clear(TEXT_LAYER_GetBufferAddress(), COLOR_BLACK);

    uint16_t barWidth = 1024 / 16;

    /* Draw all colors */
    DMA2D_FillRect(0xC0000000, 0 * barWidth, 0, barWidth, 600, COLOR_RED);
    DMA2D_FillRect(0xC0000000, 1 * barWidth, 0, barWidth, 600, COLOR_GREEN);
    DMA2D_FillRect(0xC0000000, 2 * barWidth, 0, barWidth, 600, COLOR_BLUE);
    DMA2D_FillRect(0xC0000000, 3 * barWidth, 0, barWidth, 600, COLOR_YELLOW);
    DMA2D_FillRect(0xC0000000, 4 * barWidth, 0, barWidth, 600, COLOR_CYAN);
    DMA2D_FillRect(0xC0000000, 5 * barWidth, 0, barWidth, 600, COLOR_MAGENTA);
    DMA2D_FillRect(0xC0000000, 6 * barWidth, 0, barWidth, 600, COLOR_WHITE);
    DMA2D_FillRect(0xC0000000, 7 * barWidth, 0, barWidth, 600, COLOR_ORANGE);
    DMA2D_FillRect(0xC0000000, 8 * barWidth, 0, barWidth, 600, COLOR_GRAY);
    DMA2D_FillRect(0xC0000000, 9 * barWidth, 0, barWidth, 600, COLOR_LIGHTGRAY);
    DMA2D_FillRect(0xC0000000, 10 * barWidth, 0, barWidth, 600, COLOR_DARKGRAY);
    DMA2D_FillRect(0xC0000000, 11 * barWidth, 0, barWidth, 600, COLOR_NAVY);
    DMA2D_FillRect(0xC0000000, 12 * barWidth, 0, barWidth, 600, COLOR_PURPLE);
    DMA2D_FillRect(0xC0000000, 13 * barWidth, 0, barWidth, 600, COLOR_PINK);
    DMA2D_FillRect(0xC0000000, 14 * barWidth, 0, barWidth, 600, COLOR_GOLD);
    DMA2D_FillRect(0xC0000000, 15 * barWidth, 0, barWidth, 600, COLOR_SKYBLUE);

    HAL_Delay(3000);
}


void TEST_LCD_ColorNames(void)
{
    //char buf[32];
    int y = 50;

    /* Clear screen */
    TEXT_LAYER_Clear(TEXT_LAYER_GetBufferAddress(), COLOR_BLACK);

    /*========================================================================
     * Row 1 - Primary Colors
     *========================================================================*/
    /* RED */
    TEXT_SetCursor(50, y);
    TEXT_SetTextColor(COLOR_RED, COLOR_BLACK);
    TEXT_Print(&Arial42, "RED");

    /* GREEN */
    TEXT_SetCursor(200, y);
    TEXT_SetTextColor(COLOR_GREEN, COLOR_BLACK);
    TEXT_Print(&Arial42, "GREEN");

    /* BLUE */
    TEXT_SetCursor(350, y);
    TEXT_SetTextColor(COLOR_BLUE, COLOR_BLACK);
    TEXT_Print(&Arial42, "BLUE");

    /* WHITE */
    TEXT_SetCursor(500, y);
    TEXT_SetTextColor(COLOR_WHITE, COLOR_BLACK);
    TEXT_Print(&Arial42, "WHITE");

    HAL_Delay(1000);

    /*========================================================================
     * Row 2 - Secondary Colors
     *========================================================================*/
    y = 150;

    /* YELLOW */
    TEXT_SetCursor(50, y);
    TEXT_SetTextColor(COLOR_YELLOW, COLOR_BLACK);
    TEXT_Print(&Arial42, "YELLOW");

    /* CYAN */
    TEXT_SetCursor(250, y);
    TEXT_SetTextColor(COLOR_CYAN, COLOR_BLACK);
    TEXT_Print(&Arial42, "CYAN");

    /* MAGENTA */
    TEXT_SetCursor(450, y);
    TEXT_SetTextColor(COLOR_MAGENTA, COLOR_BLACK);
    TEXT_Print(&Arial42, "MAGENTA");

    HAL_Delay(1000);

    /*========================================================================
     * Row 3 - Bright Colors
     *========================================================================*/
    y = 250;

    /* ORANGE */
    TEXT_SetCursor(50, y);
    TEXT_SetTextColor(COLOR_ORANGE, COLOR_BLACK);
    TEXT_Print(&Arial42, "ORANGE");

    /* PURPLE */
    TEXT_SetCursor(250, y);
    TEXT_SetTextColor(COLOR_PURPLE, COLOR_BLACK);
    TEXT_Print(&Arial42, "PURPLE");

    /* PINK */
    TEXT_SetCursor(450, y);
    TEXT_SetTextColor(COLOR_PINK, COLOR_BLACK);
    TEXT_Print(&Arial42, "PINK");

    HAL_Delay(1000);

    /*========================================================================
     * Row 4 - Metallic Colors
     *========================================================================*/
    y = 350;

    /* GOLD */
    TEXT_SetCursor(50, y);
    TEXT_SetTextColor(COLOR_GOLD, COLOR_BLACK);
    TEXT_Print(&Arial42, "GOLD");

    /* SILVER */
    TEXT_SetCursor(250, y);
    TEXT_SetTextColor(COLOR_SILVER, COLOR_BLACK);
    TEXT_Print(&Arial42, "SILVER");

    /* SKYBLUE */
    TEXT_SetCursor(450, y);
    TEXT_SetTextColor(COLOR_SKYBLUE, COLOR_BLACK);
    TEXT_Print(&Arial42, "SKYBLUE");

    HAL_Delay(1000);

    /*========================================================================
     * Row 5 - Dark Colors
     *========================================================================*/
    y = 450;

    /* NAVY */
    TEXT_SetCursor(50, y);
    TEXT_SetTextColor(COLOR_NAVY, COLOR_BLACK);
    TEXT_Print(&Arial42, "NAVY");

    /* TEAL */
    TEXT_SetCursor(250, y);
    TEXT_SetTextColor(COLOR_TEAL, COLOR_BLACK);
    TEXT_Print(&Arial42, "TEAL");

    /* BROWN */
    TEXT_SetCursor(450, y);
    TEXT_SetTextColor(COLOR_BROWN, COLOR_BLACK);
    TEXT_Print(&Arial42, "BROWN");

    HAL_Delay(1000);

    /* Clear screen at end */
    TEXT_LAYER_Clear(TEXT_LAYER_GetBufferAddress(), COLOR_BLACK);
}


void LCD_RandomPitchDemo(void)
             {
                 int pitch;
                 char buf[16];
                 int absPitch;

                 srand(HAL_GetTick());

                 while (1)
                 {
                     pitch = (rand() % 71) - 35;
                     absPitch = abs(pitch);

                     /* EXPANDED clear area - catch that stray pixel */
                     TEXT_LAYER_ClearArea(360, 130, 175, 93, COLOR_BLACK);  /* 5px extra on all sides */
                     TEXT_LAYER_ClearArea(478, 135, 10, 10, COLOR_BLACK);  /* Right side of second digit */
                     if (pitch < 0)
                     {
                         /* Minus sign in RED */
                         TEXT_SetTextColor(COLOR_RED, COLOR_BLACK);
                         TEXT_SetCursor(405, 160);
                         TEXT_Print(&Arial56, "-");

                         /* Number in RED */
                         snprintf(buf, sizeof(buf), "%02d", absPitch);
                         TEXT_SetCursor(435, 160);
                         TEXT_Print(&Arial56, buf);
                         TEXT_SetCursor(500, 135);
                         TEXT_Print(&Arial36, "o");
                     }
                     else
                     {
                         /* Positive number in GREEN */
                         TEXT_SetTextColor(COLOR_GREEN, COLOR_BLACK);
                         snprintf(buf, sizeof(buf), "%02d", pitch);
                         TEXT_SetCursor(435, 160);
                         TEXT_Print(&Arial56, buf);
                         TEXT_SetCursor(500, 135);
                         TEXT_Print(&Arial36, "o");

                     }

                     HAL_Delay(500);
                 }
             }

