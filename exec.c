#include "shell.h"

/**
 * execute_command - Execute a command
 *
 * This function is called by the shell when a command is read from the user.
 * It parses the command, checks if it is a built-in command and if not, forks
 * a new process to execute the command.
 *
 * @input: The command to execute
 * @envp: The shell's environment variables
 * @last_exit_status: The last exit status
 */
void execute_command(char *input, char **envp, int *last_exit_status)
{
	char **args = parse_input(input);

	if (args == NULL || args[0] == NULL)
	{
		free_args(args);
		return;
	}

	expand_variables(args);

	if (is_builtin(args[0]))
	{
		execute_builtin(args, last_exit_status);
	}
	else
	{
		execute_external_command(args, envp, last_exit_status);
	}

	free_args(args);
}

/**
 * execute_external_command - Execute an external command
 *
 * This function forks a new process to execute an external command.
 *
 * @args: The command and its arguments
 * @envp: The shell's environment variables
 * @last_exit_status: The last exit status
 */
void execute_external_command(char **args, char **envp, int *last_exit_status)
{
	pid_t child_pid;
	int status;
	char *executable = find_executable(args[0]);

	if (executable == NULL)
	{
		fprintf(stderr, "%s: command not found\n", args[0]);
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
		*last_exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
	}

	free(executable);
}

/**
 * find_executable - Find the executable associated with a command
 *
 * This function takes a command name and returns the path to the executable
 * associated with that command. If the command name contains a '/', it is
 * assumed to be a path to the executable. Otherwise, the function searches the
 * directories listed in the $PATH environment variable for an executable with
 * the given name.
 *
 * @command: The command name or path
 *
 * Return: The path to the executable, or NULL if the command was not found
 */
char *find_executable(char *command)
{
	if (strchr(command, '/') != NULL)
	{
		return (access(command, X_OK) == 0 ? strdup(command) : NULL);
	}

	return (search_path(command));
}

/**
 * search_path - Search for an executable in the PATH
 *
 * This function searches the directories listed in the PATH environment
 * variable for an executable with the given name.
 *
 * @command: The command name to search for
 *
 * Return: The full path to the executable, or NULL if not found
 */
char *search_path(char *command)
{
	char *path, *path_copy, *dir, *executable;
	size_t command_len = strlen(command);

	path = getenv("PATH");
	if (path == NULL)
	{
		return (NULL);
	}

	path_copy = strdup(path);
	dir = strtok(path_copy, ":");

	while (dir != NULL)
	{
		executable = construct_path(dir, command, command_len);
		if (executable != NULL)
		{
			free(path_copy);
			return (executable);
		}
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

/**
 * construct_path - Construct a full path to a potential executable
 *
 * This function constructs a full path by combining a directory path
 * and a command name, then checks if the resulting path is executable.
 *
 * @dir: The directory path
 * @command: The command name
 * @command_len: The length of the command name
 *
 * Return: The full path if executable, NULL otherwise
 */
char *construct_path(char *dir, char *command, size_t command_len)
{
	size_t dir_len = strlen(dir);
	char *executable = malloc(dir_len + command_len + 2);

	if (executable == NULL)
	{
		perror("malloc failed");
		return (NULL);
	}

	strcpy(executable, dir);
	strcat(executable, "/");
	strcat(executable, command);

	if (access(executable, X_OK) == 0)
	{
		return (executable);
	}

	free(executable);
	return (NULL);
}
