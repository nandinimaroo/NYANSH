#include "headers_and_definitions.h"
#define N 10000
extern int bg_cmd_index;
extern int bg_cmd[N];
void print_pinfo(pid_t x)
{
    printf("pid : %d\n", x);

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

        printf("process Status : %s\n", stat);
        printf("memory : %s\n", mem);
    }
    else
    {
        printf("pinfo : unable to open proc/%d/stat\n", x);
    }
}