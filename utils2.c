#include "shell.h"

/**
 * print_aliases - Print all aliases
 * @aliases: Array of aliases
 * @alias_count: Number of aliases
 */
void print_aliases(Alias *aliases, size_t alias_count)
{
	size_t i;

	for (i = 0; i < alias_count; i++)
	{
		printf("%s='%s'\n", aliases[i].name, aliases[i].value);
	}
}

/**
 * set_alias - Set or update an alias
 * @aliases: Array of aliases
 * @alias_count: Pointer to the number of aliases
 * @name: Name of the alias
 * @value: Value of the alias
 */
void set_alias(Alias *aliases, size_t *alias_count, char *name, char *value)
{
	size_t i;

	for (i = 0; i < *alias_count; i++)
	{
		if (strcmp(aliases[i].name, name) == 0)
		{
			free(aliases[i].value);
			aliases[i].value = strdup(value);
			return;
		}
	}

	if (*alias_count < MAX_ALIASES)
	{
		aliases[*alias_count].name = strdup(name);
		aliases[*alias_count].value = strdup(value);
		(*alias_count)++;
	}
}

/**
 * print_specific_alias - Print a specific alias or error if not found
 * @aliases: Array of aliases
 * @alias_count: Number of aliases
 * @name: Name of the alias to print
 */
void print_specific_alias(Alias *aliases, size_t alias_count, char *name)
{
	size_t i;

	for (i = 0; i < alias_count; i++)
	{
		if (strcmp(aliases[i].name, name) == 0)
		{
			printf("%s='%s'\n", aliases[i].name, aliases[i].value);
			return;
		}
	}

	fprintf(stderr, "alias: %s not found\n", name);
}

/**
 * handle_alias - Handle the alias command
 *
 * This function handles the alias command. If no arguments are given, it
 * prints out all the current aliases. If one or two arguments are given, it
 * sets a new alias. If the name of the alias is already in use, the value
 * is overwritten.
 *
 * @args: The arguments to the alias command
 */
void handle_alias(char **args)
{
	static size_t alias_count;
	static Alias aliases[MAX_ALIASES];

	if (args[1] == NULL)
	{
		print_aliases(aliases, alias_count);
	}
	else
	{
		int i;

		for (i = 1; args[i]; i++)
		{
			char *name = strtok(args[i], "=");
			char *value = strtok(NULL, "");

			if (value)
			{
				set_alias(aliases, &alias_count, name, value);
			}
			else
			{
				print_specific_alias(aliases, alias_count, name);
			}
		}
	}
}

/**
 * print_help - Print help for a command
 * @args: The command and its arguments
 *
 * This function prints help for a command if it is a built-in command and
 * if the command has a help message defined. If the command is not a built-in
 * command or it does not have a help message, it prints the shell's help
 * message.
 */
void print_help(char **args)
{
	if (args[1] == NULL)
	{
		print_general_help();
	}
	else
	{
		print_command_help(args[1]);
	}
}
