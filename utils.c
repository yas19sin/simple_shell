#include "shell.h"

/**
 * read_input - Read a line of input from stdin.
 *
 * Return: The input line as a string, or NULL on failure.
 */
char *read_input(void)
{
	char *input = NULL;
	size_t bufsize = 0;
	ssize_t characters = getline(&input, &bufsize, stdin);

	if (characters == -1)
	{
		free(input);
		return NULL;
	}

	if (input[characters - 1] == '\n')
	{
		input[characters - 1] = '\0';
	}

	return input;
}

/**
 * custom_getline - Read a line of input from stdin using a static buffer.
 *
 * @file: the file.
 *
 * Return: The input line as a string, or NULL on failure.
 */
char *custom_getline(FILE *file)
{
	static char buffer[BUFFER_SIZE];
	static ssize_t buffer_index = 0;
	static ssize_t buffer_size = 0;
	char *line = NULL;
	size_t line_size = 0;

	while (1)
	{
		if (buffer_index == buffer_size)
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
		line[line_size] = buffer[buffer_index];
		line_size++;
		buffer_index++;
	}

	return line;
}

/**
 * display_prompt - Display the shell prompt.
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("($) ");
		fflush(stdout);
	}
}

/**
 * parse_input - Parse the input string into an array of arguments.
 *
 * @input: The input string to parse.
 *
 * Return: An array of arguments (strings).
 */
char **parse_input(char *input)
{
	char **args = NULL;
	int arg_count = 0;

	int input_length = strlen(input);
	int token_start = -1;
	int i;
	int inside_quotes = 0;

	for (i = 0; i <= input_length; i++)
	{
		if ((input[i] == ' ' || input[i] == '\0' || input[i] == '\t' || input[i] == '\n') && !inside_quotes)
		{
			if (token_start != -1)
			{
				int token_length = i - token_start;
				args = realloc(args, (arg_count + 1) * sizeof(char *));
				if (args == NULL)
				{
					perror("realloc failed");
					exit(EXIT_FAILURE);
				}

				args[arg_count] = malloc(token_length + 1);
				if (args[arg_count] == NULL)
				{
					perror("malloc failed");
					exit(EXIT_FAILURE);
				}

				strncpy(args[arg_count], input + token_start, token_length);
				args[arg_count][token_length] = '\0';
				arg_count++;

				token_start = -1;
			}
		}
		else if (input[i] == '\'' || input[i] == '\"')
		{
			inside_quotes = !inside_quotes;
			if (token_start == -1)
			{
				token_start = i + 1;
			}
		}
		else if (input[i] == '#' && !inside_quotes)
		{
			break;
		}
		else if (token_start == -1)
		{
			token_start = i;
		}
	}

	args = realloc(args, (arg_count + 1) * sizeof(char *));
	if (args == NULL)
	{
		perror("realloc failed");
		exit(EXIT_FAILURE);
	}

	args[arg_count] = NULL;

	return args;
}

/**
 * print_environment - Print the environment variables.
 */
void print_environment(void)
{
	extern char **environ;
	int i = 0;

	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i]);
		i++;
	}
}