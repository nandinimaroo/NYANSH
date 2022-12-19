#include "headers_and_definitions.h"
extern char *inp_arr[N];
extern int final_input_length;
char del2[] = "|";
extern int i_std;
extern int o_std;
extern int cnt;
extern int flag_exit;

void pipey(char *in)
{
    int ex = 1;
    char *inp6 = (char *)malloc(N * sizeof(char));
    strcpy(inp6, in);

    char *inp4[N];
    int c = 0;
    while (strlen(inp6) != 0)
    {
        inp4[c++] = strtok_r(inp6, del2, &inp6);
    }

    for (int i = 0; i < c; i++)
    {
        if (inp4[i][0] == ' ')
            inp4[i]++;
        if (inp4[i][strlen(inp4[i]) - 1] == ' ')
            inp4[i][strlen(inp4[i]) - 1] = '\0';
    }
    i_std = dup(0);
    o_std = dup(1);
    int fds[cnt][2];
    for (int i = 0; i < cnt; ++i)
    {
        int x = pipe(fds[i]);
        if (x)
        {
            perror("pipe");
            ex = -1;
            return;
        }
    }
    for (int i = 0; i < c; ++i)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            if (i < cnt)
            {
                dup2(fds[i][1], 1);
            }
         
            if (i > 0)
            {
                close(fds[i - 1][1]);
                dup2(fds[i - 1][0], 0);
            }
         
            break_input(inp4[i]);
            if (flag_exit == 0)
                commands();
       
            exit(ex);
        }
    }
    for (int i = 0; i < cnt; ++i)
    {
        close(fds[i][0]);
        close(fds[i][1]);
    }
    for (int i = 0; i < c; ++i)
    {
        if (i == c - 1)
        {
            wait(&ex);
            ex = WEXITSTATUS(ex);
        }
        else
            wait(NULL);
    }
}