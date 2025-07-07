#include "timer.hpp"
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

void timer::display(WINDOW *window, int y, int x) const {
    int r = time_left();
    int minutes = r / 60;
    int seconds = r % 60;
    mvwprintw(window, y, x, "%d:%02d", minutes, seconds);
    wrefresh(window);
}
