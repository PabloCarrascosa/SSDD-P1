#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "claves.h"

int main(void) {
    init();
	char hola[256];
	sprintf(hola, "pepito grillo");
	int entero = 2;
	double dec = 2.34;
	set_value(1, hola, entero, dec);
}

