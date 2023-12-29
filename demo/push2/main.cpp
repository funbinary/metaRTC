#include "push_ctl.h"
#include <thread>

int main() {
    PushCtl ctl;
    ctl.start();

    int count = 0;
    while (count < 100) {
        std::this_thread::sleep_for(1s);
    }
}