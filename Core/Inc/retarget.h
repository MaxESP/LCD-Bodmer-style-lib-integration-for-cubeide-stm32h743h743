#ifndef __RETARGET_H
#define __RETARGET_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Serial functions */
int __io_putchar(int ch);
int _write(int file, char *ptr, int len);
int __io_getchar(void);
int Serial_Available(void);
char Serial_Read(void);
int Serial_ReadLine(char *buffer, int max_len);

#ifdef __cplusplus
}
#endif

#endif /* __RETARGET_H */
