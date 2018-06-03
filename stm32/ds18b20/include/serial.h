#ifndef __SERIAL_H
#define __SERIAL_H

void serial_init(void);
void serial_putc(char);
char serial_getc(void);

void serial_puts(char *);

#endif
