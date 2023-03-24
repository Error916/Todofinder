#ifndef TODOTYPE_H_
#define TODOTYPE_H_

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

TODOS *TODOS_Init(void);
void TODOS_Destroy(TODOS *todos);
void TODOS_Add(TODOS *todos, TODO_Frame *match);
size_t TODOS_Gen(TODOS *todos, const char *path, const char *string);

#endif //TODOTYPE_H_
