#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <locale.h>
#include <errno.h>

#define default_y LINES/2-4
#define default_x COLS/2-19
#define default_fx COLS/2-49
#define default_px COLS/2-27

typedef struct show_file_info{
    char fname[100];
    char ftype[10];
    struct stat info;
}finfo;

typedef WINDOW* WIN;
//////////////////////////////////////ui.c
void showprop();
void showlist();
void path_control(WIN, char*);
//////////////////////////////////////msg.c
void initialize();
void popup(char*,char*,int,int, int);
void fatal(char*,char*,int,int, int);
int finputbox(char*,char*,int,int, int);
void progress(char*,char*,int,int, int);
int Qbox(char*, char*, int, int, int);
int Listbox(char*, char[][9],int, int, int, int);
void inputbox(char*, char*,char*,int,int,int);
void ERROR(char*, char*,char*, int, int, int);
void killProgress();
//////////////////////////////////////fileM.c
void REFRESH();
void ErrorReport(int , char*);

///////////////////////////////////////list.c
void do_ls(char[]);
void dostat(char *);
void sort(int);
void show_file_info(char *, char *, struct stat*);
void mode_to_letters(int, char[]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

finfo* list;
int E_CODE;