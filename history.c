#include "headers_and_definitions.h"
#define N 10000

extern char *h[20];
extern char *inp_arr[N];
extern int hist_ind;
extern char *history_file;
extern int bg_flag;
void get_history()
{

    FILE *h_f = fopen(history_file, "r");
    if (h_f == NULL)
    {
        h_f = fopen(history_file, "w");
        fclose(h_f);
        h_f = fopen(history_file, "r");
    }
    char temp[100];

    while (!feof(h_f))
    {
        fscanf(h_f, "%[^\n]%*c", temp);
        if (feof(h_f))
            break;
        if (temp != NULL)
        {
            strcpy(h[hist_ind++], temp);
        }
        else
            break;
        if (hist_ind == 20)
        {
            break;
        }
    }
    fclose(h_f);
}
void post_history()
{
    FILE *h_f = fopen(history_file, "w");

    for (int i = 0; i < hist_ind; ++i)
    {
        fprintf(h_f, "%s\n", h[i]);
    }
    fclose(h_f);
}

void hist_add()
{

    char *temp = (char *)malloc(N * sizeof(char));

    strcpy(temp, inp_arr[0]);
    strcat(temp, " ");

    for (int j = 1; inp_arr[j] != NULL; ++j)
    {
        strcat(temp, inp_arr[j]);
        strcat(temp, " ");
    }
    if (bg_flag == 1)
    {
        strcat(temp, "& ");
    }
    if (hist_ind >= 1)
    {
        if (strcmp(h[hist_ind - 1], temp) != 0)
        {
            if (hist_ind < 20)
            {

                strcpy(h[hist_ind++], temp);
            }
            else
            {

                for (int j = 0; j < 19; ++j)
                {
                    strcpy(h[j], h[j + 1]);
                }
                strcpy(h[19], temp);
            }
        }
    }
    else
    {
        {
            if (hist_ind < 20)
            {

                strcpy(h[hist_ind++], temp);
            }
            else
            {

                for (int j = 0; j < 19; ++j)
                {
                    strcpy(h[j], h[j + 1]);
                }
                strcpy(h[19], temp);
            }
        }
    }

    post_history();
}
void hist_add2(char *in)
{
    char *temp = (char *)malloc(N * sizeof(char));

    strcpy(temp, in);
    if (temp[strlen(temp) - 1] == '\n')
        temp[strlen(temp) - 1] = '\0';
    if (temp[0] == ' ')
        temp++;
    if (temp[strlen(temp) - 1] == ' ')
        temp[strlen(temp) - 1] = '\0';
    if (hist_ind >= 1)
    {
        if (strcmp(h[hist_ind - 1], temp) != 0)
        {
            if (hist_ind < 20)
            {

                strcpy(h[hist_ind++], temp);
            }
            else
            {

                for (int j = 0; j < 19; ++j)
                {
                    strcpy(h[j], h[j + 1]);
                }
                strcpy(h[19], temp);
            }
        }
    }
    else
    {
        {
            if (hist_ind < 20)
            {

                strcpy(h[hist_ind++], temp);
            }
            else
            {

                for (int j = 0; j < 19; ++j)
                {
                    strcpy(h[j], h[j + 1]);
                }
                strcpy(h[19], temp);
            }
        }
    }

    post_history();
}
void print_history(int x)
{
    if (x <= hist_ind)
    {
        int j = 0;
        int i;
        for (i = hist_ind - 1; i >= 0; --i)
        {
            j++;
            if (j == x)
                break;
        }
        for (int k = i; k < hist_ind; ++k)
        {
            printf("%s\n", h[k]);
        }
    }
    else
    {
        for (int i = 0; i < ((hist_ind > 10) ? 10 : hist_ind); ++i)
        {
            printf("%s\n", h[i]);
        }
    }
}
