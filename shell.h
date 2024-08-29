#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

#define BUFFER_SIZE 1024
#define MAX_ALIASES 100

/**
 * struct alias - a shell alias
 * @name: the name of the alias
 * @value: the value of the alias
 */
typedef struct alias
{
	char *name;
	char *value;
} Alias;

/* Execution */
void execute_command(char *input, char **envp, int *last_exit_status);
void execute_builtin(char **args, int *last_exit_status);
char *find_executable(char *command);
void execute_external_command(char **args, char **envp, int *last_exit_status);
char *search_path(char *command);
char *construct_path(char *dir, char *command, size_t command_len);
void handle_exit(char **args, int *last_exit_status);
void handle_setenv(char **args);
void handle_unsetenv(char **args);
void handle_cd(char **args);
void replace_variable(char **arg, int value);
void expand_env_variable(char **arg);

/* Input */
char *read_input(void);
char **parse_input(char *input);
void expand_variables(char **args);
void print_general_help(void);
void print_command_help(char *command);

/* Environment */
void print_environment(void);
int custom_setenv(const char *name, const char *value, int overwrite);
int custom_unsetenv(const char *name);

/* Utility */
void display_prompt(void);
char *custom_getline(FILE *file);
void free_args(char **args);
void change_directory(char *path);

/* Builtins */
void handle_alias(char **args);
int handle_file_input(const char *filename,
					  char **envp, int *last_exit_status);
int handle_interactive_input(char **envp,
							 int *last_exit_status);
int handle_logical_operators(char *input,
							 char **envp, int *last_exit_status);
void handle_command_separators(char *input,
							   char **envp, int *last_exit_status);
void print_help(char **args);
int is_builtin(char *command);

#endif /* SHELL_H */
