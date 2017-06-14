#include "utility.h"

void printInfo(char * format, ...) {
	va_list vl;
	va_start(vl, format);
	vfprintf(stderr, format, vl);
	va_end(vl);
}
