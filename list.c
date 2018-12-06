#include "system.h"

#define KB 1024
#define MB (1024*1024)
#define GB (1024*1024*1024)
int total = 0;

void do_ls(char dirname[])
{
    DIR *dir_ptr;
    struct dirent *direntp;
    struct stat temp;
    
    list = (finfo*)malloc(sizeof(finfo)); //finfo를 저장할 리스트
    int i;

    if((dir_ptr = opendir(dirname)) ==NULL){
        fatal("Fatal Error","ERROR CODE(CANT_OPEN_DIR)",default_y,default_x,A_BOLD);
    }
    else
    {
        while((direntp = readdir(dir_ptr))!=NULL)
        {
            if(strcmp(".",direntp->d_name)==0 || strcmp("..",direntp->d_name)==0) continue;

            strcpy(list[total].fname, direntp->d_name);
            if(stat(list[total].fname, &temp) )continue;
            list[total++].info = temp;
            list = (finfo*)realloc(list,sizeof(finfo)*(total+1));
        } 
        closedir(dir_ptr);
    }
}

void sort(int code) //0: Name, 1: namE , 2: Size, 3:sizE, 4:Ftype, 5: ftypE
{
    extern int total;
    extern finfo* list;
    finfo temp;
    int i,j;

    for(i=0 ; i<total-1 ; i++){
        for(j=i+1 ; j<total ; j++){
            switch(code){
                case 0:
                    if(strcmp(list[i].fname, list[j].fname)==1){
                        temp = list[i];
                        list[i] = list[j];
                        list[j] = temp;
                    }
                    break;
                case 1:
                    if(strcmp(list[i].fname, list[j].fname)==-1){
                        temp = list[i];
                        list[i] = list[j];
                        list[j] = temp;
                    }
                    break;
                case 2:
                    if((long)list[i].info.st_size < (long)list[j].info.st_size){
                        temp = list[i];
                        list[i] = list[j];
                        list[j] = temp;
                    }
                    break;
                case 3:
                    if((long)list[i].info.st_size > (long)list[j].info.st_size){
                        temp = list[i];
                        list[i] = list[j];
                        list[j] = temp;
                    }
                    break;
                case 4:
                    if(strcmp(list[i].ftype, list[j].ftype)==1){
                        temp = list[i];
                        list[i] = list[j];
                        list[j] = temp;
                    }
                    break;
                case 5:
                    if(strcmp(list[i].ftype, list[j].ftype)==-1){
                        temp = list[i];
                        list[i] = list[j];
                        list[j] = temp;
                    }
                    break;
            }
        }
    }
}

void mode_color(int i, char r, char w, char x)
{
    extern WIN right;
    int j=15;
    if(r=='r'){
        wattron(right,COLOR_PAIR(8));
        mvwprintw(right,i,j,"  O  ");
        wattroff(right,COLOR_PAIR(8));
    }
    else{
        wattron(right,COLOR_PAIR(7));
        mvwprintw(right,i,j,"  X  ");
        wattroff(right,COLOR_PAIR(7));
    }
    waddch(right,ACS_VLINE);
    if(w=='w'){
        wattron(right,COLOR_PAIR(8));
        mvwprintw(right,i,j+6,"  O  ");
        wattroff(right,COLOR_PAIR(8));
    }
    else{
        wattron(right,COLOR_PAIR(7));
        mvwprintw(right,i,j+6,"  X  ");
        wattroff(right,COLOR_PAIR(7));
    }
    waddch(right,ACS_VLINE);
    if(x=='x'){
        wattron(right,COLOR_PAIR(8));
        mvwprintw(right,i,j+12,"  O  ");
        wattroff(right,COLOR_PAIR(8));
    }
    else{
        wattron(right,COLOR_PAIR(7));
        mvwprintw(right,i,j+12,"  X  ");
        wattroff(right,COLOR_PAIR(7));
    }
    waddch(right,ACS_VLINE);
    mvwaddch(right,i,14,ACS_VLINE);
}

