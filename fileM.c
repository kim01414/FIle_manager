#include "system.h"

WIN left, right,bottom,TITLE;
WIN l_path,prop;
int console_x, console_y;

extern WIN msgbox;
extern int total;
int current=3;
int b_current=1;
char f_path[88]=".";
char old_path[88];
char newName[256];

int main(int argc, char* argv[])
{
    int i,j,c=0,size=0;
    int status;
    FILE *src, *dst;
    char ErrorCode[50],rtn;
    char buffer[4096];

    char SortOption[6][9] = {"NAME(UP)","NAME(DN)","SIZE(UP)","SIZE(DN)","TYPE(UP)","TYPE(DN)"};
    initialize();
    while( 1 ){
        if(argc==2 && strcmp(argv[1],"-d")==0){
            wattron(TITLE,A_STANDOUT);
            mvwprintw(TITLE,1,120,"INPUT    %3d",c);
            mvwprintw(TITLE,2,120,"CUR  %3d/%3d",current,total);
            mvwprintw(TITLE,3,120,"ERR#     %3d",E_CODE);
            wattron(TITLE,A_BLINK|COLOR_PAIR(7));
            mvwprintw(TITLE,0,120,"-DEBUG MODE-");
            wattroff(TITLE,A_STANDOUT|A_BLINK|COLOR_PAIR(7));
            wrefresh(TITLE);
        }
        c=getch();
        if(c==KEY_UP && current>0){
            current--;
            showlist();
            showprop();
        }
        else if(c==KEY_DOWN && current<total-1){
            current++;
            showlist();
            showprop();
        }
        else if(c==338 || c==339){//Page Down or Page Up
            if(c==339) current-=25;
            else if(c==338) current+=25;
 
            if(current<0) current=0;
            if(current>total-1) while(current>total-1) current-=1;

            showlist();
            showprop();
        }
        else if(c=='s'||c=='S'){
            inputbox("Search","Enter a name of file",newName, default_y,default_x,A_BOLD);
            for(i=0 ; i<total ; i++) if(strcmp(list[i].fname,newName)==0) break;
            if(i==total) popup("Search","No matched result",default_y,default_x,A_BOLD);
            else{
                current = i;
                showlist();
                showprop();
            } 
        } 
        else if(c=='t'|| c=='T'){
            status = Listbox("Sorting Option",SortOption,6,default_y,default_x,A_BOLD);
            sort(status);
            showlist();
            showprop();
        }
        else if(c=='\n'||c=='H'||c=='h'||c==263||c=='m'||c=='M'||c=='n'|| c=='N'||c=='d'|| c=='D'||c=='c'|| c=='C'||c=='v'||c=='V'){
            if( c=='\n'&& total>0){ //Change directory to selected one
                if(S_ISDIR( list[current].info.st_mode) ){
                strncpy(f_path, list[current].fname,88);
                }
                else continue;
            }
            else if(c=='H'||c=='h' ){//Change directory
                finputbox("Change directory","Enter a new path",default_y,default_fx,A_BOLD);
            }
            else if(c==263){//Backspace
                strcpy(f_path, "..");
            }
            else if((c=='m'||c=='M') && total>0){ //Rename
                echo();
                curs_set(2);
                wattron(left,COLOR_PAIR(3));
                wmove(left,current+5,5);
                wprintw(left,"                                                                                    ");
                wmove(left,current+5,5);
                wscanw(left,"%[^\n]",newName);
                if(rename(list[current].fname,newName)!=0){
                    E_CODE = errno;
                    ErrorReport(E_CODE,ErrorCode);
                    ERROR("Rename","CANNOT_RENAME_FILE",ErrorCode,LINES/2-4,COLS/2-19,A_BOLD);
                }
                noecho();
                curs_set(0);
                wbkgd(left,COLOR_PAIR(4));
            }
            else if(c=='n' || c=='N'){//New Folder
                    inputbox("New Folder","Enter a name of new folder",newName, default_y,default_x,A_BOLD);
                    if(mkdir(newName,0755)==-1){
                        E_CODE = errno;
                        ErrorReport(E_CODE,ErrorCode);
                        ERROR("New Folder","CANT_MAKE_NEW_FOLDER",ErrorCode,LINES/2-4,COLS/2-19,A_BOLD);
                    } 
            }    
            else if( (c=='d' || c=='D') && total>0){//Remove File or Folder
                    if(Qbox("Remove File/Folder",list[current].fname,default_y,default_x,A_BOLD)==0) continue;
                    status = remove(list[current].fname);
                    if(status==-1){ 
                        E_CODE = errno;
                        ErrorReport(E_CODE,ErrorCode);
                        ERROR("Remove","CANT_REMOVE_FILE_OR_FOLDER",ErrorCode,default_y,default_x,A_BOLD);
                    }
                    else popup("Remove File/Folder","Successfully removed!",default_y,default_x,A_BOLD);
            }
            else if( (c=='c'|| c=='C') && total>0){
                inputbox("Copy File","Enter a 'destination/filename'",newName,default_y,default_x,A_BOLD);
                dst = fopen(newName,"wb"); src = fopen(list[current].fname,"rb");
                if(src==NULL || dst==NULL){
                    E_CODE = errno;
                    ErrorReport(E_CODE, ErrorCode);
                    ERROR("Copy File","CANT_COPY_FILE",ErrorCode,default_y,default_x,A_BOLD);
                    continue;
                }
                progress("Copy File","Copying file...",default_y,default_px,A_BOLD);
                size=0;
                while( (status=fread(buffer,1,4096,src))>0){
                    if(status<4096) fwrite(buffer,1,status,dst);
                    else fwrite(buffer,1,4096,dst);
                    size+=status; j=(size/(int)list[current].info.st_size*100);
                    mvwprintw(msgbox,5,26,"%3d%%",j);
                    wattron(msgbox,COLOR_PAIR(1));
                    wmove(msgbox,4,2);
                    for(i=2 ; i<2+j/2+1 ; i++) waddch(msgbox,' ');
                    wattroff(msgbox,COLOR_PAIR(1));
                   refresh(); wrefresh(msgbox);
                    usleep(50000);
                }
                wattron(msgbox,A_BLINK);
                mvwaddstr(msgbox,2,3,"File was succesfully copied");
                wrefresh(msgbox);
                getch();
                killProgress();
                fclose(src); fclose(dst);
                size=0;
            }
            else if( (c=='v'|| c=='V') && total>0){
                inputbox("Move File","Enter a 'destination/filename'",newName,default_y,default_x,A_BOLD);
                if(link( list[current].fname, newName)!=0){
                    E_CODE = errno;
                    ErrorReport(E_CODE, ErrorCode);
                    ERROR("Move File","CANT_MOVE_FILE",ErrorCode,default_y,default_x,A_BOLD);
                    continue;
                }
                if(unlink( list[current].fname )!=0){
                    unlink(newName);
                    E_CODE = errno;
                    ErrorReport(E_CODE, ErrorCode);
                    ERROR("Move File","CANT_MOVE_FILE",ErrorCode,default_y,default_x,A_BOLD);
                    continue;
                }
                popup("Move File","File was succesfully moved",default_y,default_x,A_BOLD);
            }    
            if(chdir(f_path)==-1){
                E_CODE = errno;
                ErrorReport(E_CODE,ErrorCode);
                ERROR("Open DIR","CANT_OPEN_DIR",ErrorCode,default_y,default_x,A_BOLD);
                strcpy(f_path,old_path);
                REFRESH();
                continue;
            } 
            free(list);
            getcwd(f_path,81);
            current = total=0;
            do_ls(".");
            wclear(left);
            showlist();
            showprop();
        }
        else if(c=='x' || c=='X')
        {
          if( Qbox("File Manager","Are you sure want to exit?",default_y,default_x,A_BOLD) ){
              break;
          };
        } //EXIT;
        else if(c=='l'||c=='L'){ //COMMAND
            inputbox("COMMAND","Enter a command",newName,default_y,default_x,A_BOLD);
            clear();
            refresh();
            endwin();
            free(list);
            current=total=0;
            system("clear");
            system(newName);
            puts("");
            printf("FILE MANAGER> Press Any key to continue...");
            fflush(stdout);
            getch();
            system("clear");
            initialize();
        }
        else if(c==265) popup("About","Developed by KYH",LINES/2-4,COLS/2-19,A_BOLD); //F1 Press        
    }
    delwin(left); delwin(right); delwin(TITLE);
    delwin(l_path); delwin(prop); delwin(bottom);
    endwin();
    return 0;
}

