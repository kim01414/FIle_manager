#include "system.h"

extern int current, total;
extern WIN right,left,l_path,prop;
extern finfo* list;
extern char f_path[256];
void showprop()
{
    int i;   
    for(i=6 ; i<23 ; i++) mvwprintw(right,i,3,"                        ");
    box(right,ACS_VLINE,ACS_HLINE);
    wattron(right,A_BOLD|COLOR_PAIR(3));
    mvwprintw(right,6,3,  "    SIZE        TYPE       inode    ");
    mvwprintw(right,9,3,  "  I/O Size     Block       H-link   ");
    
    mvwprintw(right,12,3, "    OWNER                  GROUP    ");
    mvwprintw(right,15,3, "             ACCESS TIME            ");
    mvwprintw(right,18,3, "             EDIT  TIME             ");
    mvwprintw(right,21,3, "             PERMISSION             ");
    wattroff(right,A_BOLD|COLOR_PAIR(3));
    wrefresh(right);
    
    box(prop,ACS_VLINE,ACS_HLINE);
    show_file_info(list[current].fname, list[current].ftype, &list[current].info);
    mvwaddstr(prop,1,15,"Properties");
    wrefresh(prop);
    if(total!=0) mvwprintw(left,4,78,"(%3d / %3d)",current+1, total);
    else mvwprintw(left,4,78,"(%3d / %3d)",0, 0);
    wrefresh(left);
}

void showlist()
{
    int i,j=0;
    char temp[80];
    box(left,ACS_VLINE,ACS_HLINE);
    j=(current)/25;
    for(i=0 ; i<25 ; i++) mvwprintw(left,i+5,2,"                                                                                      ");
    for(i=25*j ; i<total ;i++)
    {
        if(i==25*(j+1)) break;
        wattron(left,A_BOLD);
        if(current==i) wattron(left,COLOR_PAIR(3));
        else if(S_ISDIR(list[i].info.st_mode)) wattron(left,COLOR_PAIR(6));
        mvwprintw(left,i-(j*25)+5,2,"%02d ",i+1);
        wattroff(left,A_BOLD);
        strncpy(temp,list[i].fname,79);
        wprintw(left,"%s",temp);
        if(current==i) wattroff(left,COLOR_PAIR(3));
        else if(S_ISDIR(list[i].info.st_mode)) wattroff(left,COLOR_PAIR(6));
    } 
    wrefresh(left);
    path_control(l_path,f_path);
}

void path_control(WIN w, char* path)
{
    int i;
    wbkgd(w,COLOR_PAIR(3));
    box(w,ACS_VLINE,ACS_HLINE);
    wattron(w,A_BOLD|COLOR_PAIR(3));
    mvwprintw(w,1,1,"Path");
    mvwaddch(w,1,5,ACS_VLINE);
    wattroff(w,A_BOLD);
    mvwprintw(w,1,6,"                                                                                 ");
    mvwprintw(w,1,6,path);//
    wattroff(w,A_BOLD|COLOR_PAIR(3));
    wrefresh(w);
}