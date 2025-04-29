#ifndef STRING_HPP
#define STRING_HPP

#include <stdlib.h>

int strcmp2(char * s1, char * s2);	   // strcmp from left, ignoring '\0'
void mergestr(char * s1, char * s2, char * merged);

#include "string.cpp"

#endif
