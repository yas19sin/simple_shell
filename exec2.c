#include "shell.h"

/**
 * handle_cd - Handle the cd builtin command
 *
 * @args: The command arguments
 */
void handle_cd(char **args)
{
	if (args[1])
	{
		change_directory(args[1]);
	}
	else
	{
		change_directory(NULL);
	}
}

/**
 * expand_variables - Expand variables in an array of strings
 *
 * This function iterates over an array of strings and if it finds a string
 * that is equal to "$?" or "$$", it replaces it with the current exit status
 * or the current process id respectively.
 * It also expands environment variables.
 *
 * @args: The array of strings to expand
 */
void expand_variables(char **args)
{
	int i;

	for (i = 0; args[i] != NULL; i++)
	{
		if (strcmp(args[i], "$?") == 0)
		{
			replace_variable(&args[i], getpid());
		}
		else if (strcmp(args[i], "$$") == 0)
		{
			replace_variable(&args[i], getpid());
		}
		else if (args[i][0] == '$')
		{
			expand_env_variable(&args[i]);
		}
	}
}

/**
 * replace_variable - Replace a variable with its integer value
 *
 * @arg: Pointer to the argument to replace
 * @value: The integer value to replace with
 */
void replace_variable(char **arg, int value)
{
	free(*arg);
	*arg = malloc(10);
	snprintf(*arg, 10, "%d", value);
}

/**
 * expand_env_variable - Expand an environment variable
 *
 * @arg: Pointer to the argument to expand
 */
void expand_env_variable(char **arg)
{
	char *env_value = getenv(*arg + 1);

	if (env_value != NULL)
	{
		free(*arg);
		*arg = strdup(env_value);
	}
}
