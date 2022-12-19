#include "headers_and_definitions.h"
#define N 10000

extern char *inp_arr[N];
extern char *inp[N];
extern char inp2[N];
extern char del[];
extern int flag_exit;
int final_input_length;
extern int ofd;
extern int ifd;
int cnt = 0;
int hjk = 0;
extern int fg_flag;
char inppp[N];
char *full_input;

void break_input(char *in)
{
    if (in[0] == ' ')
        in++;
    if (in[strlen(in) - 1] == ' ' || in[strlen(in) - 1] == '\n')
        in[strlen(in) - 1] = '\0';
    strcpy(inppp, in);

    for (int j = 0; j < N; ++j)
    {
        inp_arr[j] = NULL;
    }
    cnt = 0;
    for (int i = 0; i < strlen(in); ++i)
    {
        if (in[i] == '|')
            cnt++;
    }
    if (cnt > 0)
    {
        hist_add2(in);

        hjk = 1;
        pipey(in);
        return;
    }
    if (in != NULL && strlen(in) != 0)
    {

        strcpy(inp2, in);
        if (in[strlen(in) - 1] == '\n')
        {
            in[strlen(in) - 1] == '\0';
        }
        else
        {
            in[strlen(in) - 1] == '\0';
        }

        int i = 0;

        while (strlen(in) != 0 && in != NULL)
        {

            inp_arr[i++] = strtok_r(in, del, &in);
        }
        final_input_length = i;
        inp_arr[i] = NULL;
        if (hjk != 1)
            hist_add();
        redirect_check();
    }
}
void input()
{
    full_input = (char *)malloc(N * sizeof(char));

    // size_t len_inp = 0;
    // fflush(stdout);
    // getline(&full_input, &len_inp, stdin);
    // fflush(stdout);
    get_input();
    int i = 0;
    if (full_input[0] == '\n')
    {
        run();
    }

    while (strlen(full_input) != 0)
    {
        inp[i++] = strtok_r(full_input, ";", &full_input);
    }
    for (int i = 0; inp[i] != NULL; ++i)
    {
        if (strchr(inp[i], '&') != NULL)
        {
            if (flag_exit == 0)

                amp_handle(inp[i]);
        }
        else
        {
            hjk = 0;
            // fg_flag = 1;
            break_input(inp[i]);
            if (flag_exit == 0 && cnt == 0)
                commands();
            backstd();
        }

        for (int j = 0; j < N; ++j)
        {
            inp_arr[j] = NULL;
        }
    }
}