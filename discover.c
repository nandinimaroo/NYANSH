#include "headers_and_definitions.h"
#define N 10000
extern char *inp_arr[N];
extern char *curr_dir;
extern char *initial;
int dflag = 0;
int fflag = 0;
int file_there = 0;
int dis_done = 0;
int i_am_dumb = 0;
int i_am_dumb2 = 0;
char initial_dir[N];

int its_a_file_bruh(char *dir, char *is_it_file)
{
    char file[N];
    strcpy(file, dir);
    strcat(file, "/");
    strcat(file, is_it_file);

    struct stat file_stat;
    if (lstat(file, &file_stat) < 0)
    {
        perror("discover");
        return -1;
    }
    int diflag = 0;
    if (S_ISDIR(file_stat.st_mode))
    {
        diflag = 1;
    }
    return !diflag;
}

void discover_print(char *dir, char *file_to_find)
{

    struct dirent **file_list;

    int n;
    if ((n = scandir(dir, &file_list, NULL, alphasort)) < 0)
    {
        perror("discover");
        return;
    }
    int diflag = 0;
    if (dflag || (!fflag && !dflag) && !file_there)
    {
        if (!file_there)
        {
            if (i_am_dumb)
            {
                printf("..%s\n", &dir[strlen(initial_dir)]);
            }
            else if (i_am_dumb2)
            {
                printf("~%s\n", &dir[strlen(initial)]);
            }
            else
                printf(".%s\n", &dir[strlen(curr_dir)]);
        }
    }
    for (int i = 0; i < n; ++i)
    {
        if (file_list[i]->d_name[0] != '.')

        {

            diflag = !(its_a_file_bruh(dir, file_list[i]->d_name));

            if (!diflag && !file_there)
            {
                if (fflag || (!fflag && !dflag))
                {
                    if (i_am_dumb)
                    {

                        printf("..%s/%s\n", &dir[strlen(initial_dir)], file_list[i]->d_name);
                    }
                    else if (i_am_dumb2)
                    {
                        printf("~%s/%s\n", &dir[strlen(initial)], file_list[i]->d_name);
                    }

                    else if (strcmp(curr_dir, dir) == 0)
                        printf("./%s\n", file_list[i]->d_name);
                    else
                        printf(".%s/%s\n", &dir[strlen(curr_dir)], file_list[i]->d_name);
                }
            }
            if (file_there)
            {

                if (strcmp(file_to_find, file_list[i]->d_name) == 0)
                {
                    dis_done = 1;
                    if (i_am_dumb)
                    {
                        printf("..%s/%s\n", &dir[strlen(initial_dir)], file_list[i]->d_name);

                    }
                    else if (i_am_dumb2)
                    {
                        printf("~%s/%s\n", &dir[strlen(curr_dir)], file_to_find);
                    }
                    else
                        printf(".%s/%s\n", &dir[strlen(curr_dir)], file_to_find);
                    return;
                }
            }
        }
    }
    for (int i = 0; i < n; ++i)
    {
        if (file_list[i]->d_name[0] != '.')

        {

            diflag = !(its_a_file_bruh(dir, file_list[i]->d_name));
            if (diflag)
            {

                char c[N];
                strcpy(c, dir);
                strcat(c, "/");
                strcat(c, file_list[i]->d_name);
                discover_print(c, file_to_find);
            }
        }
    }
}

void discoverfunc()
{
    int no_of_arguments = 0;

    char dir[N];
    char file_to_find[N];
    for (int i = 0; (1); ++i)
    {

        if (inp_arr[i] == NULL)
        {
            no_of_arguments = i;
            break;
        }
        else
        {
            if (strcmp(inp_arr[i], "-f") == 0)
            {
                fflag = 1;
            }
            else if (strcmp(inp_arr[i], "-d") == 0)
            {
                dflag = 1;
            }
            else if (strcmp(inp_arr[i], "~") == 0)
            {
                i_am_dumb2 = 1;

                strcpy(dir, initial);
            }
            else if (strcmp(inp_arr[i], ".") == 0)
            {
                strcpy(dir, curr_dir);
            }
            else if (strcmp(inp_arr[i], "..") == 0)
            {
                i_am_dumb = 1;
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
                strcpy(dir, c);
            }
            else if (inp_arr[i][strlen(inp_arr[i]) - 1] == '"' && inp_arr[i][0] == '"')
            {
                dis_done = 1;
                file_there = 1;
                strncpy(file_to_find, &inp_arr[i][1], strlen(inp_arr[i]) - 2);
            }
            else if (inp_arr[i][0] == '.' && inp_arr[i][1] == '/')
            {
                strcpy(dir, curr_dir);

                strcat(dir, &inp_arr[i][1]);
            }
        }
    }

    if (no_of_arguments == 1 || (no_of_arguments == 2 && (fflag || dflag)) || (no_of_arguments == 3 && dflag && fflag))
    {
        strcpy(dir, curr_dir);
    }
    if (dir == NULL || strlen(dir) == 0)
    {
        strcpy(dir, curr_dir);
    }
    strcpy(initial_dir, dir);
  
    discover_print(dir, file_to_find);

    dflag = 0;
    fflag = 0;
    if (dis_done == 0 && file_there)
    {
        printf("discover: file doesn't exist\n");
    }
    file_there = 0;
    dis_done = 0;
    i_am_dumb = 0;
    i_am_dumb2 = 0;
}