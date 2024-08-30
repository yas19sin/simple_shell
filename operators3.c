#include "shell.h"

/**
 * _strtow2 - Splits a string into words using a  delimiter
 * @_str: The input string
 * @_d: The  delimiter
 * Return: A pointer to an array of strings, or NULL on failure
 */
char **_strtow2(char *_str, char _d)
{
	int _j, i, _k, _m, _numwords = 0;
	char **_s;

	if (_str == NULL || _str[0] == 0)
		return (NULL);
	for (i = 0; _str[i] != '\0'; i++)
	{
		if ((_str[i] != _d && _str[i + 1] == _d) ||
			(_str[i] != _d && !_str[i + 1]) ||
			_str[i + 1] == _d)
			_numwords++;
	}
	if (_numwords == 0)
		return (NULL);
	_s = malloc((1 + _numwords) * sizeof(char *));
	if (!_s)
		return (NULL);
	for (i = 0, _j = 0; _j < _numwords; _j++)
	{
		while (_str[i] == _d && _str[i] != _d)
			i++;
		_k = 0;
		while (_str[i + _k] != _d && _str[i + _k] &&
			   _str[i + _k] != _d)
			_k++;
		_s[_j] = malloc((_k + 1) * sizeof(char));
		if (!_s[_j])
		{
			for (_k = 0; _k < _j; _k++)
				free(_s[_k]);
			free(_s);
			return (NULL);
		}
		for (_m = 0; _m < _k; _m++)
			_s[_j][_m] = _str[i++];
		_s[_j][_m] = 0;
	}
	_s[_j] = NULL;
	return (_s);
}

/**
 * _is_chain - Tests if the current character
 * in the buffer is a chain delimiter.
 * @info: The parameter struct.
 * @buf: The character buffer.
 * @p: Address of the current position in buf.
 * Return: 1 if a chain delimiter, 0 otherwise.
 */
int _is_chain(info_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';')
	{
		buf[j] = 0;
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
	{
		return (0);
	}
	*p = j;
	return (1);
}

/**
 * _check_chain - Checks whether to continue
 * chaining based on the last status.
 * @info: The parameter struct.
 * @buf: The character buffer.
 * @p: Address of the current position in buf.
 * @i: Starting position in buf.
 * @len: Length of buf.
 * Return: Void.
 */
void _check_chain(info_t *info, char *buf,
				  size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmd_buf_type == CMD_AND)
	{
		if (info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->cmd_buf_type == CMD_OR)
	{
		if (!info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * _replace_alias - Replaces aliases in the tokenized string.
 * @info: The parameter struct.
 * Return: 1 if replaced, 0 otherwise.
 */
int _replace_alias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = _node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
		{
			return (0);
		}

		free(info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
		{
			return (0);
		}

		p = _strdup(p + 1);
		if (!p)
		{
			return (0);
		}

		info->argv[0] = p;
	}
	return (1);
}

/**
 * _replace_vars - Replaces variables in the tokenized string.
 * @info: The parameter struct.
 * Return: 1 if replaced, 0 otherwise.
 */
int _replace_vars(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
		{
			continue;
		}
		if (_strcmp(info->argv[i], "$?") == 0)
		{
			_replace_string(&(info->argv[i]),
							_strdup(_convert_number(info->status, 10, 0)));
			continue;
		}
		if (_strcmp(info->argv[i], "$$") == 0)
		{
			_replace_string(&(info->argv[i]),
							_strdup(_convert_number(getpid(), 10, 0)));
			continue;
		}

		node = _node_starts_with(info->env, &info->argv[i][1], '=');
		if (node)
		{
			_replace_string(&(info->argv[i]),
							_strdup(_strchr(node->str, '=') + 1));
			continue;
		}

		_replace_string(&info->argv[i], _strdup(""));
	}
	return (0);
}
