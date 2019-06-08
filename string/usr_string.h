#ifndef USR_STRING_H_
#define USR_STRING_H_

#include <stdlib.h>
// #inlcude "clib.h"

char *usr_strncat(char *str1, const char *str2, size_t n);
int usr_strlen(const char *str);
int usr_sprintf(char *str, const char *format, void *argv[], size_t n);

#endif