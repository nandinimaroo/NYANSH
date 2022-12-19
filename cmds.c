#include "headers_and_definitions.h"
#define N 10000

extern char *inp_arr[N];
extern char *initial;
extern char *curr_dir;
extern char *prev;
extern int flag_exit;
extern int fg_flag;
extern int fg_time;
extern int final_input_length;
extern char *o_file;
extern int bg_cmd_index;
int fg_pid_for_signal = -1;
void commands()
{
    // int g = 0;
    // while (inp_arr[g] != NULL)
    //     printf("%s\n", inp_arr[g++]);
    fg_time = 0;

    if (strcmp(inp_arr[0], "cd") == 0)
    {
        if (inp_arr[2] != NULL)
        {
            printf("ERROR: CANT HAVE MORE THAN ONE COMMAND LINE ARGUMENT\n");
        }
        else if (inp_arr[1] == NULL)
        {
            if (chdir(initial) != 0)
            {
                perror("cd");
            }
        }
        else if (inp_arr[1][0] == '.' && inp_arr[1][1] == '.' && strlen(inp_arr[1]) == 2)
        {
            int x = -1;
            for (int i = strlen(curr_dir) - 1; i >= 0; --i)
            {
                if (curr_dir[i] == '/')
                {
                    x = i;
                    break;
                }
            }
            char *c;
            if (x == 0)
            {
                c = (char *)malloc(2 * sizeof(char));
                c[0] = '/';
                c[1] = '\0';
            }
            else
            {

                c = (char *)malloc(x * sizeof(char));

                strncpy(c, curr_dir, x);
                c[x] = '\0';
            }

            if (chdir(c) != 0)
            {
                perror("cd");
            }
        }
        else if (inp_arr[1][0] == '-' && strlen(inp_arr[1]) == 1)
        {

            char *c = (char *)malloc(N * sizeof(char));
            if (strlen(prev) == 0)
            {
                printf("cd: OLDPWD NOT SET\n");
            }
            else
            {
                printf("%s\n", prev);

                if (chdir(prev) != 0)
                {
                    perror("cd");
                }
            }
        }
        else if (inp_arr[1][0] == '~')
        {
            char *c = (char *)malloc(N * sizeof(char));

            strcpy(c, initial);
            strncat(c, inp_arr[1] + 1, strlen(inp_arr[1]) - 1);
            if (chdir(c) != 0)
            {
                perror("cd");
            }
        }
        else
        {
            char *c = (char *)malloc(N * sizeof(char));
            strcpy(c, curr_dir);
            strcat(c, "/");
            strcat(c, inp_arr[1]);
            if (chdir(c) == -1)
            {
                perror("cd");
            }
        }
        strcpy(prev, curr_dir);

        part1();
    }
    else if (strcmp(inp_arr[0], "echo") == 0)
    {
        int i;
        if (inp_arr[1] != NULL)
        {
            for (i = 1; inp_arr[i + 1] != NULL; ++i)
            {
                printf("%s ", inp_arr[i]);
            }
            printf("%s\n", inp_arr[i]);
        }
        else
        {
            printf("\n");
        }
    }
    else if (strcmp(inp_arr[0], "history") == 0)
    {
        if (inp_arr[1] != NULL)
            print_history((int)atoi(inp_arr[1]));
        else
            print_history(10);
    }
    else if (strcmp(inp_arr[0], "pinfo") == 0)
    {
        if (inp_arr[1] != NULL)
        {
            print_pinfo((pid_t)atoi(inp_arr[1]));
        }
        else
        {
            print_pinfo(getpid());
        }
    }
    else if (strncmp(inp_arr[0], "pwd", 3) == 0)
    {
        printf("%s\n", curr_dir);
    }
    else if (strcmp(inp_arr[0], "exit") == 0)
    {

        exit(0);
    }
    else if (strcmp(inp_arr[0], "ls") == 0)
    {
        lsfunc();
    }
    else if (strcmp(inp_arr[0], "discover") == 0)
    {
        discoverfunc();
    }
    else if (strcmp(inp_arr[0], "jobs") == 0)
    {

        if (bg_cmd_index > 0)
            jobs();
    }
    else if (strcmp(inp_arr[0], "bg") == 0)
    {

        if (bg_cmd_index > 0)
            bg();
    }
    else if (strcmp(inp_arr[0], "fg") == 0)
    {

        if (bg_cmd_index > 0)
            fg();
    }
    else if (strcmp(inp_arr[0], "sig") == 0)
    {

        if (bg_cmd_index > 0)
            sig();
    }
    else
    {
        fg_flag=1;
        if (flag_exit == 0)
        {

            time_t begin = time(NULL);
            pid_t pid;
            pid = fork();
            int status;
            fg_pid_for_signal = pid;
            // printf("%d\n", fg_pid_for_signal);

            if (pid == -1)
            {
                printf("ERROR :  can't fork\n");
            }
            else if (pid == 0)
            {

                if (inp_arr[0] != NULL && strlen(inp_arr[0]) != 0)
                {
                    if (execvp(inp_arr[0], inp_arr) < 0)
                    {
                        perror("");
                    }
                }
            }
            else
            {
                waitpid(pid, &status, WSTOPPED);
                time_t end = time(NULL);
                fg_time = end - begin;
            }
        }
    }
}
