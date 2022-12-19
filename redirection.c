#include "headers_and_definitions.h"
extern int final_input_length;
extern char *inp_arr[N];
extern char inp2[N];
int ofd = -1;
int ifd = -1;
int i_std;
int o_std;
char del1[] = " > < ><>> \t\n";

void redirect_check()
{
    int output_arrow1 = -1;
    int output_arrow2 = -1;
    int input_arrow = -1;
    for (int i = 0; i < final_input_length; ++i)
    {
        if (strstr(inp_arr[i], ">>"))
        {
            output_arrow2 = i;
        }
        else if (strstr(inp_arr[i], ">"))
        {
            output_arrow1 = i;
        }
        else if (strstr(inp_arr[i], "<"))
        {
            input_arrow = i;
        }
    }
    char *i_file = (char *)malloc(N * sizeof(char));
    char *o_file = (char *)malloc(N * sizeof(char));

    char *inp3 = (char *)malloc(N * sizeof(char));
    char *inp4[N];
    strcpy(inp3, inp2);
    int c = 0;
    while (strlen(inp3) != 0)
    {
        inp4[c++] = strtok_r(inp3, del1, &inp3);
    }
    for (int i = 0; i < final_input_length; ++i)
    {
        inp_arr[i] = NULL;
    }

    for (int i = 0; i < c; ++i)
    {
        inp_arr[i] = inp4[i];
    }
    final_input_length = c;
    if (output_arrow1 != -1 || output_arrow2 != -1)
    {
        if (input_arrow != -1)
        {
            strcpy(o_file, inp_arr[c - 1]);
            strcpy(i_file, inp_arr[c - 2]);
            inp_arr[c - 1] = NULL;
            inp_arr[c - 2] = NULL;
         
        }
        else
        {

            strcpy(o_file, inp_arr[c - 1]);
            inp_arr[c - 1] = NULL;
        }
    }
    else if (input_arrow != -1)
    {
        strcpy(i_file, inp_arr[c - 1]);
        inp_arr[c - 1] = NULL;
    }

    i_std = dup(0);
    o_std = dup(1);
    if (input_arrow != -1)
    {

        ifd = open(i_file, O_RDONLY);
        if (ifd == -1)
        {
            perror("opening input file");
        }
        else
        {
            dup2(ifd, 0);
        }
    }

    if (output_arrow1 != -1 || output_arrow2 != -1)
    {
        if (output_arrow1 != -1)

            ofd = open(o_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        else
        {
            ofd = open(o_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
            lseek(ofd, SEEK_END, 0);
        }

        if (ofd == -1)
        {
            perror("opening output file");
        }
        else
        {
            dup2(ofd, 1);
        }
    }

    return;
}

void backstd()
{
    if (ifd != -1)
        dup2(i_std, 0);
    if (ofd != -1)
        dup2(o_std, 1);
    ifd = -1;
    ofd = -1;
}