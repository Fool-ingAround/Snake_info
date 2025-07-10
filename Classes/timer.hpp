#ifndef TIMER_H
#define TIMER_H
#include <ctime>
#include <pdcurses.h>

class timer {
protected:
     int seconds;
     int paused_seconds;
     time_t start;
     time_t pause;
     bool is_paused;
public:
    timer(int seconds);
    void pause_timer();
    void resume_timer();
    int time_left();
    bool time_out();
    void display(WINDOW* window, int y, int x);
};



#endif //TIMER_H
