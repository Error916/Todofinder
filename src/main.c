#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "recdir.h"
#include "todotype.h"

int main(int argc, char **argv){
	char* pathexe;
        if(argc == 2){
                pathexe = argv[1];
        } else {
                fprintf(stderr, "ERROR: use %s [path]\n", argv[0]);
                exit(1);
        }
	RECDIR *recdir = recdir_open(pathexe);

	TODOS *todos = TODOS_Init();

	errno = 0;
	struct dirent *ent = recdir_read(recdir);
	while(ent){
		char *path = join_path(recdir_top(recdir)->path, ent->d_name);
		TODOS_Gen(todos, path);
		free(path);
		ent = recdir_read(recdir);
	}

	if(errno != 0){
		fprintf(stderr, "ERROR: could not read the dir: %s\n",  recdir_top(recdir)->path);
		exit(1);
	}

	recdir_close(recdir);

	for(size_t i = 0; i < todos->pos; ++i){
		printf("%ld %ld %s %s", todos->array[i]->line, todos->array[i]->priority, todos->array[i]->path, todos->array[i]->message);
	}

	TODOS_Destroy(todos);

	return 0;
}
