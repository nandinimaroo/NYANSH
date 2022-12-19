#include "headers_and_definitions.h"

#define N 10000
extern int flag_exit;
extern char *inp_arr[N];
extern int bg_flag;
extern int fg_flag;
extern int fg_time;
extern int bg_cmd[N];
extern int bg_st[N];
extern int bg_cmd_index;
extern char *bg_commands[N];
extern char *bg_commands2[N];
extern int final_input_length;
extern int ofd;
extern int ifd;

extern int i_std;
extern int o_std;
void amp_handle(char *in)
{
    int n = 0;
    for (int i = 0; i < strlen(in); ++i)
    {
        if (in[i] == '&')
            n++;
    }
    char *fg = (char *)malloc(N * sizeof(char));
    strcpy(fg, in);
    int input_length;
    for (int i = strlen(fg) - 1; i >= 0; --i)
    {
        if (fg[i] != ' ' && fg[i] != '\n' && fg[i] != '\0')
        {
            input_length = i;
            break;
        }
    }
    for (int i = input_length + 1; i < strlen(fg); ++i)
    {
        fg[i] = '\0';
    }
    char *bg[n];
    for (int i = 0; i < n; ++i)
    {
        bg[i] = strtok_r(fg, "&", &fg);
        bg_flag = 1;
        if (bg[i] == NULL)
            break;
        char *temp = (char *)malloc(N * sizeof(char));
        strcpy(temp, bg[i]);
        bg_commands2[bg_cmd_index] = (char *)malloc(N * sizeof(char));

        strcpy(bg_commands2[bg_cmd_index], temp);
        break_input(bg[i]);

        if (flag_exit == 0)
        {
            pid_t pid;
            pid = fork();
            int status;
            if (pid == -1)
            {
                printf("ERROR :  can't fork\n");
            }
            else if (pid == 0)
            {
                if (ifd != -1)
                    dup2(i_std, 0);
                if (ofd != -1)
                    dup2(o_std, 1);
                printf("[%d] %u\n", i + 1, getpid());
                if (ifd != -1)
                    dup2(ifd, 0);
                if (ofd != -1)
                    dup2(ofd, 1);
                execvp(inp_arr[0], inp_arr);
            }
            else
            {

                waitpid(pid, &status, WNOHANG);
                bg_commands[bg_cmd_index] = (char *)malloc(N * sizeof(char));
                bg_cmd[bg_cmd_index] = pid;
                strcpy(bg_commands[bg_cmd_index], inp_arr[0]);
                bg_st[bg_cmd_index] = 0;
                bg_cmd_index++;
                backstd();
            }
        }

        for (int j = 0; j < N; ++j)
        {
            inp_arr[j] = NULL;
        }
    }

    bg_flag = 0;

    break_input(fg);
    if (fg != NULL && strlen(fg) != 0 && fg[0] != '\0')
    {

        if (flag_exit == 0)

            commands();
        backstd();
    }
    else
    {
        fg_flag=0;
    }
}
