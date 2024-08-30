#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define _READ_BUFFER_SIZE 1024
#define _WRITE_BUFFER_SIZE 1024
#define BUF_FLUSH -1

#define CMD_NORMAL 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE ".shell_history"
#define HISTORY_MAX 4096

extern char **environ;

/**
 * struct _liststr - Structure for singly linked list node.
 * @num: The number associated with the node.
 * @str: The string stored in the node.
 * @next: Pointer to the next node.
 */
typedef struct _liststr
{
	int num;
	char *str;
	struct _liststr *next;
} list_t;

/**
 * struct _passinfo - Structure for passing arguments to functions.
 * This allows a uniform prototype for function pointer structs.
 * @arg: String generated from getline containing arguments.
 * @argv: Array of strings generated from arguments.
 * @path: String representing the path for the current command.
 * @argc: Argument count.
 * @line_count: Error count.
 * @err_num: Error code for exit() calls.
 * @linecount_flag: Flag to count this line of input.
 * @fname: The program's filename.
 * @env: Linked list, a local copy of the environment.
 * @history: Linked list for history.
 * @alias: Linked list for alias.
 * @environ: A pointer to the environment variables.
 * @env_changed: Flag to indicate if the environment was changed.
 * @status: Return status of the last executed command.
 * @cmd_buf: Pointer to the command buffer, used in chaining.
 * @cmd_buf_type: Type of command (normal, OR, AND, etc.).
 * @readfd: File descriptor for reading input.
 * @histcount: Line number count for history.
 */
typedef struct _passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf;
	int cmd_buf_type;
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT                                                            \
	{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	 0, 0, 0}

/**
 * struct _builtin - Structure for defining a builtin.
 * command and its related function.
 * @type: The type of the builtin command.
 * @func: The function associated with the command.
 */
typedef struct _builtin
{
	char *type;
	int (*func)(info_t *);
} _builtin_table;

/* Function Declarations */

/* exec.c */
int _unsetenv(info_t *info, char *var);
int _setenv(info_t *info, char *var, char *value);
void _clear_info(info_t *info);
void _set_info(info_t *info, char **av);
void _free_info(info_t *info, int all);

/* exec1.c */
ssize_t _input_buf(info_t *info, char **buf, size_t *len);
ssize_t _get_input(info_t *info);
ssize_t _read_buf(info_t *info, char *buf, size_t *i);
int _getline(info_t *info, char **ptr, size_t *length);
void _sigintHandler(__attribute__((unused)) int sig_num);

/* exec2.c */
char *_get_history_file(info_t *info);
int _write_history(info_t *info);
int read_history(info_t *info);
int _build_history_list(info_t *info, char *buf, int linecount);
int _renumber_history(info_t *info);

/* exec3.c */
list_t *_add_node(list_t **head, const char *str, int num);
list_t *_add_node_end(list_t **head, const char *str, int num);
size_t _print_list_str(const list_t *h);
int _delete_node_at_index(list_t **head, unsigned int index);
void _free_list(list_t **head_ptr);

/* functions.c */
size_t _list_len(const list_t *h);
char **_list_to_strings(list_t *head);
size_t _print_list(const list_t *h);
list_t *_node_starts_with(list_t *node, char *prefix, char c);
ssize_t _get_node_index(list_t *head, list_t *node);

/* functions1.c */
int _bfree(void **_ptr);
int _is_cmd(info_t *info, char *path);
char *_dup_chars(char *pathstr, int start, int stop);
char *_find_path(info_t *info, char *pathstr, char *cmd);
char *_memset(char *_s, char _b, unsigned int _n);

/* functions2.c */
void _ffree(char **_pp);
void *_realloc(void *pnt, unsigned int size_old, unsigned int size_new);
int hsh(info_t *info, char **av);
int _find_builtin(info_t *info);

/* functions3.c */
void _fork_cmd(info_t *info);
int _strlen(char *s);
int _strcmp(char *s1, char *s2);
char *_starts_with(const char *haystack, const char *needle);
char *_strcat(char *dest, char *src);

/* operators.c */
int _putsfd(char *str, int fd);
int _erratoi(char *s);
void _print_error(info_t *info, char *estr);
int _print_d(int input, int fd);
char *_convert_number(long int num, int base, int flags);

/* operators1.c */
void _remove_comments(char *buf);
char *_strncpy(char *_dest, char *_src, int n);
char *_strncat(char *_dest, char *_src, int n);
char *_strchr(char *_s, char _c);
char **_get_environ(info_t *info);

/* operators2.c */
char *_strcpy(char *_dest, char *_src);
char *_strdup(const char *_str);
void _puts(char *_str);
int _putchar(char _c);
char **_strtow(char *string, char *_delim);

/* operators3.c */
char **_strtow2(char *_str, char _d);
int _is_chain(info_t *info, char *buf, size_t *p);
void _check_chain(info_t *info, char *buf,
				  size_t *p, size_t i, size_t len);
int _replace_alias(info_t *info);
int _replace_vars(info_t *info);

/* helper.c */
int _replace_string(char **old, char *new);
void _find_cmd(info_t *info);

/* utils.c */
int _interactive(info_t *info);
int _is_delim(char c, char *delim);
int _isalpha(int c);
int _atoi(char *s);
int _myhelp(info_t *info);

/* utils1.c */
int _myexit(info_t *info);
int _mycd(info_t *info);
int _myhistory(info_t *info);
int _unset_alias(info_t *info, char *str);
int _set_alias(info_t *info, char *str);

/* utils2.c */
int _print_alias(list_t *node);
int _myalias(info_t *info);
int _myenv(info_t *info);
char *_getenv(info_t *info, const char *name);
int _mysetenv(info_t *info);

/* utils3.c */
int _myunsetenv(info_t *info);
int populate_env_list(info_t *info);
void _eputs(char *str);
int _eputchar(char c);
int _putfd(char c, int fd);

#endif /* SHELL_H */
