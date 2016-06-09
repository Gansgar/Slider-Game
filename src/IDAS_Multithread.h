/*
 * MIT License
 *
 * Copyright (c) 2016 Georg A. Friedrich
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
