#include "shell.h"

/**
 * handle_logical_operators - Handle logical operators (&& and ||)
 *
 * This function handles logical operators in the input. It tokenizes the
 * input by the logical operators and executes the commands on either side of
 * the operator. If the operator is &&, the second command is only executed if
 * the first command's exit status is 0. If the operator is ||, the second
 * command is only executed if the first command's exit status is not 0.
 *
 * @input: The command line input with logical operators
 * @envp: The environment variables
 * @last_exit_status: Pointer to the last exit status
 *
 * Return: The exit status of the last command executed
 */
int handle_logical_operators(char *input, char **envp, int *last_exit_status)
{
	char *token, *saveptr;
	int *prev_status = 0;
	int operators = 0; /* 0: none, 1: &&, 2: || */

	token = strtok_r(input, "&|", &saveptr);
	while (token != NULL)
	{
		char *trimmed = token;

		while (*trimmed == ' ')
			trimmed++;

		if (operators == 0 ||
			(operators == 1 && prev_status == 0) ||
			(operators == 2 && prev_status != 0))
		{
			execute_command(trimmed, envp, last_exit_status);
			prev_status = last_exit_status;
		}

		if (saveptr[0] == '&' && saveptr[1] == '&')
		{
			operators = 1;
			saveptr += 2;
		}
		else if (saveptr[0] == '|' && saveptr[1] == '|')
		{
			operators = 2;
			saveptr += 2;
		}
		else
		{
			operators = 0;
		}

		token = strtok_r(NULL, "&|", &saveptr);
	}

	return (*prev_status);
}

/**
 * handle_command_separators - Handle command separators (&&, ||, and ;)
 *
 * This function handles command separators in the input. It tokenizes the
 * input by the command separators and executes the commands on either side of
 * the separator. If the separator is &&, the second command is only executed
 * if the first command's exit status is 0. If the separator is ||, the second
 * command is only executed if the first command's exit status is not 0. If the
 * separator is ;, the second command is always executed.
 *
 * @input: The command line input with logical operators
 * @envp: The environment variables
 * @last_exit_status: Pointer to the last exit status
 */
void handle_command_separators(char *input, char **envp, int *last_exit_status)
{
	char *token, *saveptr;

	token = strtok_r(input, ";", &saveptr);

	while (token != NULL)
	{
		char *trimmed = token;

		while (*trimmed == ' ')
			trimmed++;

		handle_logical_operators(trimmed, envp, last_exit_status);

		token = strtok_r(NULL, ";", &saveptr);
	}
}
