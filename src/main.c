#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "recdir.h"

size_t findtodo(FILE *f, size_t *lines){
	char c;
	size_t line = 1;
	size_t matches = 0;
	do {
      		c = (char)fgetc(f);
		if(c == '\n') ++line;
		if(c == 'T'){
      			c = (char)fgetc(f);
			if(c == 'O'){
      				c = (char)fgetc(f);
				if(c == 'D'){
      					c = (char)fgetc(f);
					if(c == 'O'){
						*lines++ = line;
						++matches;
					}
				}
			}
		}
    	} while(c != EOF);

	return matches;
}

void printlines(FILE *f, size_t *lines, size_t mat){
	char line[256];
	size_t count = 0;
	fseek(f, 0, SEEK_SET);
        while (fgets(line, sizeof line, f) != NULL && mat > 0)
        {
            	if (count == *lines - 1){
            		printf("%ld %s", *lines, line);
			lines++;
			mat--;
		}
                count++;
        }
}

void printtodo(const char* path){
	FILE *f = fopen(path, "r");

	size_t *linearray = malloc(100 * sizeof(size_t));
	size_t matches = findtodo(f, linearray);
	if (matches > 0) printf("%s\n", path);
	printlines(f, linearray, matches);

	free(linearray);
	fclose(f);
}

int main(int argc, char **argv){
	(void) argc;
	RECDIR *recdir = recdir_open(argv[1]);

	errno = 0;
	struct dirent *ent = recdir_read(recdir);
	while(ent){
		char *path = join_path(recdir_top(recdir)->path, ent->d_name);
		printtodo(path);
		free(path);
		ent = recdir_read(recdir);
	}

	if(errno != 0){
		fprintf(stderr, "ERROR: could not read the dir: %s\n",  recdir_top(recdir)->path);
		exit(1);
	}

	recdir_close(recdir);
	return 0;
}
