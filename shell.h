#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

typedef struct Alias {
	char *name;
	char *value;
} Alias;


void display_prompt(void);
char *read_input(void);
char **parse_input(char *input);
void print_environment();
char *custom_getline(FILE *file);

void execute_command(char *input, char **envp);
char *find_executable(char *command);
int custom_asprintf(char **strp, const char *fmt, ...);

#endif /* SHELL_H */