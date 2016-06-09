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

#ifndef SLIDER_GAME_EXCERISE_SLIDERGUI_H
#define SLIDER_GAME_EXCERISE_SLIDERGUI_H

#include <ncurses.h>
#include "GameField.h"
#include "Node.h"


class SliderGUI {
    enum COLOR_MODE {
        CHESSBOARD,
        RAINBOW,
        CIRCLE,
        LAST_ITEM,
    };

private:
    void quit();
    void init_win();
    void draw_win();
    void draw_menu(int);
    void pro_menu(int, bool);
    int resize_menu();
    const Node* getLast(int);

    WINDOW *play_area;
    WINDOW *text_area;
    WINDOW *help_area;
    unsigned int width, height;

    GameField cField;

    COLOR_MODE currentMode = RAINBOW;
    bool _DEBUG = false;
    bool playing = false;
    bool help = false;
    unsigned int user_mv = 0;
    unsigned int current_hmv = 0;
    int gen_mv = -1;

public:
    static int new_size;

    SliderGUI(unsigned int w, unsigned int h, bool debug);
    SliderGUI(unsigned int w, unsigned int h);
    ~SliderGUI();
};


#endif //SLIDER_GAME_EXCERISE_SLIDERGUI_H
