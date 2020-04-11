#include "shell.h"


void shell_loop(void)
{
char *cmd;
char **argument;
int stat;

do {
printf("$> ");

cmd = read_cmd();
argument = parse_cmd(cmd);
stat = shell_execute(argument);

free(cmd);
free(argument);
} while (stat);
}

char *read_cmd(void)
{
char *buff = NULL;
size_t size = 0;

int read = getline(&buff, &size, stdin);
if (read == EOF)
{
perror("this is the end of file");
exit(EXIT_FAILURE);
}
buff[read-1] = '\0';

return (buff);
}

char **parse_cmd(char *cmd)
{
size_t bufsize = B_BBUFSIZE, i = 0;
char **t, *token;  

t = malloc(bufsize * sizeof(char*));

if (t == NULL)
{
perror("error");
exit(EXIT_FAILURE);
}
token = strtok(cmd, B_DELIM);

while (token != NULL)
{
t[i] = token;
i++;

if (i >= bufsize)
{
bufsize += B_BBUFSIZE;
t = realloc(t, bufsize * sizeof(char*));

if (t == NULL)
{
perror("error");
exit(EXIT_FAILURE);
}
}
token = strtok(NULL, B_DELIM);
}
t[i] = NULL;
return (t);
}

int shell_execute(char **argument)
{
pid_t pid, wpid;
int status;

pid = fork();

if (pid == 0)
{
if (execve("$PATH", argument, NULL) == -1)
{
perror("error");
}
exit(EXIT_FAILURE);
}
else if (pid < 0)
{
perror("error");
}
else
{
do
{
wpid = waitpid(pid, &status, WUNTRACED);
if (wpid == '\0')
{
perror("error");
}
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}
return (1);
}
