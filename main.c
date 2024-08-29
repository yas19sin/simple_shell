#include "shell.h"

/**
 * main - The main entry point for the shell
 * @argc: The number of command line arguments
 * @argv: The command line arguments
 * @envp: The environment variables
 *
 * This function is called when the program starts. It checks if a file name
 * is passed as an argument and if so, reads commands from the file. If no
 * filename is given, it reads commands from the standard input.
 *
 * Return: The exit status of the shell
 */
int main(int argc, char *argv[], char **envp)
{
	int last_exit_status = 0;

	if (argc > 1)
	{
		return (handle_file_input(argv[1], envp, &last_exit_status));
	}
	else
	{
		return (handle_interactive_input(envp, &last_exit_status));
	}
}

/**
 * handle_file_input - Process commands from a file
 * @filename: The name of the file to read commands from
 * @envp: The environment variables
 * @last_exit_status: The exit status of the previous command
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int handle_file_input(const char *filename, char **envp, int *last_exit_status)
{
	FILE *file = fopen(filename, "r");
	char *input;

	if (file == NULL)
	{
		perror("Error opening file");
		return (EXIT_FAILURE);
	}

	while ((input = custom_getline(file)) != NULL)
	{
		handle_command_separators(input, envp, last_exit_status);
		free(input);
	}

	fclose(file);
	return (EXIT_SUCCESS);
}

/**
 * handle_interactive_input - Process commands from standard input
 * @envp: The environment variables
 * @last_exit_status: The exit status of the previous command
 *
 * Return: EXIT_SUCCESS
 */
int handle_interactive_input(char **envp, int *last_exit_status)
{
	char *input;

	while (1)
	{
		display_prompt();
		input = read_input();
		if (input == NULL)
		{
			printf("\n");
			break;
		}
		handle_command_separators(input, envp, last_exit_status);
		free(input);
	}

	return (EXIT_SUCCESS);
}

/**
 * print_command_help - Print help for a command
 * @command: The name of the command
 *
 * This function prints help for a command if it is a built-in command and
 * if the command has a help message defined. If the command is not a built-in
 * command or it does not have a help message, it prints the shell's help
 * message.
 */
void print_command_help(char *command)
{
	static char *help_messages[] = {
		"exit: exit [n]\nExit the shell.\n\t[n]: the exit status\n",
		"env: env\nPrint the environment variables.\n",
		"setenv: setenv [VARIABLE] [VALUE]\n"
		"Set an environment variable.\n\t[VARIABLE]: the variable to set\n\t[VALUE]: the value of the variable\n",
		"unsetenv: unsetenv [VARIABLE]\n"
		"Unset an environment variable.\n\t[VARIABLE]: the variable to unset\n",
		"cd: cd [DIRECTORY]\n"
		"Change the working directory.\n\t[DIRECTORY]: the directory to change to\n",
		"alias: alias [NAME] [VALUE]\n"
		"Set an alias.\n\t[NAME]: the name of the alias\n\t[VALUE]: the value of the alias\n",
		"help: help [COMMAND]\n"
		"Print help for a command or all.\n\t[arg]: the command to print help\n"};
	static char *commands[] = {
		"exit", "env", "setenv", "unsetenv", "cd", "alias", "help", NULL};
	int i;

	for (i = 0; commands[i]; i++)
	{
		if (strcmp(command, commands[i]) == 0)
		{
			printf("%s", help_messages[i]);
			return;
		}
	}
	print_general_help();
}
