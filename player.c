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
WINDOW *game_window;
WINDOW *info_window;
WINDOW *legend_window;
#define GAME_XY 7
#define INFO_X 36
#define INFO_Y 14
int g_p=0;
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
    int PID_player;
    int type;
    sem_t is_online_still;
    bool is_online;
    int round;
    int id;
}getData;
void init_curses_mode()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(500);

    game_window = newwin(GAME_XY,GAME_XY,0,0);
    info_window = newwin(INFO_Y,INFO_X,0,GAME_XY+8);
    legend_window = newwin(INFO_Y-1,INFO_X,INFO_Y,GAME_XY+8);

}
void refresh_windows()
{
    refresh();
    wrefresh(game_window);
    wrefresh(info_window);
    wrefresh(legend_window);
}
void destroy_curses_mode()
{
    endwin();
}
void print_map()
{
    for(int y=0;y<7;y++)
    {
        for(int x=0;x<7;x++)
        {
            mvwaddch(game_window,y,x,getData.map[y*7+x]);
        }
    }
}
void print_legend()
{
    int i=0;
    box(legend_window,0,0);
    mvwprintw(legend_window,++i,2,"Legend:");
    mvwprintw(legend_window,++i,2,"WSAD - let you move");
    mvwprintw(legend_window,++i,2,"1234 - players");
    mvwaddch(legend_window,++i,2,(' '|A_REVERSE));
    mvwprintw(legend_window,i,3,"    - wall");
    mvwprintw(legend_window,++i,2,"*    - beast (will eat you)");
    mvwprintw(legend_window,++i,2,"#    - bushes (slow down)");
    mvwprintw(legend_window,++i,2,"c    - one coin");
    mvwprintw(legend_window,++i,2,"t    - treasure (10 coins)");
    mvwprintw(legend_window,++i,2,"T    - large treasure (50 coins)");
    mvwprintw(legend_window,++i,2,"D    - dropped treasure");
    mvwprintw(legend_window,++i,2,"A    - campsite");


}
void print_info(struct data* d)
{
    int i=0;
    box(info_window,i,0);
    mvwprintw(info_window,++i,2,"Server's PID: %d",d->PID_server);
    mvwprintw(info_window,++i,2,"Campsite X/Y: 19/13 | 20/13");
    mvwprintw(info_window,++i,2,"Round number: %d",d->round);
    i++;
    mvwprintw(info_window,++i,2,"Player:");
    mvwprintw(info_window,++i,2,"Number:     %d",d->id+1);
    mvwprintw(info_window,++i,2,"Type:       HUMAN");
    mvwprintw(info_window,++i,2,"Curr X/Y    %2.d/%2.d",d->t_player.current.x,d->t_player.current.y);
    mvwprintw(info_window,++i,2,"Deaths      %d",d->t_player.deaths);
    mvwprintw(info_window,++i,2,"Coins found %d     ",d->t_player.coins_actually_carry);
    mvwprintw(info_window,++i,2,"Coins brought %d   ",d->t_player.coins_amount_collected);

}
void send_key(int c,struct data *ptr)
{
    ptr->key=c;
}
void *is_serwer_online(void *arg)
{
    struct data *d=(struct data*)arg;

    while(1)
    {
        if(kill(d->PID_server,0)==-1)
        {
            sem_post(&((d)->wait_server));
            sem_post(&((d)->wait_player));
            g_p=1;
        }
    }
}
int main()
{
    int x = shm_open("p_player_1",O_RDWR,0666);
    int x2 = shm_open("p_player_2",O_RDWR,0666);
    int x3 = shm_open("p_player_3",O_RDWR,0666);
    int x4 = shm_open("p_player_4",O_RDWR,0666);
    int x5 = shm_open("p_players",O_RDWR,0666);
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
    int player_id;
    struct data *ptr[4];
    int is_serwer_full=0;
    ptr[0]=(struct data*)mmap(NULL,sizeof(struct data),PROT_WRITE | PROT_READ,MAP_SHARED,x,0);
    ptr[1]=(struct data*)mmap(NULL,sizeof(struct data),PROT_WRITE | PROT_READ,MAP_SHARED,x2,0);
    ptr[2]=(struct data*)mmap(NULL,sizeof(struct data),PROT_WRITE | PROT_READ,MAP_SHARED,x3,0);
    ptr[3]=(struct data*)mmap(NULL,sizeof(struct data),PROT_WRITE | PROT_READ,MAP_SHARED,x4,0);
    int *tab=(int*)mmap(NULL,sizeof(int)*4,PROT_WRITE | PROT_READ,MAP_SHARED,x5,0);
    for(int i=0;i<4;i++)
    {
        if(*(tab+i)==0)
        {
            *(tab+i)=1;
            player_id=i;
            ((ptr[i]))->is_online=TRUE;
            is_serwer_full=1;
            break;
        }
    }
    if(is_serwer_full==0)
    {
        printf("Server is full!\n");
        close(x);
        close(x2);
        close(x3);
        close(x4);
        close(x5);
        return 1;
    }
    init_curses_mode();
    int c;
    int count=0;
    refresh_windows();
    (ptr[player_id])->PID_player=getpid();
    (ptr[player_id])->type=1;
    pthread_t th1;
    pthread_create(&th1,NULL,is_serwer_online,(void*)(ptr[player_id]));
    sem_post(&(ptr[player_id])->wait_for_online);
    sem_post(&(ptr[player_id])->is_online_still);
    while(1)
    {
        if(g_p==1)
        {
            mvprintw(7,0,"LOST CONNECTION        ");
            mvprintw(8,0,"RESTART SERVER!!          ");
            mvprintw(9,0,"'q' let you quit     ");
        }
        getData=*(ptr[player_id]);
        sem_wait(&((ptr[player_id])->wait_player));
        c=getch();
        if(c=='q' || c=='Q')
        {
            break;
        }
        send_key(c,(ptr[player_id]));

        sem_post(&((ptr[player_id])->wait_for_thread_server));
        sem_wait(&((ptr[player_id])->wait_server));
        usleep(10000);
        print_map();
        print_info(ptr[player_id]);
        print_legend();
        refresh_windows();
        flushinp();
    }
    close(x);
    close(x2);
    close(x3);
    close(x4);
    close(x5);
    destroy_curses_mode();
    return 0;
}
