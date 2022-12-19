#include "headers_and_definitions.h"
extern char *full_input;
extern char *curr_dir;
extern char *initial;
extern char *username, sys_name, current;
int input_ind = 0;

void auto_comp()
{
    int tab_ind = 0;
    for (int i = input_ind; i >= 0; --i)
    {
        if (full_input[i] == ' ')
        {
            tab_ind = i;
            break;
        }
    }
    char *find = (char *)malloc(N * sizeof(char));
    strcpy(find, full_input + tab_ind + 1);
    char *found[N];
    DIR *dir_pointer;
    dir_pointer = opendir(curr_dir);
    if (dir_pointer != NULL)
    {

        struct dirent *dir_reader = readdir(dir_pointer);
        int n = 0;
        while (dir_reader != NULL)
        {

            if (strncmp(find, dir_reader->d_name, strlen(find)) == 0 && strlen(find) <= strlen(dir_reader->d_name))
            {
                found[n] = (char *)malloc(N * sizeof(char));
                strcpy(found[n++], dir_reader->d_name);
            }
            dir_reader = readdir(dir_pointer);
        }
        if (n == 1)
        {
            struct stat file_stat;
            char file[N];
            strcpy(file, curr_dir);
            strcat(file, "/");
            strcat(file, found[0]);
            int er = stat(file, &file_stat);
            if (er < 0)
            {
                perror("");
                return;
            }
            int dflag = 0;
            if (S_ISDIR(file_stat.st_mode))
            {
                dflag = 1;
            }
            if (dflag)
            {
                for (int i = strlen(find); i <= strlen(found[0]); ++i)
                {
                    printf("%c", found[0][i]);
                    full_input[input_ind++] = found[0][i];
                }
                printf("/");
                full_input[input_ind++] = '/';
            }
            else
            {
                for (int i = strlen(find); i <= strlen(found[0]); ++i)
                {
                    printf("%c", found[0][i]);
                    full_input[input_ind++] = found[0][i];
                }
                printf(" ");
                full_input[input_ind++] = ' ';
            }
        }
        if (n > 1)
        {
            for (int i = 0; i < n; ++i)
            {
                struct stat file_stat;
                char file[N];
                strcpy(file, curr_dir);
                strcat(file, "/");
                strcat(file, found[i]);
                int er = stat(file, &file_stat);
                if (er < 0)
                {
                    perror("");
                    return;
                }
                int dflag = 0;
                if (S_ISDIR(file_stat.st_mode))
                {
                    dflag = 1;
                }
                if (dflag)
                {
                    printf("\n%s/", found[i]);
                }
                else
                {
                    printf("\n%s", found[i]);
                }
            }
            printf("\n");

            pp();
            printf("%s", full_input);
        }
    }
}
void get_input()
{
    struct termios org;
    tcgetattr(STDIN_FILENO, &org);
    struct termios raw = org;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    setbuf(stdout, NULL);

    for (int i = 0; i < input_ind; ++i)
    {
        full_input[i] = '\0';
    }
    input_ind = 0;
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
            {
                printf("\n");
                break;
            }
            else if (c == 127)
            {
                if (input_ind > 0)
                {
                    full_input[input_ind - 1] = '\0';
                    input_ind--;
                    printf("\b \b");
                }
            }
            else if (c == 9)
            {
                auto_comp();
            }
            else if (c == 4)
            {
                exit(0);
            }
        }
        else
        {
            printf("%c", c);
            full_input[input_ind++] = c;
            full_input[input_ind] = '\0';
        }
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &org);
}
