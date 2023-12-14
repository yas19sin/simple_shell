#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

#define HISTORY_SIZE 10
#define BUFFER_SIZE 1024

void display_prompt(void);
char *read_input(void);
char **parse_input(char *input);
void print_environment();
char *custom_getline(void);

void execute_command(char *input, char **envp);
char *find_executable(char *command);
void add_to_history(char *command);
void display_history(void);

#endif /* SHELL_H */