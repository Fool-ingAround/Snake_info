#ifndef TIMER_H
#define TIMER_H
#include <ctime>
#include <pdcurses.h>

class timer {
protected:
     static int seconds;
     static int paused_seconds;
     static time_t start;
     static time_t pause;
     static bool is_paused;
public:
    timer(int minutes);
    static void pause_timer();
    static void resume_timer();
    static int time_left();
    static bool time_out();
    static void display(WINDOW* window, int y, int x);
};



#endif //TIMER_H
