#include "system.h"
WIN msgbox, shadow;

void popup(char* title, char* msg, int y, int x, int flag)
{
    int settings = A_NORMAL; //default settings
    int i,j;
    settings |= flag; //additional settings
    msgbox = newwin(9,39,y,x); //create msgbox
    shadow = newwin(9,39,y+1,x+2); //create shadow of msgbox
    wbkgd(msgbox,COLOR_PAIR(2)); //COLOR_WHITE + COLOR_CYAN
    wbkgd(shadow,COLOR_PAIR(3)); //COLOR_WHITE + COLOR+BLACK

    box(msgbox,ACS_VLINE,ACS_HLINE);
    wattron(msgbox,settings);
    mvwprintw(msgbox,3,3,msg);

    mvwprintw(msgbox,0,1,title);
    wattron(msgbox,A_STANDOUT|COLOR_PAIR(1));
    mvwprintw(msgbox,6,16,"   OK   ");
    
    wrefresh(shadow);
    wrefresh(msgbox);
    getch();
    delwin(msgbox);
    delwin(shadow);
    REFRESH();
}

int Qbox(char* title, char* msg, int y, int x, int flag)
{
    int settings = A_NORMAL; //default settings
    int rtn,c;
    settings |= flag; //additional settings
    msgbox = newwin(9,39,y,x); //create msgbox
    shadow = newwin(9,39,y+1,x+2); //create shadow of msgbox
    wbkgd(msgbox,COLOR_PAIR(2)); //COLOR_WHITE + COLOR_CYAN
    wbkgd(shadow,COLOR_PAIR(3)); //COLOR_WHITE + COLOR+BLACK

    box(msgbox,ACS_VLINE,ACS_HLINE);
    wattron(msgbox,settings);
    mvwprintw(msgbox,3,5,msg);

    mvwprintw(msgbox,0,1,title);
    wattron(msgbox,A_STANDOUT|COLOR_PAIR(1));
    mvwprintw(msgbox,6,5,  "    YES(Y)   ");
    mvwprintw(msgbox,6,22, "    NO(N)    ");
    wrefresh(shadow);
    wrefresh(msgbox);
    while(( c=getch() ) ){
        if(c=='y' || c=='Y' || c=='\n'){
            rtn=1;
            break;
        } 
        else if(c=='n' || c=='N' || c==27){
            rtn=0;
            break;
        } 
    }
    delwin(msgbox);
    delwin(shadow);
    REFRESH();
    return rtn;
}

int Listbox(char* title, char option[][9],int total, int y, int x, int flag)
{
    int settings = A_NORMAL; //default settings
    int rtn=0,c;
    settings |= flag; //additional settings
    msgbox = newwin(9,39,y,x); //create msgbox
    shadow = newwin(9,39,y+1,x+2); //create shadow of msgbox
    wbkgd(msgbox,COLOR_PAIR(2)); //COLOR_WHITE + COLOR_CYAN
    wbkgd(shadow,COLOR_PAIR(3)); //COLOR_WHITE + COLOR+BLACK
    keypad(msgbox,TRUE);
    box(msgbox,ACS_VLINE,ACS_HLINE);
    wattron(msgbox,settings);
    mvwprintw(msgbox,0,1,title);
    wattron(msgbox,A_STANDOUT|COLOR_PAIR(1));
    mvwprintw(msgbox,6,13,  "   SELECT   ");
    wattron(msgbox,A_DIM);
    mvwprintw(msgbox,3,9,"      %s      ",option[rtn]);
    wattroff(msgbox,A_DIM);

    wrefresh(shadow);
    wrefresh(msgbox);
    while( ( c=getch() ) ){
        if(c==KEY_UP){
            rtn--;
            if(rtn<0) rtn=total-1;
        }
        else if(c==KEY_DOWN){
            rtn++;
            if(rtn==total) rtn=0;
        }
        else if(c=='\n') break;
        wattron(msgbox,A_DIM);
        mvwprintw(msgbox,3,9,"      %s      ",option[rtn]);
        wattroff(msgbox,A_DIM);
        wrefresh(msgbox);
    }
    REFRESH();
    delwin(msgbox);
    delwin(shadow);
    return rtn;
}

int finputbox(char* title, char* msg, int y, int x, int flag)
{
    int settings = A_NORMAL; //default settings
    int i,j;
    extern char f_path[88],old_path[88];
    extern WIN right;
    settings |= flag; //additional settings
    msgbox = newwin(9,99,y,x); //create msgbox
    shadow = newwin(9,99,y+1,x+2); //create shadow of msgbox
    wbkgd(msgbox,COLOR_PAIR(2)); //COLOR_WHITE + COLOR_CYAN
    wbkgd(shadow,COLOR_PAIR(3)); //COLOR_WHITE + COLOR+BLACK

    box(msgbox,ACS_VLINE,ACS_HLINE);
    wattron(msgbox,settings);
    mvwprintw(msgbox,0,1,title);
    mvwprintw(msgbox,2,5,msg);
    
    wattron(msgbox,A_STANDOUT|COLOR_PAIR(1));
    for(i=0 ; i<88 ; i++) mvwprintw(msgbox,4,5+i, " ");
    mvwprintw(msgbox,6,26, "     OK     ");
    mvwprintw(msgbox,6,60,"   Cancel   ");
    wmove(msgbox,4,5);
    echo();
    curs_set(2);
    wrefresh(shadow);
    wrefresh(msgbox);
    strcpy(old_path,f_path);
    wscanw(msgbox,"%[^\n]",f_path);
    curs_set(0);
    noecho();
    delwin(msgbox);
    delwin(shadow);
    REFRESH();
    showprop();
    return 0;
}

