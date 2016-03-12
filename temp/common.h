
#ifndef COMMON_H

#define	COMMON_H

#include <stdbool.h>

#define SERVER_PORT 1500

#define BUFFER_SIZE 128

char * read_line(int sockt);

bool start_with(char * string, char * start);

#endif