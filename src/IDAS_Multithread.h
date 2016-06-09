//
// Created by Georg Friedrich on 14/05/16.
//

#ifndef SLIDER_GAME_EXCERISE_IDAS_MULTITHREAD_H
#define SLIDER_GAME_EXCERISE_IDAS_MULTITHREAD_H


#include <future>
#include <ncurses.h>
#include "GameField.h"
#include "IDA_Star.h"

class IDAS_Multithread {
private:
    static std::atomic_int moves;
    static std::future<int> ret;
    static std::atomic_int currentID;
    static std::atomic_int lastFinished;

    static int func1(GameField field, int w, int h, int id) {
        int mv = IDA_Star::start(field, w, h, id);
        if (currentID == id) {
            moves = mv;
            lastFinished = id;
        }

        return mv;
    }
public:


    static void start_calc(GameField& field, int w, int h) {
        currentID++;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ret = std::async(std::launch::async, func1, field, w, w,  int(currentID) );
    }

    static int getRet() {
        return moves;
    }

    static bool stillRunning() {
        return lastFinished!=currentID;
    }

    static void SIGINT_handler(int i) {
        IDA_Star::STOP = true;
        endwin();
        exit(0);
    }

    static int getCurrentThreadID() {
        return currentID;
    }
};


#endif //SLIDER_GAME_EXCERISE_IDAS_MULTITHREAD_H
