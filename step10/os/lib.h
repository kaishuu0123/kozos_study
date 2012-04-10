#ifndef _LIB_H_INCLUDED_
#define _LIB_H_INCLUDED_

void *memset(void *, int, long);
void *memcpy(void *, const void *, long);
int memcmp(const void *, const void *, long);
int strlen(const char *);
char *strcpy(char *, const char *);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, int);

int putc(unsigned char);
int puts(unsigned char *);
unsigned char getc(void);
int gets(unsigned char *buf);
int putxval(unsigned long, int);

#endif
