#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "todotype.h"

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

	int i;
	for(i = todos->pos; i >= 0; i--){
		if( todos->array[i] == NULL || todos->array[i]->priority > match->priority) break;
		todos->array[i+1] = todos->array[i];
	}

	todos->array[i] = match;
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
