#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <ncurses.h>
#include <wchar.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
WINDOW *map_window;
WINDOW *info_window;
WINDOW *legend_window;

#define MAP_X 37
#define MAP_Y 33
#define INFO_X 50
#define INFO_Y 21
#define DEFOULT 5
#define O_COIN 1
#define O_TRESURE 2
#define O_BIG_TRESURE 3
#define O_DROP_ 4
int g_round=0;
const wchar_t map[]={' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,'#','#',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ','#','#',' ',' ',' ',' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,'A','A',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#','#',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ','#',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,'#','#','#',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,'#','#',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ','#',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,'#','#',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ','#',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' ',' ',' ',' ',' ',' ',' '|A_REVERSE,' ',' ',' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE,' '|A_REVERSE};
struct position{
    int x;
    int y;
};
struct player{
    struct position current;
    struct position respawn;
    unsigned int coins_amount_collected;
    unsigned int coins_actually_carry;
    unsigned int deaths;
    int is_in_bush;

}players[4];
struct coin{
    struct position current;
    bool is_collected;
    unsigned int coin_type;
    unsigned int amount;

};
struct beast{
    struct position current;
};
struct sendBeast{
    struct beast *t_beast;
    struct coin *t_coin;
    sem_t beast_wait;
    sem_t beast_go;
    bool is_in_game;
};
struct data{
    struct player t_player;
    wchar_t map[7*7];
    int key;
    sem_t wait_server;
    sem_t wait_player;
    sem_t wait_for_online;
    sem_t wait_for_thread_server;
    sem_t jeszcze_jeden;
    int PID_server;
    int PID;
    int type;
    sem_t is_online_still;
    bool is_online;
    int round;
    int id;
}*sendData[4];
struct data_int{
    struct data *t_data;
    int i;
    struct coin *co;
    int *p_i;
};
void init_curses_mode()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(500);

    map_window = newwin(MAP_Y,MAP_X,0,0);
    info_window = newwin(INFO_Y,INFO_X,0,MAP_X+2);
    legend_window = newwin(MAP_Y-INFO_Y,INFO_X,INFO_Y,MAP_X+2);

}
void refresh_windows()
{
    refresh();
    wrefresh(map_window);
    wrefresh(info_window);
    wrefresh(legend_window);
}
void print_map()
{
    for(int y=0;y<MAP_Y;y++)
    {
        for(int x=0;x<MAP_X;x++)
        {
            mvwaddch(map_window,y,x,map[y*MAP_X+x]);
        }
    }
}
void print_info()
{
    int i=0;
    box(info_window,i,0);
    mvwprintw(info_window,++i,2,"Server's PID: %d",getpid());
    mvwprintw(info_window,++i,2,"Campsite X/Y: 19/13 | 20/13",1,1);
    mvwprintw(info_window,++i,2,"Round number: %d",g_round+1);

    mvwprintw(info_window,++i,2,"Parameter:   Player1  Player2  Player3  Player4");
    mvwprintw(info_window,++i,2,"PID         %d     %d     %d     %d",sendData[0]->PID,sendData[1]->PID,sendData[2]->PID,sendData[3]->PID);
    mvwprintw(info_window,++i,2,"Type        %s     %s     %s     %s",sendData[0]->type?"HUMAN":"-",sendData[1]->type?"HUMAN":"-",sendData[2]->type?"HUMAN":"-",sendData[3]->type?"HUMAN":"-");
    if(sendData[0]->is_online==FALSE)
    {
        mvwprintw(info_window,++i,2,"Curr X/Y    --/--");
    }
    else
    {
        mvwprintw(info_window,++i,2,"Curr X/Y    %0.2d/%0.2d",players[0].current.y,players[0].current.x);
    }
    if((sendData[1])->is_online==FALSE)
    {
        mvwprintw(info_window,i,20,"    --/--");
    }
    else
    {
        mvwprintw(info_window,i,20,"    %0.2d/%0.2d",players[1].current.y,players[1].current.x);
    }
    if((sendData[2])->is_online==FALSE)
    {
        mvwprintw(info_window,i,30,"    --/--");
    }
    else
    {
        mvwprintw(info_window,i,30,"    %0.2d/%0.2d",players[2].current.y,players[2].current.x);
    }
    if((sendData[3])->is_online==FALSE)
    {
        mvwprintw(info_window,i,39,"     --/--");
    }
    else
    {
        mvwprintw(info_window,i,39,"     %2.d/%2.d",players[3].current.y,players[3].current.x);
    }


    if(sendData[0]->is_online==FALSE)
    {
        mvwprintw(info_window,++i,2,"Deaths        -");
    }
    else
    {
        mvwprintw(info_window,++i,2,"Deaths        %d",players[0].deaths);
    }
    if((sendData[1])->is_online==FALSE)
    {
        mvwprintw(info_window,i,18,"        -");
    }
    else
    {
        mvwprintw(info_window,i,18,"        %d",players[1].deaths);
    }
    if((sendData[2])->is_online==FALSE)
    {
        mvwprintw(info_window,i,28,"        -");
    }
    else
    {
        mvwprintw(info_window,i,28,"        %d",players[2].deaths);
    }
    if((sendData[3])->is_online==FALSE)
    {
        mvwprintw(info_window,i,38,"        -");
    }
    else
    {
        mvwprintw(info_window,i,38,"        %d",players[3].deaths);
    }
    mvwprintw(info_window,++i,2,"Coins");
    mvwprintw(info_window,++i,2,"   carried   %d        %d         %d        %d    ",players[0].coins_actually_carry,players[1].coins_actually_carry,players[2].coins_actually_carry,players[3].coins_actually_carry);
    mvwprintw(info_window,++i,2,"   brought   %d        %d         %d        %d    ",players[0].coins_amount_collected,players[1].coins_amount_collected,players[2].coins_amount_collected,players[3].coins_amount_collected);

}
void print_legend()
{
    int i=0;
    box(legend_window,0,0);
    mvwprintw(legend_window,++i,2,"Legend:");
    mvwprintw(legend_window,++i,2,"WSAD - let you move");
    mvwprintw(legend_window,++i,2,"1234 - players");
    mvwaddch(legend_window,++i,2,map[0]);
    mvwprintw(legend_window,i,3,"    - wall");
    mvwprintw(legend_window,++i,2,"#    - bushes (slow down)");
    mvwprintw(legend_window,++i,2,"c    - one coin");
    mvwprintw(legend_window,++i,2,"t    - treasure (10 coins)");
    mvwprintw(legend_window,++i,2,"T    - large treasure (50 coins)");
    mvwprintw(legend_window,++i,2,"D    - dropped treasure");
    mvwprintw(legend_window,++i,2,"A    - campsite");


}
void print_coins(struct coin *t_coin,unsigned int count)
{
    for(unsigned int i=0;i<count;i++)
    {
        if((t_coin+i)->is_collected==FALSE)
        {
            switch (mvwinch(map_window,(t_coin+i)->current.y,(t_coin+i)->current.x))
            {
                case '1':
                    players[0].coins_actually_carry+=(t_coin+i)->amount;
                    (t_coin+i)->is_collected=TRUE;
                    break;
                case '2':
                    players[1].coins_actually_carry+=(t_coin+i)->amount;
                    (t_coin+i)->is_collected=TRUE;
                    break;
                case '3':
                    players[2].coins_actually_carry+=(t_coin+i)->amount;
                    (t_coin+i)->is_collected=TRUE;
                    break;
                case '4':
                    players[3].coins_actually_carry+=(t_coin+i)->amount;
                    (t_coin+i)->is_collected=TRUE;
                    break;
                default:
                    switch((t_coin+i)->coin_type)
                    {
                        case O_COIN:
                            mvwaddch(map_window,(t_coin+i)->current.y,(t_coin+i)->current.x,'c');
                            break;
                        case O_TRESURE:
                            mvwaddch(map_window,(t_coin+i)->current.y,(t_coin+i)->current.x,'t');
                            break;
                        case O_BIG_TRESURE:
                            mvwaddch(map_window,(t_coin+i)->current.y,(t_coin+i)->current.x,'T');
                            break;
                        default:
                            mvwaddch(map_window,(t_coin+i)->current.y,(t_coin+i)->current.x,'D');
                            break;
                    }
            }
        }
    }
}
void get_coins(struct coin *t_coin,unsigned int count,int coin_type)
{
    for(unsigned int i=0;i<count;i++)
    {
        one_more_time:
        (t_coin+i)->current.x=(rand()%MAP_X);
        (t_coin+i)->current.y=(rand()%MAP_Y);
        if(mvwinch(map_window,(t_coin+i)->current.y,(t_coin+i)->current.x)!=' ')
        {
            goto one_more_time;
        }
        if(coin_type==DEFOULT)
        {
            (t_coin+i)->coin_type=(unsigned int)((rand()%3)+1);
            switch((t_coin+i)->coin_type)
            {
                case O_COIN:
                    (t_coin+i)->amount=1;
                    break;
                case O_TRESURE:
                    (t_coin+i)->amount=10;
                    break;
                case O_BIG_TRESURE:
                    (t_coin+i)->amount=50;
                    break;
                default:
                    break;
            }
        }
        else
        {
            (t_coin+i)->coin_type=coin_type;
            switch((t_coin+i)->coin_type)
            {
                case O_COIN:
                    (t_coin+i)->amount=1;
                    break;
                case O_TRESURE:
                    (t_coin+i)->amount=10;
                    break;
                case O_BIG_TRESURE:
                    (t_coin+i)->amount=50;
                    break;
                default:
                    break;
            }
        }
        (t_coin+i)->is_collected=FALSE;
    }
}
void kill_player(int i,struct coin *coin)
{

    int j=0;
    while(j<40)
    {
        if(((coin+j)->is_collected)==TRUE)
        {

            if(players[i].coins_actually_carry!=0)
            {
                (coin+j)->coin_type=O_DROP_;
                (coin+j)->is_collected=FALSE;
                (coin+j)->amount=players[i].coins_actually_carry;
                players[i].coins_actually_carry=0;
                (coin+j)->current.x=players[i].current.x;
                (coin+j)->current.y=players[i].current.y;
            }
            players[i].deaths++;
            players[i].current.y=players[i].respawn.y;
            players[i].current.x=players[i].respawn.x;
            return;
        }
        j++;
    }
}
void print_players(int i,struct coin *coin)
{
            switch(mvwinch(map_window,players[i].current.y,players[i].current.x))
            {
                case 'A':
                    players[i].coins_amount_collected+=players[i].coins_actually_carry;
                    players[i].coins_actually_carry=0;
                case '#':
                    if(players[i].is_in_bush==2)
                    {
                        players[i].is_in_bush = 1; //musisz settować na 0 bo zrobieniu mova(w funkcji move)
                    }
                    else if(players[i].is_in_bush==0)
                    {
                        players[i].is_in_bush = 2;
                    }
                    break;
                default:
                    mvwprintw(map_window,players[i].current.y,players[i].current.x,"%d",i+1);
            }

}


