/* shell.h */
#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMANDS 100
#define MAX_ARGS 64
#define BUFFER_SIZE 1024

void execute_command(char *line);
void free_argv(char **argv);
char **tokenize_line(char *line);
int find_command(char *command);
void print_environment();

char *custom_getline(void);
char *custom_strtok(char *str, const char *delimiter);

#endif /* SHELL_H */
