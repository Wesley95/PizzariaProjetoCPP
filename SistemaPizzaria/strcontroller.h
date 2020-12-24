#pragma once

#include <stdio.h>

//----------------------------
//PROTOTIPAGEM CONTROLE DE STR
//----------------------------
char* substring(const char* text, const int startIndex, const int charCount);
char* lower(const char* str);
char* upper(const char* str);
bool digit(const char* str);
char* strbetween(const char* str, const char startdel, const char enddel);
char* configpath();
char* mostsellingpath();
char* principalfolder();
char* admpath();
char** separatebydelimiter(const char* str, const char* delimiter, int& size);
int countseparatedbydelimiter(const char* str, const char* delimiter);
char* resizestr(const char* originalstr, const char* addstr);
char** resizedoublepointer(char** actual, int* actualsize, const char* newtext);