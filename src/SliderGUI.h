//
// Created by Georg Friedrich on 13/05/16.
//


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
