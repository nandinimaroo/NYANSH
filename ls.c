#include "headers_and_definitions.h"
#define N 10000
extern char *inp_arr[N];
extern char *curr_dir;
extern char *initial;
int argument_count;
int curr_dir_flag;
int aflag;
int lflag;
int dir_num;
void lsfunc()
{
    argument_count = 0;
    curr_dir_flag = 0;
    aflag = 0;
    lflag = 0;
    dir_num = 0;
    int i = 1;
    while (inp_arr[i++] != NULL)
    {
        argument_count++;
    }

    if (argument_count == 0)
    {

        inp_arr[1] = (char *)malloc(N * sizeof(char));
        strcpy(inp_arr[1], curr_dir);
        argument_count++;
    }
    for (i = 1; i <= argument_count; ++i)
    {

        if (strcmp(inp_arr[i], "-l") == 0)
        {
            lflag = 1;
        }
        else if (strcmp(inp_arr[i], "-a") == 0)
        {
            aflag = 1;
        }
        else if (strcmp(inp_arr[i], "-al") == 0 || strcmp(inp_arr[i], "-la") == 0)
        {
            aflag = 1;
            lflag = 1;
        }
        else
            dir_num++;
    }

    int flag_for_just_ls = 0;
    for (i = 1; i <= argument_count; ++i)
    {

        if ((strcmp(inp_arr[i], "-l") != 0) && (strcmp(inp_arr[i], "-a") != 0) && (strcmp(inp_arr[i], "-al") != 0) && (strcmp(inp_arr[i], "-la") != 0))
        {
            flag_for_just_ls = 1;
            ls_dir(inp_arr[i]);
        }
    }
    if (flag_for_just_ls == 0)
    {
        ls_dir(curr_dir);
    }
}

void ls_dir(char *dir)
{

    if (strcmp(dir, "..") == 0)
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
        ls_print(c);
    }
    else if (strcmp(dir, "~") == 0)
    {
        char *c = (char *)malloc(N * sizeof(char));

        strcpy(c, initial);
        ls_print(c);
    }
    else if (dir[0] == '~')
    {
        char *c = (char *)malloc(N * sizeof(char));

        strcpy(c, initial);
        strncat(c, dir + 1, strlen(dir) - 1);
        ls_print(c);
    }
    else if (strcmp(dir, ".") == 0)
    {
        char *c = (char *)malloc(N * sizeof(char));

        strcpy(c, curr_dir);
        ls_print(c);
    }
    else if (strncmp(curr_dir, dir, strlen(curr_dir)) == 0)
    {
        ls_print(dir);
    }
    else
    {
        char *c = (char *)malloc(N * sizeof(char));
        strcpy(c, curr_dir);
        strcat(c, "/");
        strcat(c, dir);
        ls_print(c);
    }
}
int alphasort(const struct dirent **dir1, const struct dirent **dir2)
{
    char a[200], b[200];
    strcpy(a, (*dir1)->d_name);
    strcpy(b, (*dir2)->d_name);
    return (strcasecmp(a, b));
}

