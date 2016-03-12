#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "common.h"

bool start_with(char * string, char * start) {
	while (1) {
		if (*start == 0) {
			return true;
		} else if (*start != *string) {
			return false;
		}
		string++;
		start++;
	}
}

char * read_line(int sockt) {
	char buffer[BUFFER_SIZE];
	char * line = NULL, * tmp;
	int size, total_size;
	while (1) {
		size = recv(sockt, buffer, BUFFER_SIZE, 0);
		if (size == -1 || size == 0) {
			free(line);
			return NULL;
		}
		if (line == NULL) {
			line = malloc(sizeof(char) * size);
			if (line == NULL) {
				return NULL;
			}
			total_size = size;
		} else {
			total_size += size;
			tmp = realloc(line, sizeof(char) * (total_size));
			if (tmp == NULL) {
				tmp = malloc(sizeof(char) * (total_size));
				if (tmp == NULL) {
					return NULL;
				}
				memcpy(tmp, line, sizeof(char) * (total_size - size));
				free(line);
				line = tmp;
			}
		}
		memcpy(line + total_size - size, buffer, size);
		if (line[total_size-1] == '\0') {
			return line;
		}
	}
}