void show_file_info(char *filename,  char *ftype, struct stat *info_p)
{
    int i,length;
    long fsize;
    char *uid_to_name(), *ctime(), *gid_to_name(), *filemod();
    void mode_to_letters();
    char modestr[11],NONE[10]=" -",*type=filename;
    char id[30];
    extern WIN right;
    length = strlen(type);
    type+=length;
    
    mode_to_letters(info_p->st_mode, modestr);
    mvwprintw(right,7,3,"                                   ");
    fsize=(long)info_p->st_size;
    if(fsize<KB) mvwprintw(right,7,3, "  %3ld Bytes",fsize);
    else if(fsize<MB &&fsize>=KB) mvwprintw(right,7,3, "  %3ld KB",fsize/KB);
    else if(fsize<GB && fsize>=MB) mvwprintw(right,7,3, "  %3ld MB",fsize/MB);
    else if(fsize>=GB) mvwprintw(right,7,3, "  %3ld GB",fsize/GB);

    if( S_ISDIR(info_p->st_mode) ){
        if( (info_p->st_mode) & S_ISVTX ) wattron(right,COLOR_PAIR(7));
        else  wattron(right,COLOR_PAIR(6));
        strcpy(ftype,"<DIR>");
        mvwprintw(right,7,19,"%s",ftype);
        if( (info_p->st_mode) & S_ISVTX ) wattroff(right,COLOR_PAIR(7));
        else wattroff(right,COLOR_PAIR(6));
    } 
    else{
        for(i=0 ; i<length ; i++){
            if(type[0]=='.') break;
            type--;
            if(i==length-1) type = NONE;
        }
        strcpy(ftype,type+1);
        mvwprintw(right,7,15,"  %5s",ftype);
    } 

    mvwprintw(right,7,28," %08ld",(long)info_p->st_ino);
    mvwprintw(right,10,2,"%6ld bytes   %8ld   %8ld",(long)info_p->st_blksize,(long)info_p->st_blocks,(long)info_p->st_nlink);
    mvwprintw(right,13,3,"            ");

    strncpy(id,uid_to_name(info_p->st_uid),30);
    if(strcmp(id,"root")==0) wattron(right,COLOR_PAIR(7));
    mvwprintw(right,13,3,"%16s",id);
    if(strcmp(id,"root")==0) wattroff(right,COLOR_PAIR(7));

    strncpy(id,gid_to_name(info_p->st_gid),30);
    if(strcmp(id,"root")==0) wattron(right,COLOR_PAIR(7));
    mvwprintw(right,13,23,"%16s",id);
    if(strcmp(id,"root")==0) wattroff(right,COLOR_PAIR(7));

    mvwprintw(right,16,8,"%.24s",ctime(&info_p->st_atime));
    mvwprintw(right,19,8,"%.24s",ctime(&info_p->st_mtime));
    
    wattron(right,A_BOLD);
    mvwprintw(right,22,8,"------ READ  WRITE EXEC");
    mvwprintw(right,23,8,"USER  ");
    mvwprintw(right,24,8,"GROUP ");
    mvwprintw(right,25,8,"OTHER ");
    wattroff(right,A_BOLD);
    mode_color(23,modestr[1],modestr[2],modestr[3]);
    mode_color(24,modestr[4],modestr[5],modestr[6]);
    mode_color(25,modestr[7],modestr[8],modestr[9]);
    wattron(right,COLOR_PAIR(3)|A_BOLD);
    mvwprintw(right,27,5,"EDIT");
    wattroff(right,COLOR_PAIR(3)|A_BOLD);
    mvwprintw(right,27,10,"%.24s",ctime(&info_p->st_ctime));
    wrefresh(right);
}

void mode_to_letters(int mode, char str[])
{
    strcpy(str,"----------");
    if( S_ISDIR(mode)) str[0]= 'd';
    if( S_ISCHR(mode)) str[0]= 'c';
    if( S_ISBLK(mode)) str[0]= 'b';

    if( mode & S_IRUSR) str[1]='r';
    if( mode & S_IWUSR) str[2]='w';
    if( mode & S_IXUSR) str[3]='x';

    if( mode & S_IRGRP) str[4]='r';
    if( mode & S_IWGRP) str[5]='w';
    if( mode & S_IXGRP) str[6]='x';

    if( mode & S_IROTH) str[7]='r';
    if( mode & S_IWOTH) str[8]='w';
    if( mode & S_IXOTH) str[9]='x';
}

char *uid_to_name(uid_t uid)
{
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];
    if( (pw_ptr = getpwuid(uid))==NULL)
    {
        sprintf(numstr,"%d",uid);
        return numstr;
    }
    else return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid)
{
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];
    if( (grp_ptr = getgrgid(gid))==NULL)
    {
        sprintf(numstr,"%d",gid);
        return numstr;
    }
    else return grp_ptr->gr_name;
}