void ls_print_deets(struct stat file_status, char *dir)
{
    char perms[] = "----------";
    int dflag = 0;
    int eflag = 0;
    if (S_ISDIR(file_status.st_mode))
    {
        perms[0] = 'd';
        dflag = 1;
    }
    if (file_status.st_mode & S_IRUSR)
        perms[1] = 'r';
    if (file_status.st_mode & S_IWUSR)
        perms[2] = 'w';
    if (file_status.st_mode & S_IXUSR)
    {
        perms[3] = 'x';
        eflag = 1;
    }
    if (file_status.st_mode & S_IRGRP)
        perms[4] = 'r';
    if (file_status.st_mode & S_IWGRP)
        perms[5] = 'w';
    if (file_status.st_mode & S_IXGRP)
        perms[6] = 'x';
    if (file_status.st_mode & S_IROTH)
        perms[7] = 'r';
    if (file_status.st_mode & S_IWOTH)
        perms[8] = 'w';
    if (file_status.st_mode & S_IXOTH)
        perms[9] = 'x';
    char months[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    struct tm *time = localtime(&file_status.st_mtime);
    char time_file[N];
    sprintf(time_file, "%s %02d %02d:%02d ", months[time->tm_mon], time->tm_mday,
            time->tm_hour, time->tm_min);
    printf("%s\t", perms);
    printf("%lu\t", file_status.st_nlink);
    printf("%s\t", getpwuid(file_status.st_uid)->pw_name);
    printf("%s\t", getgrgid(file_status.st_gid)->gr_name);
    printf("%lu\t", file_status.st_size);
    printf("%s\t", time_file);
    if (dflag)
    {
        printf("\x1b[34m"
               "%s\n"
               "\x1b[0m",
               dir);
    }
    else if (eflag)
    {
        printf("\x1b[32m"
               "%s\n"
               "\x1b[0m",
               dir);
    }
    else
    {
        printf("%s\n", dir);
    }
}

void ls_print(char *dir)
{
    DIR *dir_pointer;
    dir_pointer = opendir(dir);
    if (dir_pointer == NULL)
    {
        struct stat file_stat;
        char file[N];
        strcpy(file, dir);

        int er = stat(file, &file_stat);
        if (er < 0)
        {
            perror("ls");
            return;
        }
        int eflag = 0;

        if (file_stat.st_mode & S_IXUSR)
        {
            eflag = 1;
        }
        if (!lflag)
        {
            if (eflag)
            {
                printf("\x1b[32m"
                       "%s\n"
                       "\x1b[0m",
                       &dir[strlen(curr_dir) + 1]);
            }
            else
            {
                printf("%s\n", &dir[strlen(curr_dir) + 1]);
            }
        }
        else
        {
            ls_print_deets(file_stat, &dir[strlen(curr_dir) + 1]);
        }

        return;
    }
    if (dir_num > 1)
    {
        printf("%s:\n", &dir[strlen(curr_dir) + 1]);
    }
    struct dirent *dir_reader = readdir(dir_pointer);
    int total = 0;
    int file_count = 0;
    if (lflag)
    {
        while (dir_reader != NULL)
        {
            char file[N];
            strcpy(file, dir);
            strcat(file, "/");
            strcat(file, dir_reader->d_name);
            struct stat file_stat;
            if (lstat(file, &file_stat) < 0)
            {
                perror("ls");
                return;
            }
            if (dir_reader->d_name[0] != '.' || aflag)
            {
                total += (file_stat.st_blocks * 512 + 1023) / 1024;
            }
            dir_reader = readdir(dir_pointer);
            file_count++;
        }
        printf("total: %d\n", total);
    }
    closedir(dir_pointer);

    struct dirent **file_list;

    int n;
    if ((n = scandir(dir, &file_list, NULL, alphasort)) < 0)
    {
        perror("ls");
        return;
    }

    for (int i = 0; i < n; ++i)
    {
        if (file_list[i]->d_name[0] != '.' || aflag)

        {

            if (!lflag)
            {
                struct stat file_stat;
                char file[N];
                strcpy(file, dir);
                strcat(file, "/");
                strcat(file, file_list[i]->d_name);
                int er = stat(file, &file_stat);
                if (er < 0)
                {
                    perror("ls");
                    return;
                }
                int dflag = 0;
                int eflag = 0;
                if (S_ISDIR(file_stat.st_mode))
                {
                    dflag = 1;
                }

                if (file_stat.st_mode & S_IXUSR)
                {
                    eflag = 1;
                }
                if (dflag)
                {
                    printf("\x1b[34m"
                           "%s\n"
                           "\x1b[0m",
                           file_list[i]->d_name);
                }
                else if (eflag)
                {
                    printf("\x1b[32m"
                           "%s\n"
                           "\x1b[0m",
                           file_list[i]->d_name);
                }
                else
                {
                    printf("%s\n", file_list[i]->d_name);
                }
            }
        }
    }

    if (lflag)
    {
        for (int i = 0; i < n; ++i)
        {
            if (file_list[i]->d_name[0] != '.' || aflag)
            {
                struct stat file_stat;
                char file[N];
                strcpy(file, dir);
                strcat(file, "/");
                strcat(file, file_list[i]->d_name);
                int er = stat(file, &file_stat);
                if (er < 0)
                {
                    perror("ls");
                    return;
                }

                ls_print_deets(file_stat, file_list[i]->d_name);
            }
        }
    }
}
