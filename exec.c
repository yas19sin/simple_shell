#include "shell.h"

/**
 * execute_command - Execute a command with arguments.
 *
 * @input: The command and arguments as a string.
 * @envp: The environment variables.
 */
void execute_command(char *input, char **envp)
{
	pid_t child_pid;
	int status;

	char *executable;
	char **args = parse_input(input);

	if (args == NULL || args[0] == NULL)
	{
		free(args);
		return;
	}

	if (strcmp(args[0], "exit") == 0)
	{
		if (args[1] != NULL)
		{
			int exit_status = atoi(args[1]);
			free(args);
			exit(exit_status);
		}
		else
		{
			free(args);
			exit(EXIT_SUCCESS);
		}
	}
	else if (strcmp(args[0], "env") == 0)
	{
		print_environment();
		free(args);
		return;
	}
	else if (strcmp(args[0], "setenv") == 0)
	{
		if (args[1] != NULL && args[2] != NULL && args[3] == NULL)
		{
			if (setenv(args[1], args[2], 1) != 0)
			{
				fprintf(stderr, "setenv: Unable to set environment variable\n");
			}
		}
		else
		{
			fprintf(stderr, "usage: setenv NAME VALUE\n");
		}
	}
	else if (strcmp(args[0], "unsetenv") == 0)
	{
		if (args[1] != NULL && args[2] == NULL)
		{
			if (unsetenv(args[1]) != 0)
			{
				fprintf(stderr, "unsetenv: Unable to unset environment variable\n");
			}
		}
		else
		{
			fprintf(stderr, "usage: unsetenv NAME\n");
		}
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		char *new_directory = (args[1] != NULL) ? args[1] : getenv("HOME");

		if (chdir(new_directory) != 0)
		{
			fprintf(stderr, "cd: %s: No such file or directory\n", new_directory);
		}
		else
		{
			char cwd[PATH_MAX];
			if (getcwd(cwd, sizeof(cwd)) != NULL)
			{
				setenv("PWD", cwd, 1);
			}
		}
	}
	else
	{
		executable = find_executable(args[0]);

		if (executable == NULL)
		{
			fprintf(stderr, "%s: command not found\n", args[0]);
			free(args);
			return;
		}

		child_pid = fork();

		if (child_pid == -1)
		{
			perror("Error forking process");
			exit(EXIT_FAILURE);
		}

		if (child_pid == 0)
		{
			if (execve(executable, args, envp) == -1)
			{
				perror(executable);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			waitpid(child_pid, &status, 0);
		}
	}

	free(args);
}

/**
 * find_executable - Find the full path of an executable in the PATH.
 *
 * @command: The name of the command.
 *
 * Return: The full path of the executable, or NULL if not found.
 */
char *find_executable(char *command)
{
	char *path_copy, *token, *executable_path, *path;

	if (command[0] == '/')
	{
		if (access(command, X_OK) == 0)
		{
			return strdup(command);
		}
		else
		{
			return NULL;
		}
	}

	path = getenv("PATH");
	path_copy = strdup(path);
	token = strtok(path_copy, ":");

	while (token != NULL)
	{
		executable_path = malloc(strlen(token) + strlen(command) + 2);
		if (executable_path == NULL)
		{
			perror("malloc failed");
			exit(EXIT_FAILURE);
		}

		strcpy(executable_path, token);
		strcat(executable_path, "/");
		strcat(executable_path, command);

		if (access(executable_path, X_OK) == 0)
		{
			free(path_copy);
			return executable_path;
		}

		free(executable_path);
		token = strtok(NULL, ":");
	}

	free(path_copy);
	return NULL;
}