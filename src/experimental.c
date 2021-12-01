#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "recdir.h"

#define TODO_INIT_SIZE 32

typedef struct {
	char* path;
	size_t line;
	size_t priority;
	char* message;
} TODO_Frame;

typedef struct {
	TODO_Frame **array;
	size_t array_size;
	size_t pos;
} TODOS;

TODOS *TODOS_Init(){
	TODOS *todos = malloc(sizeof(TODOS));
	todos->array = calloc(TODO_INIT_SIZE, sizeof(TODO_Frame*));
	todos->array_size = TODO_INIT_SIZE;
	todos->pos = 0;

	return todos;
}

void TODOS_Destroy(TODOS *todos){
	for(size_t i = 0; i < todos->pos; ++i){
		free(todos->array[i]->message);
		free(todos->array[i]->path);
		free(todos->array[i]);
	}
	free(todos->array);
	free(todos);
}

void TODOS_Add(TODOS *todos, TODO_Frame *match){
	if(todos->pos + 1 > todos->array_size){
		todos->array_size *= 2;
		todos->array = realloc(todos->array, todos->array_size * sizeof(TODO_Frame*));
	}

	todos->array[todos->pos] = match;
	todos->pos++;
}

size_t TODOS_Gen(TODOS *todos, char *path){
	FILE *f =  fopen(path, "r");

	size_t line = 0;
	size_t todo_in_file = 0;
	char c;
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
						const size_t mess_size = 256;
						char *mess = malloc(mess_size * sizeof(char));

						size_t priority = 1;
						while((char)fgetc(f) == 'O') priority++;
						fseek(f, -(4 + priority), SEEK_CUR);
						fgets(mess, mess_size * sizeof(char), f);

						TODO_Frame *match = malloc(sizeof(TODO_Frame));
                        			match->line = line + 1;
						match->priority = priority;
						match->path = strdup(path);
						match->message = mess;

						TODOS_Add(todos, match);
						todo_in_file++;
                    			}
                		}
            		}
        	}
        } while(c != EOF);

	fclose(f);

	return todo_in_file;
}

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