void inputbox(char* title, char* msg, char* ans, int y, int x, int flag)
{
    int settings = A_NORMAL; //default settings
    int i,j;
    char buffer[256];
    extern WIN right;
    settings |= flag; //additional settings
    msgbox = newwin(9,39,y,x); //create msgbox
    shadow = newwin(9,39,y+1,x+2); //create shadow of msgbox
    wbkgd(msgbox,COLOR_PAIR(2)); //COLOR_WHITE + COLOR_CYAN
    wbkgd(shadow,COLOR_PAIR(3)); //COLOR_WHITE + COLOR+BLACK

    box(msgbox,ACS_VLINE,ACS_HLINE);
    wattron(msgbox,settings);
    mvwprintw(msgbox,0,1,title);
    mvwprintw(msgbox,2,5,msg);
    
    wattron(msgbox,A_STANDOUT|COLOR_PAIR(1));
    for(i=0 ; i<33 ; i++) mvwprintw(msgbox,4,3+i, " ");
    mvwprintw(msgbox,6,5, "     OK     ");
    mvwprintw(msgbox,6,22,"   Cancel   ");
    wmove(msgbox,4,3);
    echo();
    curs_set(2);
    wrefresh(shadow);
    wrefresh(msgbox);
    wscanw(msgbox,"%[^\n]",buffer);
    strcpy(ans, buffer);
    curs_set(0);
    noecho();
    wclear(right);
    delwin(msgbox);
    delwin(shadow);
    REFRESH();
}

void fatal(char* title, char* msg, int y, int x, int flag)
{
    int settings = A_NORMAL; //default settings
    int i,j;
    extern WIN left, right,bottom,TITLE,l_path,prop;
    settings |= flag; //additional settings
    msgbox = newwin(9,39,y,x); //create msgbox
    shadow = newwin(9,39,y+1,x+2); //create shadow of msgbox
    wbkgd(msgbox,COLOR_PAIR(5)); //COLOR_WHITE + COLOR_RED
    wbkgd(shadow,COLOR_PAIR(3)); //COLOR_WHITE + COLOR+BLACK

    box(msgbox,ACS_VLINE,ACS_HLINE);
    wattron(msgbox,settings);
    mvwprintw(msgbox,3,3,msg);

    mvwprintw(msgbox,0,1,title);
    wattron(msgbox,A_STANDOUT|COLOR_PAIR(1));
    mvwprintw(msgbox,6,16,"   OK   ");
    
    wrefresh(shadow);
    wrefresh(msgbox);
    getch();
    delwin(left); delwin(right); delwin(bottom); delwin(TITLE);
    delwin(l_path); delwin(prop); delwin(msgbox); delwin(shadow);
    endwin();
    exit(1);
}

void ERROR(char* title, char* msg, char* ecode, int y, int x, int flag)
{
    int settings = A_NORMAL; //default settings
    int i,j;
    settings |= flag; //additional settings
    msgbox = newwin(9,39,y,x); //create msgbox
    shadow = newwin(9,39,y+1,x+2); //create shadow of msgbox
    wbkgd(msgbox,COLOR_PAIR(5)); //COLOR_WHITE + COLOR_RED
    wbkgd(shadow,COLOR_PAIR(3)); //COLOR_WHITE + COLOR+BLACK
    noecho();
    box(msgbox,ACS_VLINE,ACS_HLINE);
    wattron(msgbox,settings);
    mvwprintw(msgbox,3,3,msg);
    mvwprintw(msgbox,4,3,ecode);
    mvwprintw(msgbox,0,1,title);
    wattron(msgbox,A_STANDOUT|COLOR_PAIR(1));
    mvwprintw(msgbox,6,16,"   OK   ");
    wrefresh(shadow);
    wrefresh(msgbox);
    getch();    
    delwin(msgbox);
    delwin(shadow);
    REFRESH();
}

void progress(char* title, char* msg, int y, int x, int flag)
{
    int settings = A_NORMAL; //default settings
    int i,j;
    settings |= flag; //additional settings
    msgbox = newwin(9,55,y,x); //create msgbox
    shadow = newwin(9,55,y+1,x+2); //create shadow of msgbox
    wbkgd(msgbox,COLOR_PAIR(2)); //COLOR_WHITE + COLOR_CYAN
    wbkgd(shadow,COLOR_PAIR(3)); //COLOR_WHITE + COLOR+BLACK

    box(msgbox,ACS_VLINE,ACS_HLINE);
    wattron(msgbox,settings);
    mvwprintw(msgbox,2,3,msg);

    mvwprintw(msgbox,0,1,title);
    wrefresh(shadow);
    wrefresh(msgbox);
    
    /*for(i=0;i<101;i++){
        mvwprintw(msgbox,5,26,"%3d%%",i);
        wattron(msgbox,COLOR_PAIR(1));
        mvwaddch(msgbox,4,2+i/2,' ');
        wattroff(msgbox,COLOR_PAIR(1));
        usleep(30000);
        wrefresh(msgbox);
    }*/
}

void killProgress()
{
    delwin(msgbox);
    delwin(shadow);
    REFRESH();
}