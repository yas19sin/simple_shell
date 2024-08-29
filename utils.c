#include "shell.h"

/**
 * display_prompt - Display a prompt to the user
 *
 * This function displays a prompt to the user when the shell is running in
 * interactive mode. If the shell is not running in interactive mode, this
 * function does nothing.
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("$ ");
		fflush(stdout);
	}
}

/**
 * read_input - Read a line of input from the user
 *
 * This function reads a line of input from the user and returns the line as a
 * string. If the user enters an empty line, the function returns NULL.
 *
 * Return: The line entered by the user, or NULL if the user entered an empty
 * line.
 */
char *read_input(void)
{
	char *input = NULL;
	size_t bufsize = 0;
	ssize_t characters = getline(&input, &bufsize, stdin);

	if (characters == -1)
	{
		free(input);
		return (NULL);
	}

	if (input[characters - 1] == '\n')
	{
		input[characters - 1] = '\0';
	}

	return (input);
}

/**
 * custom_getline - Read a line of input from a file
 *
 * This function reads a line of input from a file and returns the line as a
 * string. If the end of the file is reached, the function returns NULL.
 *
 * @file: The file to read from
 * Return: The line read from the file, or NULL if the end of the file was
 * reached.
 */
char *custom_getline(FILE *file)
{
	static char buffer[BUFFER_SIZE];
	static size_t buffer_index;
	static size_t buffer_size;
	char *line = NULL;
	size_t line_size = 0;

	while (1)
	{
		if (buffer_index >= buffer_size)
		{
			buffer_size = fread(buffer, 1, BUFFER_SIZE, file);
			if (buffer_size == 0)
			{
				break;
			}
			buffer_index = 0;
		}

		if (buffer[buffer_index] == '\n' || buffer[buffer_index] == '\0')
		{
			line = realloc(line, line_size + 1);
			if (line == NULL)
			{
				perror("realloc failed");
				exit(EXIT_FAILURE);
			}
			line[line_size] = '\0';
			buffer_index++;
			break;
		}

		line = realloc(line, line_size + 1);
		if (line == NULL)
		{
			perror("realloc failed");
			exit(EXIT_FAILURE);
		}
		line[line_size++] = buffer[buffer_index++];
	}
	return (line);
}

/**
 * parse_input - Parse a line of input into individual arguments
 *
 * This function takes a line of input and splits it into individual arguments.
 * It handles quoted strings and ignores everything after a '#' character.
 *
 * @input: The line of input to parse
 *
 * Return: An array of strings, where each string is an argument. The last
 * element of the array is always NULL.
 */
char **parse_input(char *input)
{
	char **args = NULL, *token, *saveptr;
	int arg_count = 0, in_quotes = 0, i;

	for (token = strtok_r(input, " \t\n", &saveptr);
		 token != NULL; token = strtok_r(NULL, " \t\n", &saveptr))
	{
		if (token[0] == '#' && !in_quotes)
		{
			break;
		}
		for (i = 0; token[i]; i++)
		{
			if (token[i] == '"')
			{
				in_quotes = !in_quotes;
			}
		}
		args = realloc(args, (arg_count + 1) * sizeof(char *));

		if (args == NULL)
		{
			perror("realloc failed");
			exit(EXIT_FAILURE);
		}
		args[arg_count] = strdup(token);
		if (args[arg_count] == NULL)
		{
			perror("strdup failed");
			exit(EXIT_FAILURE);
		}
		arg_count++;
	}
	args = realloc(args, (arg_count + 1) * sizeof(char *));
	if (args == NULL)
	{
		perror("realloc failed");
		exit(EXIT_FAILURE);
	}
	args[arg_count] = NULL;
	return (args);
}

/**
 * free_args - Free the memory allocated for the arguments
 *
 * This function takes an array of strings as an argument and frees the memory
 * allocated for each string in the array.
 * It then frees the memory allocated for
 * the array itself.
 *
 * @args: The array of strings to free
 */
void free_args(char **args)
{
	if (args)
	{
		int i;

		for (i = 0; args[i]; i++)
		{
			free(args[i]);
		}
		free(args);
	}
}
