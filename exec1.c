#include "shell.h"

/**
 * is_builtin - Check if a command is a shell builtin
 *
 * This function takes a command name as an argument and checks if it is a
 * shell builtin. If the command is a builtin, the function returns 1,
 * otherwise it returns 0.
 *
 * @command: The command name to check
 * Return: 1 if the command is a builtin, 0 otherwise
 */
int is_builtin(char *command)
{
	char *builtins[] = {
		"exit",
		"env",
		"setenv",
		"unsetenv",
		"cd",
		"alias",
		"help",
		NULL};
	int i;

	for (i = 0; builtins[i]; i++)
	{
		if (strcmp(command, builtins[i]) == 0)
		{
			return (1);
		}
	}
	return (0);
}

/**
 * execute_builtin - Execute a built-in command
 *
 * This function is called when the user's command is a built-in command. It
 * calls the appropriate function to handle the command.
 *
 * @args: The arguments passed to the command
 * @last_exit_status: Pointer to the last exit status
 */
void execute_builtin(char **args, int *last_exit_status)
{
	if (strcmp(args[0], "exit") == 0)
	{
		handle_exit(args, last_exit_status);
	}
	else if (strcmp(args[0], "env") == 0)
	{
		print_environment();
		*last_exit_status = 0;
	}
	else if (strcmp(args[0], "setenv") == 0)
	{
		handle_setenv(args, last_exit_status);
	}
	else if (strcmp(args[0], "unsetenv") == 0)
	{
		handle_unsetenv(args, last_exit_status);
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		handle_cd(args, last_exit_status);
	}
	else if (strcmp(args[0], "alias") == 0)
	{
		handle_alias_builtin(args);
		*last_exit_status = 0;
	}
	else if (strcmp(args[0], "help") == 0)
	{
		print_help(args);
		*last_exit_status = 0;
	}
	else
	{
		fprintf(stderr, "Error: Command not recognized\n");
		*last_exit_status = 1;
	}
}

/**
 * handle_exit - Handle the exit builtin command
 *
 * @args: The command arguments
 * @last_exit_status: Pointer to the last exit status
 */
void handle_exit(char **args, int *last_exit_status)
{
	int status = 0;

	if (args[1] != NULL)
	{
		char *endptr;
		long int num = strtol(args[1], &endptr, 10);

		if (*endptr != '\0' || num > INT_MAX || num < INT_MIN)
		{
			fprintf(stderr, "exit: Illegal number: %s\n", args[1]);
			*last_exit_status = 2;
			return;
		}

		status = (int)num;
	}

	*last_exit_status = status;
	exit(*last_exit_status);
}

/**
 * handle_setenv - Handle the setenv builtin command
 *
 * @args: The command arguments
 * @last_exit_status: Pointer to the last exit status
 */
void handle_setenv(char **args, int *last_exit_status)
{
	if (args[1] == NULL || args[2] == NULL)
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		*last_exit_status = 1;
		return;
	}

	if (custom_setenv(args[1], args[2], 1) != 0)
	{
		perror("setenv");
		*last_exit_status = 1;
	}
	else
	{
		*last_exit_status = 0;
	}
}

/**
 * handle_unsetenv - Handle the unsetenv builtin command
 *
 * @args: The command arguments
 * @last_exit_status: Pointer to the last exit status
 */
void handle_unsetenv(char **args, int *last_exit_status)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		*last_exit_status = 1;
		return;
	}

	if (custom_unsetenv(args[1]) != 0)
	{
		perror("unsetenv");
		*last_exit_status = 1;
	}
	else
	{
		*last_exit_status = 0;
	}
}
