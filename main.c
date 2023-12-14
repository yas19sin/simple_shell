#include "shell.h"

/**
 * main - Entry point for the shell program.
 *
 * Return: Always 0.
 */
 int main(void)
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

		execute_command(input, NULL);

		free(input);
	}

	return 0;
}