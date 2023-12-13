#include "shell.h"

/**
 * execute_command - Executes the given command line.
 * @line: The command line to be executed.
 */
void execute_command(char *line)
{
	pid_t child_pid;
	int status;
	char **argv = tokenize_line(line);
	struct stat st;

	if (argv == NULL)
	{
		perror("Error");
		return;
	}

	if (strcmp(argv[0], "exit") == 0)
	{
		int exit_status = (argv[1] != NULL) ? atoi(argv[1]) : EXIT_SUCCESS;
		free_argv(argv);
		exit(exit_status);
	}
	else if (strcmp(argv[0], "env") == 0)
	{
		print_environment();
	}
	else if (strcmp(argv[0], "setenv") == 0)
	{
		if (argv[1] != NULL && argv[2] != NULL)
		{
			if (setenv(argv[1], argv[2], 1) == -1)
			{
				perror("Error");
			}
		}
		else
		{
			fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		}
	}
	else if (strcmp(argv[0], "unsetenv") == 0)
	{
		if (argv[1] != NULL)
		{
			if (unsetenv(argv[1]) == -1)
			{
				perror("Error");
			}
		}
		else
		{
			fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		}
	}
	else if (strcmp(argv[0], "cd") == 0)
	{
		char *new_dir = (argv[1] != NULL) ? argv[1] : getenv("HOME");

		if (chdir(new_dir) == -1)
		{
			perror("Error");
		}
		else
		{
			char *cwd = getcwd(NULL, 0);
			if (cwd != NULL)
			{
				setenv("PWD", cwd, 1);
				free(cwd);
			}
			else
			{
				perror("Error");
			}
		}
	}
	else if (!find_command(argv[0]))
	{
		fprintf(stderr, "%s: command not found\n", argv[0]);
	}
	else if (stat(argv[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "%s: is a directory\n", argv[0]);
	}
	else
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Error");
			free_argv(argv);
			exit(EXIT_FAILURE);
		}

		if (child_pid == 0)
		{
			/* Child process */
			if (execvp(argv[0], argv) == -1)
			{
				perror("Error");
				fprintf(stderr, "Failed to execute command: %s\n", argv[0]);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			/* Parent process */
			waitpid(child_pid, &status, 0);
		}
	}

	free_argv(argv);
}

/**
 * free_argv - Frees the memory allocated for the argument vector.
 * @argv: The argument vector to be freed.
 */
void free_argv(char **argv)
{
	int i = 0;

	while (argv[i] != NULL)
	{
		free(argv[i]);
		i++;
	}

	free(argv);
}

/**
 * tokenize_line - Tokenizes the given command line.
 * @line: The command line to be tokenized.
 * Return: An array of tokens.
 */
char **tokenize_line(char *line)
{
	char **tokens = malloc(MAX_ARGS * sizeof(char *));
	char *token;
	int i = 0;

	if (tokens == NULL)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}

	token = custom_strtok(line, " \t\n");
	while (token != NULL)
	{
		tokens[i++] = strdup(token);
		token = custom_strtok(NULL, " \t\n");

		if (i >= MAX_ARGS - 1)
			break;
	}

	tokens[i] = NULL;

	return tokens;
}

/**
 * find_command - Checks if the given command exists in the system.
 * @command: The command to be checked.
 * Return: 1 if the command is found, 0 otherwise.
 */
int find_command(char *command)
{
	char *path;
	char *path_copy;
	char *token;

	if (command == NULL || command[0] == '\0')
		return 0;

	if (strchr(command, '/') != NULL && access(command, F_OK) == 0)
		return 1;

	path = getenv("PATH");
	path_copy = strdup(path);
	if (path_copy == NULL)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}

	token = custom_strtok(path_copy, ":");

	while (token != NULL)
	{
		char full_path[1024];
		snprintf(full_path, sizeof(full_path), "%s/%s", token, command);

		if (access(full_path, F_OK) == 0)
		{
			free(path_copy);
			return 1;
		}

		token = custom_strtok(NULL, ":");
	}

	free(path_copy);
	return 0;
}

/**
 * print_environment - Prints the current environment variables.
 */
void print_environment()
{
	extern char **environ;
	int i = 0;

	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i]);
		i++;
	}
}
