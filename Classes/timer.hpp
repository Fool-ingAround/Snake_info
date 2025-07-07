#ifndef TIMER_H
#define TIMER_H
#include <ctime>
#include <pdcurses.h>

class timer {
protected:
    static const int seconds = 180;
    static time_t start;
public:
    timer();

    int time_left() const;
    bool time_out() const;
    void display(WINDOW* window, int y, int x) const;
};



#endif //TIMER_H
