#include "shell.h"

/**
 * handle_cd - Handle the cd builtin command
 *
 * @args: The command arguments
 * @last_exit_status: Pointer to the last exit status
 */
void handle_cd(char **args, int *last_exit_status)
{
	char *target_dir;
	char cwd[PATH_MAX];

	if (args[1] == NULL || strcmp(args[1], "~") == 0)
	{
		target_dir = getenv("HOME");
	}
	else if (strcmp(args[1], "-") == 0)
	{
		target_dir = getenv("OLDPWD");
		if (target_dir == NULL)
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			*last_exit_status = 1;
			return;
		}
		printf("%s\n", target_dir);
	}
	else
	{
		target_dir = args[1];
	}

	if (chdir(target_dir) != 0)
	{
		perror("cd");
		*last_exit_status = 1;
		return;
	}

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		*last_exit_status = 1;
		return;
	}

	custom_setenv("OLDPWD", getenv("PWD"), 1);
	custom_setenv("PWD", cwd, 1);
	*last_exit_status = 0;
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
	char *last_exit_status_str;

	last_exit_status_str = get_last_exit_status_str();

	for (i = 0; args[i] != NULL; i++)
	{
		if (strcmp(args[i], "$?") == 0)
		{
			replace_variable(&args[i], atoi(last_exit_status_str));
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

	free(last_exit_status_str);
}

/**
 * replace_variable - Replace a variable with its integer value
 *
 * @arg: Pointer to the argument to replace
 * @value: The integer value to replace with
 */
void replace_variable(char **arg, int value)
{
	char value_str[12]; /* Max size of an int as string + null terminator */

	snprintf(value_str, sizeof(value_str), "%d", value);
	free(*arg);
	*arg = strdup(value_str);
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
