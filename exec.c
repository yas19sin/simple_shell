#include "shell.h"

Alias aliases[100];
size_t alias_count = 0;
int last_exit_status = 0;

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
	int ar;

	char *replacement;
	char *executable;
	char **args = parse_input(input);

	if (args == NULL || args[0] == NULL)
	{
		free(args);
		return;
	}
	
	for (ar = 0; args[ar] != NULL; ar++)
	{
		replacement = NULL;
		if (args[ar][0] == '$')
		{
			char *variable_name = args[ar] + 1;
			char *env_value = getenv(variable_name);
			if (env_value != NULL)
			{
				replacement = strdup(env_value);
			}
		}
		else if (strcmp(args[ar], "$?") == 0)
		{
			custom_asprintf(&replacement, "%d", last_exit_status);
		}
		else if (strcmp(args[ar], "$$") == 0)
		{
			custom_asprintf(&replacement, "%d", getpid());
		}

		if (replacement != NULL)
		{
			free(args[ar]);
			args[ar] = replacement;
		}
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
			exit(last_exit_status);
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
	else if (strcmp(args[0], "alias") == 0)
	{
		if (args[1] == NULL)
		{
			size_t i;
			for (i = 0; i < alias_count; i++)
			{
				printf("%s='%s'\n", aliases[i].name, aliases[i].value);
			}
		}
		else
		{
			int i;
			for (i = 1; args[i] != NULL; i++)
			{
				if (strchr(args[i], '=') != NULL)
				{
					char *name = strtok(args[i], "=");
					char *value = strtok(NULL, "=");
					size_t j;

					for (j = 0; j < alias_count; j++)
					{
						if (strcmp(aliases[j].name, name) == 0)
						{
							free(aliases[j].value);
							aliases[j].value = strdup(value);
							break;
						}
					}

					if (alias_count < sizeof(aliases) / sizeof(aliases[0]))
					{
						aliases[alias_count].name = strdup(name);
						aliases[alias_count].value = strdup(value);
						alias_count++;
					}
					else
					{
						fprintf(stderr, "alias: too many aliases\n");
					}
				}
				else
				{
					int found = 0;
					size_t j;
					for (j = 0; j < alias_count; j++)
					{
						if (strcmp(aliases[j].name, args[i]) == 0)
						{
							printf("%s='%s'\n", aliases[j].name, aliases[j].value);
							found = 1;
							break;
						}
					}
					if (!found)
					{
						fprintf(stderr, "alias: %s not found\n", args[i]);
					}
				}
			}
		}
	}
	else
	{
		char *replacement;
		int i;
		for (i = 0; args[i] != NULL; i++)
		{
			replacement = NULL;
			if (strcmp(args[i], "$?") == 0)
			{
				custom_asprintf(&replacement, "%d", last_exit_status);
			}
			else if (strcmp(args[i], "$$") == 0)
			{
				custom_asprintf(&replacement, "%d", getpid());
			}

			if (replacement != NULL)
			{
				free(args[i]);
				args[i] = replacement;
			}
		}

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
			last_exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
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

/**
 * custom_asprintf - The custom asprintf.
 *
 * @strp: The first argument.
 * @fmt: The second argument.
 * @...: The third argument.
 *
 * Return: len or -1.
 */
int custom_asprintf(char **strp, const char *fmt, ...)
{
	int len;

	va_list args;
	va_start(args, fmt);
	len = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	if (len < 0)
	{
		return -1;
	}

	*strp = malloc(len + 1);
	if (*strp == NULL)
	{
		return -1;
	}

	va_start(args, fmt);
	vsnprintf(*strp, len + 1, fmt, args);
	va_end(args);

	return len;
}