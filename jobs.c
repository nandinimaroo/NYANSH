#include "headers_and_definitions.h"
extern int bg_cmd[N];
extern int bg_st[N];
extern int bg_cmd_index;
extern char *bg_commands[N];
extern char *bg_commands2[N];
extern char *inp_arr[N];
extern int fg_flag;
int b_to_f = 0;
int j1[N];
char *j2[N];
int j3[N];
int j4[N];
int j5[N];

void sint(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void sstr(char *a, char *b)
{
    char *temp = (char *)malloc(N * sizeof(char));
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
}

int runorstop(pid_t x)
{
    char file[N];
    sprintf(file, "/proc/%d/stat", (int)x);
    char BUFF[N] = {'\0'};

    FILE *pinfo_pointer = fopen(file, "r");

    if (pinfo_pointer != NULL)
    {
        fread(BUFF, N, 1, pinfo_pointer);
        fclose(pinfo_pointer);
        char *pinfo_token;
        char *stat = (char *)malloc(N * sizeof(char));
        char *mem = (char *)malloc(N * sizeof(char));
        char *at4 = (char *)malloc(N * sizeof(char));

        pinfo_token = strtok(BUFF, " ");

        for (int i = 1; i < 23; i++)
        {
            pinfo_token = strtok(NULL, " ");
            if (i == 2)
            {
                strcpy(stat, pinfo_token);
            }
            if (i == 22)
            {
                strcpy(mem, pinfo_token);
            }
            if (i == 4)
            {
                strcpy(at4, pinfo_token);
            }
            if (i == 7)

            {
                int plus_flag = 0;
                for (int j = 0; j < bg_cmd_index; ++j)
                {
                    if (x == bg_cmd[j])
                    {
                        plus_flag = 1;
                    }
                }
                if (strcmp(pinfo_token, at4) == 0 && !plus_flag)
                {
                    strcat(stat, "+");
                }
            }
        }
        if (stat[0] == 'S')
            return 1;
        else
            return 0;
    }
    else
    {
        return -1;
    }
}
int jlist()
{

    int indd = 0;
    for (int i = 0; i < bg_cmd_index; ++i)
    {

        if (runorstop(bg_cmd[i]) != -1)
        {
            j1[indd] = indd + 1;
            j2[indd] = (char *)malloc(N * sizeof(char));
            strcpy(j2[indd], bg_commands2[i]);
            j3[indd] = bg_cmd[i];
            j5[indd] = i;
            j4[indd++] = runorstop(bg_cmd[i]);
        }
    }
    return indd;
}
void bg()
{
    int indd = jlist();
    int p = -1;
    if (inp_arr[1] == NULL)
    {
        printf("bg: wrong usage\n");
        return;
    }

    for (int i = 0; i < indd; ++i)
    {
        if (j1[i] == atoi(inp_arr[1]))
        {
            p = j3[i];
            break;
        }
    }
    if (p == -1)
    {
        printf("bg: invalid job number\n");
        return;
    }
    if (kill(p, SIGCONT) == 0)
    {
    }
    else
    {
        printf("bg: wrong usage\n");
    }
}
void fg()
{
    fg_flag = 1;

    int indd = jlist();
    int p = -1;
    if (inp_arr[1] == NULL)
    {
        printf("fg: wrong usage\n");
        return;
    }
    for (int i = 0; i < indd; ++i)
    {
        if (j1[i] == atoi(inp_arr[1]))
        {
            p = j3[i];
            break;
        }
    }
    if (p == -1)
    {
        printf("fg: invalid job number\n");
        return;
    }
    int status;
    setpgid(p, getpgid(0));
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, p);

    if (kill(p, SIGCONT) < 0)
        perror("Error");

    waitpid(p, &status, WUNTRACED);

    tcsetpgrp(0, getpgid(0));

    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}
void sig()
{
    int indd = jlist();
    int p = -1;
    if (inp_arr[1] == NULL || inp_arr[2] == NULL)
    {
        printf("bg: wrong usage\n");
        return;
    }
    for (int i = 0; i < indd; ++i)
    {
        if (j1[i] == atoi(inp_arr[1]))
        {
            p = j3[i];
            break;
        }
    }
    if (p == -1)
    {
        printf("sig: invalid job number\n");
        return;
    }
    if (kill(p, atoi(inp_arr[2])) == 0)
    {
    }
    else
    {
        printf("sig: wrong usage\n");
    }
}
void jobs()
{

    int rflagg = 0;
    int sflagg = 0;
    if (inp_arr[1] == NULL)
    {
        rflagg = 1;
        sflagg = 1;
    }
    else
    {
        if (strcmp(inp_arr[1], "-r") == 0)
        {
            rflagg = 1;
        }
        else if (strcmp(inp_arr[1], "-s") == 0)
        {
            sflagg = 1;
        }
        else
        {
            printf("job: invalid command\n");
            return;
        }
    }
    int indd = jlist();
    for (int i = 0; i < indd; ++i)
    {
        for (int j = i + 1; j < indd; ++j)
        {
            if (strcmp(j2[i], j2[j]) > 0)
            {
                sint(&j1[i], &j1[j]);
                sint(&j3[i], &j3[j]);
                sint(&j4[i], &j4[j]);
                sint(&j5[i], &j5[j]);
                sstr(j2[i], j2[j]);
            }
        }
    }
    for (int i = 0; i < indd; ++i)
    {

        if (j4[i] == 1)
        {
            printf("[%d] Running %s [%d]\n", j1[i], j2[i], j3[i]);
        }
        if (j4[i] == 0)
        {
            printf("[%d] Stopped %s [%d]\n", j1[i], j2[i], j3[i]);
        }
    }
}
