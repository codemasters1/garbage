#ifndef __SERIAL_H
#define __SERIAL_H

void Serial_Init(void);
void Serial_Putc(char);
char Serial_Getc(void);

void Serial_Puts(char *);
uint16_t Serial_SendBuffer(char *, uint16_t);

#endif
