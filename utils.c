/* get_line.c */
#include "shell.h"

char *custom_getline(void)
{
    static char buffer[BUFFER_SIZE];
    static size_t position = 0;
    ssize_t bytesRead = 0;
    char *line = NULL;

    while (1)
    {
        if (position == 0)
        {
            bytesRead = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            if (bytesRead == 0)
                break;
            if (bytesRead == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
        }

        while ((size_t)position < (size_t)bytesRead)
        {
            if (buffer[position] == '\n' || buffer[position] == '\0')
            {
                buffer[position] = '\0';
                line = strdup(buffer);
                if (line == NULL)
                {
                    perror("strdup");
                    exit(EXIT_FAILURE);
                }

                position++;
                return line;
            }
            position++;
        }

        position = 0;
    }

    return NULL;
}

char *custom_strtok(char *str, const char *delimiter)
{
    static char *nextToken = NULL;
    char *token;

    if (str != NULL)
    {
        nextToken = str;
    }
    else if (nextToken == NULL)
    {
        return NULL;
    }

    token = nextToken;
    nextToken = strpbrk(nextToken, delimiter);

    if (nextToken != NULL)
    {
        *nextToken++ = '\0';
    }

    return token;
}
