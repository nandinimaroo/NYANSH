#include "headers_and_definitions.h"
#define N 10000
char *initial;
char *username;
char *sys_name;
char *curr_dir;
char *dir, *current;
char *cmd[] = {"cd", "echo", "pwd"};
char *inp_arr[N] = {NULL};
int bg_st[N] = {0};
int bg_cmd[N] = {1};
char *bg_commands[N] = {NULL};
char *bg_commands2[N] = {NULL};
int bg_cmd_index = 0;
char *inp[N] = {NULL};
char inp2[N];
char del[] = "        \t\n";
char *prev;
int flag_exit = 0;
char *h[20];
int hist_ind = 0;
char *history_file;
int bg_flag = 0;
int fg_flag = 0;
int fg_time;
extern int fg_pid_for_signal;
extern char inppp[N];
void run();

void bg_cmd_exit(int sigNum)
{
    int status;
    int i = 0;
    int pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0)
    {

        while (bg_cmd[i] != pid)
        {
            i++;
        }
        if (WIFSTOPPED(status))
            return;
        bg_st[i] = 1;
        printf("%s with pid %d exited %s\n", bg_commands[i], bg_cmd[i], WIFEXITED(status) ? "normally" : "abnormally");
    }
}
void ctrlzhandle(int sigNum)
{
    if (fg_flag == 1)
    {
        if (kill(fg_pid_for_signal, SIGTSTP) != -1)
        {
            bg_commands2[bg_cmd_index] = (char *)malloc(N * sizeof(char));

            strcpy(bg_commands2[bg_cmd_index], inppp);

            bg_commands[bg_cmd_index] = (char *)malloc(N * sizeof(char));
            bg_cmd[bg_cmd_index] = fg_pid_for_signal;
            strcpy(bg_commands[bg_cmd_index], inp_arr[0]);
            bg_st[bg_cmd_index] = 0;
            bg_cmd_index++;
        }
        else return;
    }

    return;
}
void ctrlchandle(int sigNum)
{
    if (fg_flag == 1)
    {
        if (fg_pid_for_signal != -1)
            kill(fg_pid_for_signal, SIGINT);

    }
    return;
}

void part1()
{

    int uid = getuid();

    username = (char *)malloc(N * sizeof(char));
    sys_name = (char *)malloc(N * sizeof(char));
    curr_dir = (char *)malloc(N * sizeof(char));
    dir = (char *)malloc(N * sizeof(char));
    current = (char *)malloc(N * sizeof(char));
    username = getpwuid(uid)->pw_name;
    gethostname(sys_name, N);
    getcwd(curr_dir, N);
    dir = getpwuid(uid)->pw_dir;

    if (strncmp(initial, curr_dir, strlen(initial)) == 0)
    {
        strcpy(current, "~");
        strncat(current, curr_dir + strlen(initial), strlen(curr_dir) - strlen(initial));
    }
    else
    {
        strncpy(current, curr_dir, strlen(curr_dir));
    }
}
void pp()
{
    printf("<"
           "\x1b[32m"
           "%s@%s:"
           "\x1b[34m"
           "%s"
           "\x1b[0m"
           ">",
           username,
           sys_name, current);
    return;
}
void run()
{

    printf("<"
           "\x1b[32m"
           "%s@%s:"
           "\x1b[34m"
           "%s"
           "\x1b[0m",
           username,
           sys_name, current);
    if (fg_flag)
    {
        if (fg_time > 1)
            printf("[took %ds]>\n", fg_time);
        else
        {
            printf(">\n");
        }
        fg_flag = 0;
    }
    else
    {
        printf(">\n");
    }
    input();

    for (int j = 0; inp[j] != NULL; ++j)
    {
        inp[j] = NULL;
    }
    signal(SIGCHLD, bg_cmd_exit);

    if (flag_exit == 0)
        run();
}
int main()
{
    signal(SIGTSTP, ctrlzhandle);
    signal(SIGINT, ctrlchandle);
    initial = (char *)malloc(N * sizeof(char));
    history_file = (char *)malloc(N * sizeof(char));

    for (int i = 0; i < 20; ++i)
    {
        h[i] = (char *)malloc(N * sizeof(char));
    }

    getcwd(initial, N);
    strcpy(history_file, initial);
    strcat(history_file, "/h.txt");
    get_history();

    prev = (char *)malloc(N * sizeof(char));

    part1();
    run();

    post_history();
}
