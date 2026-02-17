#ifndef __TEST_LCD_H__
#define __TEST_LCD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*============================================================================
 * Display Test Functions
 *============================================================================*/

/**
 * @brief Display 16 vertical color bars (RGB test pattern)
 * @note Width: 1024/16 = 64px per bar, Height: 600px
 */
//void LCD_ColorBarTest(void);
void TEXT_ColorBarTest(void);
/**
 * @brief Cycle through solid colors
 */
void TEST_LCD_ColorNames(void);




void LCD_RandomPitchDemo(void);




#ifdef __cplusplus
}
#endif

#endif /* __LCD_TEST_H__ */