void generate_spawn_point(int player_number)
{

    one_more_time2:
    players[player_number].respawn.x=(rand()%MAP_X);
    players[player_number].respawn.y=(rand()%MAP_Y);
    if(mvwinch(map_window,players[player_number].respawn.y,players[player_number].respawn.x)!=' ')
    {
        goto one_more_time2;
    }
    players[player_number].current.x=players[player_number].respawn.x;
    players[player_number].current.y=players[player_number].respawn.y;

}
void init_players()
{
    for(int i=0;i<4;i++)
    {
        generate_spawn_point(i);
        players[i].coins_amount_collected=0;
        players[i].coins_actually_carry=0;
        players[i].deaths=0;
        players[i].is_in_bush=0;

    }
    int p=0;
    while(p==0)
    {
        p=1;
        for(int i=0;i<4;i++)
        {
            for(int j=0; j<4;j++)
            {
                if(i!=j)
                {
                    if((players[i].current.x==players[j].current.x) && (players[i].current.y==players[j].current.y))
                    {
                        generate_spawn_point(i);
                        p=0;
                    }
                }
            }

        }
    }

}
int add_new_coin(struct coin *t_coin,int tab_size,int coin_type)
{
    for(int i=0;i<tab_size;i++)
    {
        if((t_coin+i)->is_collected==TRUE)
        {
            get_coins(t_coin+i,1,coin_type);
            return 0;
        }
    }
    return -1;
}
void destroy_curses_mode()
{
    endwin();
}
void move_player(int player_num,int c,struct coin *co)
{
        if(players[player_num].is_in_bush==1 || players[player_num].is_in_bush==0)
        {
            switch (c){
                case 'W':
                case 'w': //up
                    players[player_num].current.y--;
                    if(mvwinch(map_window,players[player_num].current.y,players[player_num].current.x)==(' '|A_REVERSE))
                    {
                        players[player_num].current.y++;
                        break;
                    }
                    if(players[player_num].is_in_bush==1)
                    {
                        players[player_num].is_in_bush=0;
                    }
                    break;
                case 'S':
                case 's': //down
                    players[player_num].current.y++;
                    if(mvwinch(map_window,players[player_num].current.y,players[player_num].current.x)==(' '|A_REVERSE))
                    {
                        players[player_num].current.y--;
                        break;
                    }
                    if(players[player_num].is_in_bush==1)
                    {
                        players[player_num].is_in_bush=0;
                    }
                    break;
                case 'D':
                case 'd': //right
                    players[player_num].current.x++;
                    if(mvwinch(map_window,players[player_num].current.y,players[player_num].current.x)==(' '|A_REVERSE))
                    {
                        players[player_num].current.x--;
                        break;
                    }
                    if(players[player_num].is_in_bush==1)
                    {
                        players[player_num].is_in_bush=0;
                    }
                    break;
                case 'A':
                case 'a': //left
                    players[player_num].current.x--;
                    if(mvwinch(map_window,players[player_num].current.y,players[player_num].current.x)==(' '|A_REVERSE))
                    {
                        players[player_num].current.x++;
                        break;
                    }
                    if(players[player_num].is_in_bush==1)
                    {
                        players[player_num].is_in_bush=0;
                    }
                    break;
                default:
                    break;

            }
            for(int i=0;i<4;i++)
            {
                if(i!=player_num)
                {
                    if((sendData[i])->is_online==TRUE)
                    {
                        if((players[i].current.x==players[player_num].current.x) && (players[i].current.y==players[player_num].current.y))
                        {
                            kill_player(player_num,co);
                            kill_player(i,co);
                        }
                    }
                }
            }
        }
}
void send_data(int player_num,struct data *t_data)
{
    for(int y=0;y<7;y++)
    {
        for(int x=0;x<7;x++)
        {
            t_data->map[y*7+x]=mvwinch(map_window,players[player_num].current.y+y-3,players[player_num].current.x+x-3);
        }
    }
    t_data->t_player=players[player_num];
}
void move_beast(struct beast *t_beast,struct coin *t_coin)
{
    int odl=5,od;
    int m_i=10;
    for(int i=0;i<4;i++)
    {
        if(sendData[i]->is_online==TRUE)
        {
            od=sqrt(pow((t_beast->current.x-players[i].current.x),2)+pow((t_beast->current.y-players[i].current.y),2));
            if(odl>od)
            {
                odl=od;
                m_i=i;
            }

        }
    }
    if(odl<=2)
    {

        if(((t_beast->current.x-players[m_i].current.x)==0) && ((t_beast->current.y-players[m_i].current.y)==0))
        {
            kill_player(m_i,t_coin);
        }
        else if(((t_beast->current.x-players[m_i].current.x)==0) && ((t_beast->current.y-players[m_i].current.y)>0))
        {
            (t_beast->current.y)--;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.y)++;
            }
        }
        else if(((t_beast->current.x-players[m_i].current.x)==0) && ((t_beast->current.y-players[m_i].current.y)<0))
        {
            (t_beast->current.y)++;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.y)--;
            }
        }
        else if(((t_beast->current.x-players[m_i].current.x)>0) && ((t_beast->current.y-players[m_i].current.y)==0))
        {
            (t_beast->current.x)--;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.x)++;
            }

        }
        else if(((t_beast->current.x-players[m_i].current.x)<0) && ((t_beast->current.y-players[m_i].current.y)==0))
        {
            (t_beast->current.x)++;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.x)--;
            }
        }
        else if(((t_beast->current.x-players[m_i].current.x)>0) && ((t_beast->current.y-players[m_i].current.y)>0))
        {
            (t_beast->current.x)--;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.x)++;
                (t_beast->current.y)--;
                if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
                {
                    (t_beast->current.y)++;
                }
            }
        }
        else if(((t_beast->current.x-players[m_i].current.x)<0) && ((t_beast->current.y-players[m_i].current.y)<0))
        {
            (t_beast->current.x)++;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.x)--;
                (t_beast->current.y)++;
                if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
                {
                    (t_beast->current.y)--;
                }
            }
        }
        else if(((t_beast->current.x-players[m_i].current.x)<0) && ((t_beast->current.y-players[m_i].current.y)>0))
        {
            (t_beast->current.y)--;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.y)++;
                (t_beast->current.x)++;
                if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
                {
                    (t_beast->current.x)--;
                }
            }
        }
        else if(((t_beast->current.x-players[m_i].current.x)>0) && ((t_beast->current.y-players[m_i].current.y)<0))
        {
            (t_beast->current.y)++;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.y)--;
                (t_beast->current.x)--;
                if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
                {
                    (t_beast->current.x)++;
                }
            }
        }
    }
    else
    {
        int move;
        onemorerand:
        move=rand()%4;
        if(move==0)
        {
            (t_beast->current.y)++;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.y)--;
                goto onemorerand;
            }
        }
        else if(move==1)
        {
            (t_beast->current.y)--;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.y)++;
                goto onemorerand;
            }
        }
        else if(move==2)
        {
            (t_beast->current.x)++;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.x)--;
                goto onemorerand;
            }
        }
        else if(move==3)
        {
            (t_beast->current.x)--;
            if(mvwinch(map_window,t_beast->current.y,t_beast->current.x)==(' '|A_REVERSE) || players[0].respawn.x==t_beast->current.x && players[0].respawn.y==t_beast->current.y ||players[1].respawn.x==t_beast->current.x && players[1].respawn.y==t_beast->current.y||players[2].respawn.x==t_beast->current.x && players[2].respawn.y==t_beast->current.y||players[3].respawn.x==t_beast->current.x && players[3].respawn.y==t_beast->current.y)
            {
                (t_beast->current.x)++;
                goto onemorerand;
            }
        }
    }
}
void *play_beast(void* arg)
{
    struct sendBeast *t_beast=(struct sendBeast*)arg;
    while(1)
    {
        sem_wait(&(t_beast->beast_wait));
        move_beast(t_beast->t_beast,t_beast->t_coin);
        sem_post(&(t_beast->beast_go));
    }
    return NULL;
}
void print_beast(struct sendBeast *b)
{
    mvwprintw(map_window,b->t_beast->current.y,b->t_beast->current.x,"*");
}
void generate_beast_xy(struct sendBeast *b)
{
    one_more_time3:
    b->t_beast->current.x=(rand()%MAP_X);
    b->t_beast->current.y=(rand()%MAP_Y);
    if(mvwinch(map_window,b->t_beast->current.y,b->t_beast->current.x)!=' ')
    {
        goto one_more_time3;
    }
    for(int i=0;i<4;i++)
    {
        if(players[i].current.x==b->t_beast->current.x && players[i].current.y==b->t_beast->current.y)
        {
            goto one_more_time3;
        }
    }
}
void *check_is_player(void *arg)
{
    struct data_int *t_data_int=(struct data_int *)arg;
    struct data *t_data=t_data_int->t_data;
    disconected:
    sem_wait(&(t_data->is_online_still));
    while(1)
    {
        if(kill(t_data->PID,0)==-1)
        {

            kill_player(t_data_int->i,t_data_int->co);
            sem_post(&((t_data)->wait_player));
            sem_post(&((t_data)->wait_server));
            sem_post(&((t_data)->wait_for_thread_server));
            sem_post(&((t_data)->jeszcze_jeden));
            *((t_data_int->p_i)+(t_data_int->i))=0;
            t_data->is_online=FALSE;

            goto disconected;
        }
    }
    return NULL;
}
void *play_player(void* arg)
{
    int i=((struct data_int*)(arg))->i;
    struct data *ptr=((struct data_int*)(arg))->t_data;
    struct coin *p_coin=((struct data_int*)(arg))->co;
    dis:
    sem_wait(&(ptr->wait_for_online));
    while(1)
    {
        sem_post(&((ptr)->wait_player));
        sem_wait(&((ptr)->jeszcze_jeden));

        if(ptr->is_online==FALSE)
        {
            goto dis;
        }

        move_player(i,(ptr)->key,p_coin);
        (ptr)->key=-1;

    }
    return NULL;
}