void initialize()
{
    int stdX,stdY;
    int i,j=2,size=0;
    char title[] = "FILE MANAGER ver 0.1";
    char action[10][20]={"   MOVE(V)    ","   RENAME(M)  ","   COPY(C)   ","  SEARCH(S)  ","  DELETE(D)  ",
                         "   mvPATH(H)  ","   NewDIR(N)  ","   SORT(T)   ","  COMMAND(L) ","   EXIT(X)   "};
    setlocale(LC_ALL, "ko_KR.utf8");
    setlocale(LC_CTYPE, "ko_KR.utf8");
    initscr();    
    start_color();
    getcwd(f_path,100);
    list = NULL;
    keypad(left,TRUE);  keypad(right,TRUE);  keypad(TITLE,TRUE); keypad(bottom,TRUE); 
    keypad(l_path,TRUE); keypad(prop,TRUE);  keypad(stdscr,TRUE);
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    init_pair(2,COLOR_WHITE,COLOR_CYAN);
    init_pair(3,COLOR_WHITE,COLOR_BLACK);
    init_pair(4,COLOR_BLACK,COLOR_WHITE);
    init_pair(5,COLOR_WHITE,COLOR_RED);
    init_pair(6,COLOR_BLUE,COLOR_WHITE);
    init_pair(7,COLOR_RED,COLOR_WHITE);
    init_pair(8,COLOR_GREEN,COLOR_WHITE);
    getmaxyx(stdscr,stdY,stdX);
    refresh();
    if(stdX<132 || stdY<43) fatal("Terminal Size","Plz increase the size to 132 x 43",1,1,A_BOLD);//break;
    noecho();
    curs_set(0);

    do_ls(".");
////////////////////  Program Title  ///////////////////
    TITLE  = newwin(5,132,0,0);
    wattron(TITLE,A_BOLD);
    box(TITLE,ACS_VLINE,ACS_HLINE);
    mvwprintw(TITLE,2,(132-strlen(title))/2,title);
   
////////////////////  FILE LIST  //////////////////////
    left = newwin(31,90,5,0);
    box(left,ACS_VLINE,ACS_HLINE);
    wbkgd(left,COLOR_PAIR(4));
    showlist();

////////////////////  FILE Path  //////////////////////
    l_path=newwin(3,88,6,1);
    path_control(l_path,f_path);

////////////////////  FILE Prop  //////////////////////
    right= newwin(31,42,5,90);
    prop = newwin(3,40,6,91);
    wattron(prop,A_BOLD);
    wattron(prop,A_BOLD|COLOR_PAIR(3));
    wbkgd(right,COLOR_PAIR(4));
    showprop();
    
//////////////////  Bottom Menu  //////////////////////
    bottom = newwin(7,132,36,0);
    wattron(bottom,A_BOLD);
    box(bottom,ACS_VLINE,ACS_HLINE);

    for(i=0 ; i<10 ; i++)
    {
        wattron(bottom,COLOR_PAIR(4)|A_DIM);
        mvwprintw(bottom,j,8+size,action[i]);
        wattroff(bottom,COLOR_PAIR(4)|A_DIM);
        size+=15+10;
        if(i==4){
            j+=2; size=0;
        } 
    }
    mvwprintw(left,4,78,"(%3d / %3d)",current,total);
    REFRESH();
}

void REFRESH()
{
    wclear(left);
    box(left,ACS_VLINE,ACS_HLINE);
    showlist();
    showprop(3);
    refresh();
    wrefresh(TITLE); wrefresh(left); wrefresh(right);
    wrefresh(l_path); wrefresh(prop);  wrefresh(bottom);
    path_control(l_path,f_path);
}