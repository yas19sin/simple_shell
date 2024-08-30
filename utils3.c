#include "shell.h"

static size_t alias_count;
static Alias aliases[MAX_ALIASES];

static int last_exit_status;

/**
 * handle_aliases - Handle aliases in command arguments
 * @args: The command arguments
 */
void handle_aliases(char **args)
{
	size_t i;

	if (args[0] == NULL)
	{
		return;
	}

	for (i = 0; i < alias_count; i++)
	{
		if (strcmp(args[0], aliases[i].name) == 0)
		{
			char **new_args = parse_input(aliases[i].value);
			int j = 1;

			free(args[0]);
			args[0] = strdup(new_args[0]);
			while (new_args[j])
			{
				args[j] = strdup(new_args[j]);
				j++;
			}
			args[j] = NULL;
			free_args(new_args);
			break;
		}
	}
}

/**
 * initialize_aliases - Initialize aliases
 */
void initialize_aliases(void)
{
	alias_count = 0;
}

/**
 * get_last_exit_status_str - Gets the last exit status as a string
 *
 * Return: The last exit status as a string
 */
char *get_last_exit_status_str(void)
{
	char *status_str;

	status_str = malloc(12); /* Max size of an int as string + null terminator */
	if (status_str == NULL)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	snprintf(status_str, 12, "%d", last_exit_status);
	return (status_str);
}

/**
 * set_last_exit_status - Sets the last exit status
 * @status: The exit status
 */
void set_last_exit_status(int status)
{
	last_exit_status = status;
}
