/* main.c */
#include "shell.h"

int main(void)
{
    char *line;

    while (1)
    {
        write(STDOUT_FILENO, "$ ", 2);
        line = custom_getline();
        if (line == NULL)
            break;

        execute_command(line);
        free(line);
    }

    return 0;
}
