#include "timer.hpp"
#include <ctime>
#include <pdcurses.h>

time_t timer::start = time(nullptr);
const int timer::seconds;

timer::timer() {
    start = time(nullptr);
}

int timer::time_left() const{
    time_t now = time(nullptr);
    int elapsed = static_cast<int>(difftime(now, start));
    int remaining = seconds - elapsed;
    if (remaining > 0) {
        return remaining;
    } else {
        return 0;
    }
}

bool timer::time_out() const{
    return time_left() == 0;
}

void timer::display(int y, int x, WINDOW *window) const {
    int r = time_left();
    int minutes = r / 60;
    int seconds = r % 60;
    mvwprintw(window, y, x, "%02d:%02d", minutes, seconds);
    wrefresh(window);
}

void timer::wait_one_second() const{
    time_t start = time(nullptr);
    while (time(nullptr) == start);
}