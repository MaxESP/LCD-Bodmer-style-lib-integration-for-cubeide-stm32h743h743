#include "retarget.h"
#include <stdio.h>

/* External UART handle */
extern UART_HandleTypeDef huart1;

/**
  * @brief  Retarget putchar for printf
  */
int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 100);
    return ch;
}

/**
  * @brief  Retarget write for printf
  */
int _write(int file, char *ptr, int len) {
    (void)file;
    //HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 100);//**************************avoir
    HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}

/**
  * @brief  Get character for scanf (not used but required)
  */
int __io_getchar(void) {
    uint8_t ch = 0;
    HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY);
    return ch;
}

/**
  * @brief  Check if serial data available (non-blocking)
  */
int Serial_Available(void) {
    return __HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE);
}

/**
  * @brief  Read one character (blocking)
  */
char Serial_Read(void) {
    uint8_t ch = 0;
    HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY);
    return (char)ch;
}

/**
  * @brief  Read a line from serial
  */
int Serial_ReadLine(char *buffer, int max_len) {
    int idx = 0;
    char ch;

    while (idx < max_len - 1) {
        ch = Serial_Read();

        // Echo
        if (ch != '\r' && ch != '\n') {
            HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 100);
        }

        // Handle backspace
        if (ch == '\b' || ch == 0x7F) {
            if (idx > 0) {
                idx--;
                HAL_UART_Transmit(&huart1, (uint8_t*)" \b", 2, 100);
            }
            continue;
        }

        // Handle Enter
        if (ch == '\r' || ch == '\n') {
            buffer[idx] = '\0';
            printf("\r\n");
            return idx;
        }

        // Store character
        buffer[idx++] = ch;
    }

    buffer[idx] = '\0';
    return idx;
}
