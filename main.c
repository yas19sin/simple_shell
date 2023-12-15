#include "shell.h"

/**
 * main - Entry point for the shell program.
 *
 * Return: Always 0.
 */
int main(int argc, char *argv[])
{
	char *input;

	if (argc > 1)
	{
		FILE *file = fopen(argv[1], "r");
		if (file == NULL)
		{
			perror("Error opening file");
			return EXIT_FAILURE;
		}

		while ((input = custom_getline(file)) != NULL)
		{
			execute_command(input, NULL);
			free(input);
		}

		fclose(file);
		return EXIT_SUCCESS;
	}
	else
	{
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

		return EXIT_SUCCESS;
	}
}