#define CH_UP 4
int main(){
    srand(time(0));
    int x = shm_open("p_player_1",O_CREAT | O_RDWR,0666);
    int x2 = shm_open("p_player_2",O_CREAT | O_RDWR,0666);
    int x3 = shm_open("p_player_3",O_CREAT | O_RDWR,0666);
    int x4 = shm_open("p_player_4",O_CREAT | O_RDWR,0666);
    int x5 = shm_open("p_players",O_CREAT | O_RDWR,0666);
    if(x==-1)
    {
        return 1;
    }
    if(x2==-1)
    {
        close(x);
        return 1;
    }
    if(x3==-1)
    {
        close(x);
        close(x2);
        return 1;
    }
    if(x4==-1)
    {
        close(x);
        close(x2);
        close(x3);
        return 1;
    }
    if(x5==-1)
    {
        close(x);
        close(x2);
        close(x3);
        close(x4);
        return 1;
    }
    if(ftruncate(x,sizeof(struct data))==-1)
    {
        close(x);
        shm_unlink("p_player_1");


        return 2;
    }
    if(ftruncate(x2,sizeof(struct data))==-1)
    {
        close(x);
        shm_unlink("p_player_1");
        close(x2);
        shm_unlink("p_player_2");

        return 2;
    }
    if(ftruncate(x3,sizeof(struct data))==-1)
    {
        close(x);
        shm_unlink("p_player_1");
        close(x2);
        shm_unlink("p_player_2");
        close(x3);
        shm_unlink("p_player_3");

        return 2;
    }
    if(ftruncate(x4,sizeof(struct data))==-1)
    {
        close(x);
        shm_unlink("p_player_1");
        close(x2);
        shm_unlink("p_player_2");
        close(x3);
        shm_unlink("p_player_3");
        close(x4);
        shm_unlink("p_player_4");

        return 2;
    }
    if(ftruncate(x5,sizeof(int)*4)==-1)
    {
        close(x);
        shm_unlink("p_player_1");
        close(x2);
        shm_unlink("p_player_2");
        close(x3);
        shm_unlink("p_player_3");
        close(x4);
        shm_unlink("p_player_4");
        close(x5);
        shm_unlink("p_players");

        return 2;
    }
    struct data *ptr[4];
    int g_p=0;
    ptr[0]=(struct data*)mmap(NULL,sizeof(struct data),PROT_WRITE | PROT_READ,MAP_SHARED,x,0);
    ptr[1]=(struct data*)mmap(NULL,sizeof(struct data),PROT_WRITE | PROT_READ,MAP_SHARED,x2,0);
    ptr[2]=(struct data*)mmap(NULL,sizeof(struct data),PROT_WRITE | PROT_READ,MAP_SHARED,x3,0);
    ptr[3]=(struct data*)mmap(NULL,sizeof(struct data),PROT_WRITE | PROT_READ,MAP_SHARED,x4,0);
    int *tab=(int*)mmap(NULL,sizeof(int)*4,PROT_WRITE | PROT_READ,MAP_SHARED,x5,0);
    for(int i=0;i<CH_UP;i++)
    {
        (ptr[i])->is_online=FALSE;
        sem_init(&(ptr[i])->wait_player,1,0);
        sem_init(&(ptr[i])->wait_server,1,0);
        sem_init(&(ptr[i])->wait_for_online,1,0);
        sem_init(&(ptr[i])->wait_for_thread_server,1,0);
        sem_init(&(ptr[i])->jeszcze_jeden,1,0);
        sem_init(&(ptr[i])->is_online_still,1,0);
        (ptr[i])->PID=0;
        (ptr[i])->type=0;
        (ptr[i])->PID_server=getpid();
        (ptr[i])->round=0;
        (ptr[i])->id=i;
        *(tab+i)=0;
    }
    init_curses_mode();
    struct coin t_coin[40];
    print_map();
    struct data_int a[4];
    pthread_t th1[4];
    pthread_t th2[4];
    pthread_t th_b[5];
    struct beast big_beast[5];
    struct sendBeast t_beast[5];

    for(int i=0;i<4;i++)
    {
        a[i].t_data=ptr[i];
        a[i].i=i;
        a[i].p_i=tab;
        a[i].co=t_coin;
    }
    init_players();
    get_coins(t_coin,40,DEFOULT);
    for(int i=0;i<5;i++)
    {
        sem_init(&(t_beast+i)->beast_wait,1,0);
        sem_init(&(t_beast+i)->beast_go,1,0);
        (t_beast+i)->t_beast=big_beast+i;
        (t_beast+i)->t_coin=t_coin;
        (t_beast+i)->is_in_game=FALSE;
        generate_beast_xy(t_beast+i);
    }
    t_beast->is_in_game=TRUE;
    for(int i=0; i<CH_UP;i++)
    {
            pthread_create(th1+i,NULL,play_player,(void*)(a+i));
            pthread_create(th2+i,NULL,check_is_player,(void*)(a+i));
            sendData[i]=ptr[i];

    }
    for(int i=0; i<5;i++)
    {
        pthread_create(th_b+i,NULL,play_beast,(void*)(t_beast+i));
    }
    int count=0;
    int c;
    while(1)
    {


        for(int i=0; i<CH_UP;i++)
        {
            if((ptr[i])->is_online==TRUE)
            {
                sem_wait(&((ptr[i])->wait_for_thread_server));
            }
        }
        print_map();
        print_info();
        print_legend();
        for(int i=0; i<CH_UP;i++)
        {
            if((ptr[i])->is_online==TRUE)
            {
                sem_post(&((ptr[i])->jeszcze_jeden));
            }
        }
        for(int i=0;i<5;i++)
        {
            if((t_beast+i)->is_in_game==TRUE)
            {
                sem_post(&((t_beast+i)->beast_wait));
            }
        }
        for(int i=0;i<5;i++)
        {
            if((t_beast+i)->is_in_game==TRUE)
            {
                sem_wait(&((t_beast+i)->beast_go));
            }
        }
        for(int i=0;i<CH_UP;i++)
        {
            if((ptr[i])->is_online==TRUE)
            {
                print_players(i,t_coin);
            }
        }
        print_coins(t_coin,40);
        for(int i=0;i<5;i++)
        {
            if((t_beast+i)->is_in_game==TRUE)
            {
                print_beast(t_beast+i);
            }
        }
        for(int i=0;i<CH_UP;i++)
        {
            if((ptr[i])->is_online==TRUE)
            {
                (ptr[i])->round=g_round;
                send_data(i,ptr[i]);
            }
        }
        g_p=0;
        for(int i=0;i<CH_UP;i++)
        {
            if((ptr[i])->is_online==TRUE)
            {
                g_p=1;
            }
        }
        if(g_p==1)
        {
            g_round++;
        }
        c=getch();
        for(int i=0;i<CH_UP;i++)
        {
            if((ptr[i])->is_online==TRUE)
            {
                sem_post(&((ptr[i])->wait_server));
            }
        }
        switch (c)
        {
            case 'c':
                add_new_coin(t_coin,40,O_COIN);
                break;
            case 't':
                add_new_coin(t_coin,40,O_TRESURE);
                break;
            case 'T':
                add_new_coin(t_coin,40,O_BIG_TRESURE);
                break;
            case 'b':
            case 'B':
                for(int i=0;i<5;i++)
                {
                    if((t_beast+i)->is_in_game==FALSE)
                    {
                        (t_beast+i)->is_in_game=TRUE;
                        break;
                    }
                }
                break;
            case 'q':
            case 'Q':
                goto endd;
            default:
                break;

        }
        refresh_windows();

    }
    endd:
    for(int i=0;i<4;i++)
    {
        sem_destroy(&(ptr[i])->wait_player);
        sem_destroy(&(ptr[i])->wait_server);
        sem_destroy(&(ptr[i])->wait_for_online);
        sem_destroy(&(ptr[i])->wait_for_thread_server);
        sem_destroy(&(ptr[i])->jeszcze_jeden);
        sem_destroy(&(ptr[i])->is_online_still);
    }
    for(int i=0;i<5;i++)
    {
        sem_destroy(&(t_beast+i)->beast_wait);
        sem_destroy(&(t_beast+i)->beast_go);
    }
    close(x);
    shm_unlink("p_player_1");
    close(x2);
    shm_unlink("p_player_2");
    close(x3);
    shm_unlink("p_player_3");
    close(x4);
    shm_unlink("p_player_4");
    close(x5);
    shm_unlink("p_players");
    destroy_curses_mode();
    return 0;
};
