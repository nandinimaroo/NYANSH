#ifndef nyansh
#define nyansh
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <termios.h>
#include <ctype.h>
#include <pwd.h>
#include <dirent.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#define N 10000
void redirect_check();
void amp_handle();
void get_input();
void commands();
void bg_cmd_exit();
void part1();
void commands();
void amp_handle();
void run();
void print_pinfo(pid_t x);
void lsfunc();
void ls_print(char *dir);
void ls_dir(char *dir);
void break_input();
void input();
void hist_add();
void pp();
void hist_add2(char *in);
void get_history();
void post_history();
void print_history(int x);
void ctrlzhandle();
void ctrlchandle();
void discoverfunc();
void backstd();
void jobs();
void pipey(char *in);
void bg();
void fg();
void sig();
#endif