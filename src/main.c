#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include "recdir.h"
#include "todotype.h"

int main(int argc, char **argv){
	int s, opt;
	ssize_t num_threads;
	char* pathexe;

	pathexe = ".";
	num_threads = 4;

	while ((opt = getopt(argc, argv, "p:t:")) != -1) {
    		switch (opt) {
    		case 't':
        		num_threads = strtoul(optarg, NULL, 0);
        		break;

    		case 'p':
        		pathexe = optarg;
        		break;

    		default:
        		fprintf(stderr, "Usage: %s [-t num_threads] arg...\n", argv[0]);
        		exit(EXIT_FAILURE);
    		}
	}

	RECDIR *recdir = recdir_open(pathexe);

	TODOS *todos = TODOS_Init();
	TODOS *fixmes = TODOS_Init();

	errno = 0;
	struct dirent *ent = recdir_read(recdir);
	while(ent){
		char *path = join_path(recdir_top(recdir)->path, ent->d_name);
		TODOS_Gen(todos, path, "TODO");
		TODOS_Gen(fixmes, path, "FIXME");
		free(path);
		ent = recdir_read(recdir);
	}

	if(errno != 0){
		fprintf(stderr, "ERROR: could not read the dir: %s\n",  recdir_top(recdir)->path);
		exit(1);
	}

	recdir_close(recdir);

	printf("+----+------+----------------------+\n");
	printf("| -> | line | %-20s | message\n", "file path");
	printf("+----+------+----------------------+\n");

	for(size_t i = 0; i < todos->pos; ++i){
		printf("| %-2ld | %-4ld | %-20s | %s", todos->array[i]->priority, todos->array[i]->line, todos->array[i]->path, todos->array[i]->message);
	}

	printf("+----+------+----------------------+\n");

	for(size_t i = 0; i < fixmes->pos; ++i){
		printf("| %-2ld | %-4ld | %-20s | %s", fixmes->array[i]->priority, fixmes->array[i]->line, fixmes->array[i]->path, fixmes->array[i]->message);
	}

	printf("+----+------+----------------------+\n");

	TODOS_Destroy(todos);
	TODOS_Destroy(fixmes);

	return 0;
}
