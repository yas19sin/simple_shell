#include "shell.h"
#include <unistd.h>

/**
 * print_environment - Print the environment variables
 *
 * This function prints the environment variables to the standard output.
 */
void print_environment(void)
{
	char **environ = environ;
	int i;

	for (i = 0; environ[i]; i++)
	{
		printf("%s\n", environ[i]);
	}
}

/**
 * custom_setenv - Set an environment variable
 *
 * This function sets an environment variable with the given name and value.
 * If the variable already exists and overwrite is 0,
 * the function does nothing.
 * If the variable already exists and overwrite is 1,
 * the existing value is
 * overwritten.
 *
 * @name: The name of the environment variable to set
 * @value: The value to set the environment variable to
 * @overwrite: If 1, overwrite any existing value of the variable
 *
 * Return: 0 on success, -1 on failure
 */
int custom_setenv(const char *name, const char *value, int overwrite)
{
	char *env_var;

	if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL)
	{
		return (-1);
	}

	if (getenv(name) != NULL && !overwrite)
	{
		return (0);
	}

	env_var = malloc(strlen(name) + strlen(value) + 2);

	if (env_var == NULL)
	{
		return (-1);
	}

	sprintf(env_var, "%s=%s", name, value);

	if (putenv(env_var) != 0)
	{
		free(env_var);
		return (-1);
	}

	return (0);
}

/**
 * custom_unsetenv - Unset an environment variable
 *
 * This function unsets an environment variable with the given name.
 *
 * @name: The name of the environment variable to unset
 *
 * Return:: 0 on success, -1 on failure
 */
int custom_unsetenv(const char *name)
{
	if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL)
	{
		return (-1);
	}

	return (unsetenv(name));
}

/**
 * change_directory - Change the current working directory
 *
 * This function changes the current working directory to the given path.
 * If the path is NULL, the function changes to the home directory.
 *
 * @path: The path to change to
 */
void change_directory(char *path)
{
	char *new_path = path ? path : getenv("HOME");
	char cwd[PATH_MAX];

	if (chdir(new_path) != 0)
	{
		perror("cd");
	}
	else if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		setenv("PWD", cwd, 1);
	}
	else
	{
		perror("getcwd");
	}
}

/**
 * print_general_help - Print general help message
 */
void print_general_help(void)
{
	printf("Shell Help:\n");
	printf("  exit [status]: Exit the shell with a status code.\n");
	printf("  env: Display the current environment variables.\n");
	printf("  setenv VARIABLE VALUE: Set or update an environment variable.\n");
	printf("  unsetenv VARIABLE: Remove an environment variable.\n");
	printf("  cd [DIRECTORY]: Change the current directory.\n");
	printf("  alias [name='value']: Define or display aliases.\n");
	printf("  help [command]: Display help for a specific command.\n");
}
