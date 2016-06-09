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

#include <ncurses.h>
#include <cstdlib>
#include <iostream>
#include <future>
#include <cmath>
#include "SliderGUI.h"
#include "IDA_Star.h"
#include "IDAS_Multithread.h"


/* A static variable. Responsible for holding the
 * inputed rezise, when the current instant is closed */
int SliderGUI::new_size = -1;

/* This function needs to be allways triggered,
 * if I exit the program. It resets the terminal
 * to the old state and deletes all configurations
 * made by ncurses.*/
void SliderGUI::quit() {
    delwin(play_area);
    delwin(text_area);
    delwin(help_area);
    endwin();
}

/* This function inits the three windows visible
 * on the screen. I also deallocate the previous
 * screens, to stop memory leaks.
 *
 * The format of the window structure is:
 * ┌──────────────┐┌────┬┐
 * ├───────────┐  ││text││
 * │play_area  │  ││area││
 * ├───────────┘  │├────┘│
 * │              ││     │
 * │              ││     │
 * │              ││     │
 * └──────────────┘└─────┘
 * ┌──────────┬───┐
 * │help_area │   │
 * ├──────────┘   │
 * └──────────────┘
 */
void SliderGUI::init_win() {
    if (play_area)
        delwin(play_area);
    if (text_area)
        delwin(text_area);
    if (help_area)
        delwin(help_area);

    // width and height is the gamefield width / height. (Default 3x3)
    play_area = newwin(height * 4 + 1, (width * 4 + 1) * 2 - 1, 0, 0);
    text_area = newwin(100, 100, 0, (width * 4 + 1) * 2 - 1);
    help_area = newwin(height * 4 + 2, (width * 4 + 1) * 2 - 2, height * 4 + 1, 1);
}

/* Draws the playing window.
 *  - Adds border
 *  - Adds lines
 *  - Replaces crossings with right symbol
 *  - Draws the single fields.
 *     + numbers at position y/x
 *     + draws the right background color
 */
void SliderGUI::draw_win() {


    // Draw the single fields, one per one
    for (int i = 0; i < cField.getArrlen(); ++i) {
        const unsigned short num = *(cField.getField() + i);
        short color = 0; // saves the color value
        if (num != cField.getMax()) {
            std::string s = std::to_string(num);
            // Placing the text in the right position. Text is centered in it's field.
            mvwaddstr(play_area, i / width * 4 + 2, int(i%width*4*2+4-(s.size()/2 )), s.c_str());

            switch (currentMode) {
                case CHESSBOARD: // Even / Uneven
                    color = short(1 + num % 2);
                    break;
                case RAINBOW: // Sum Distance from the upper left corner. Scaled to get the full color range
                    color = short((num % width + num / width) % 7 + 10);
                    break;
                case CIRCLE: // Euklidean Distance from the center.
                    color = short(sqrt(pow(width / 2.0 - (num % width + 0.5), 2) +
                                       pow(height / 2.0 - (num / width + 0.5), 2)));
                    color = short(round(7 * color * 2.0 / sqrt(pow(width, 2) + pow(height, 2))) + 10);
                    break;
                case LAST_ITEM: // Must be included. No meaning.
                    break;
            }
        }

        // Fills up the complete field with the specfied color pair, with the normal attributes.
        for (int j = 0; j <= 3; ++j)
            mvwchgat(play_area, i / width * 4 + j, i % width * 8 + 1, 7, A_NORMAL, color, NULL);
    }

    // Draws the box around play_area
    box(play_area, 0, 0);

    // Draws lines, that seperate the fields.
    for (int i = 1; i < width; i++) {
        mvwvline(play_area, 1, i * 4 * 2, 0, height * 4 + 1 - 2);
        mvwhline(play_area, i * 4, 1, 0, (height * 4 + 1) * 2 - 3);
    }

    /* Sets the right symbols for intersections. Without intersections would be:
     *
     *   |
     * --|--
     *   |   */
    for (int i = 0; i <= height; ++i) {
        for (int j = 0; j <= width; ++j) {
            int y = 4 * i;
            int x = 4 * 2 * j;
            chtype ch = ACS_PLUS;

            if (j == 0)
                ch = ACS_LTEE;
            else if (j == width)
                ch = ACS_RTEE;
            else if (i == 0)
                ch = ACS_TTEE;
            else if (i == height)
                ch = ACS_BTEE;

            // Not corners, which are printed correct before.
            if ((i > 0 || j > 0) && (i < height || j < width) && (i > 0 || j < width) && (i < height || j > 0))
                mvwaddch(play_area, y, x, ch);
        }
    }
}

// Size of my menu, constant.
#define MENU_SIZE 6

// Draws the text_area
void SliderGUI::draw_menu(int y) {
    mvwaddstr(text_area, 1, 2, "Play");
    mvwaddstr(text_area, 2, 2, "Reset");
    // If I'm playing I need to activate help. Shuffle is senseless
    mvwaddstr(text_area, 3, 2, playing ? "Help" : "Shuffle");
    mvwaddstr(text_area, 4, 2, "Switch Colors");
    mvwaddstr(text_area, 5, 2, "Resize");
    mvwaddstr(text_area, 6, 2, "Quit");

    // Draws the cursor on the right item.
    mvwaddch(text_area, y + 1, 1, '>');
}

// Actions for specific menu item.
void SliderGUI::pro_menu(int y, bool act) {
    // If not activated, return.
    if (!act)
        return;

    switch (y) {
        case 0: // Play
            /* - Shuffels field
             * - deactivate the live help
             * - Sets the playing variable to true
             * - resets the user moves to 0 */
            gen_mv = cField.shuffle();
            help = false;
            playing = true;
            user_mv = 0;
            break;
        case 1: // Reset
            /* - Reset field to original
             * - Sets play to false
             * - Resets user moves */
            cField.reset();
            playing = false;
            user_mv = 0;
            break;
        case 2: // Help or Shuffle
            /* if Playing:
             *   - sets the current help move (number of moves from the A* position) to 0
             *   - help to true
             * if not Playing:
             *   - shuffels field
             *   - user moves to 0
             *   - see case 0 for comparrision */
            if (playing) {
                current_hmv = 0;
                help = true;
            } else {
                gen_mv = cField.shuffle();
                user_mv = 0;
            }
            break;
        case 3: // Switch Colors
            /* Toggles through all colors.
             * The index of LAST_ITEM is equal to the
             * number of all elements inside the enum.
             * Works instead of Javas length method. */
            currentMode = COLOR_MODE((currentMode + 1) % LAST_ITEM);
            break;
        case 4: // Resize
            /* Shows the resize menu.
             * If the user accepts new_size will be greater = than 1. */
            new_size = resize_menu();
            if (new_size < 1)
                break;
            // Needs to be called to do attribute reset.
            quit();
            // Stops the code.
            throw std::runtime_error("Stop code");
        case 5: // Quit
            /* Quit and Stops the code by throwing an error. */
            quit();
            throw std::runtime_error("Stop code");
        default:
            break;
    }
}

/* Creates the to the user visible resize menu.
 * Menu looks like:
 * +-----------------------+
 * | New Size: 3_          |
 * |                       |
 * |  Cancel           OK  |
 * +-----------------------+
 *
 * If the user accepts, the function will
 * return the input as long as it is bigger
 * than 1, or will return -1 if not. The
 * latter will also be triggered, if the user
 * cancels the menu.
 */
int SliderGUI::resize_menu() {
    int c = 0; // character buffer
    int i = 0; // current iteration
    /* Creates a new window that is keypad
     * enabled and that has waits for user
     * input. */
    WINDOW *resize_win = newwin(5, 30, 2, 5);
    nodelay(resize_win, false);
    keypad(resize_win, true);

    // The current inputed number. Default is 3.
    int input_num = 3;
    /* If the user has selected OK or Cancel.
     * In default Cancel is selected. */
    bool ok = false;


    goto draw;
    while (true) {
        c = wgetch(resize_win);
        i++;
        switch (c) {
            case 10:
                if (input_num > 1 && ok)
                    return input_num;
                return -1;
            case 'Q':
                quit();
                throw std::runtime_error("Stop code");
            case 127:
                input_num /= 10;
                break;
            case KEY_RIGHT:
            case KEY_LEFT:
                ok = !ok;
                break;
            default:
                if (c >= '0' && c <= '9')
                    input_num = input_num * 10 + (c - '0');
                break;
        }
        draw:
        wclear(resize_win);
        wborder(resize_win, '|', '|', '-', '-', '+', '+', '+', '+');
        if (_DEBUG)
            mvwaddstr(resize_win, 0, 0, (std::to_string(i) + " | " + std::to_string(c)).c_str());
        mvwaddstr(resize_win, 1, 2, "New size: ");
        if (input_num > 0)
            waddstr(resize_win, std::to_string(input_num).c_str());
        wattron(resize_win, A_BLINK);
        waddch(resize_win, '_');
        wattroff(resize_win, A_BLINK);


        for (int j = 0; j < 2; ++j) {
            if (ok == j)
                wattron(resize_win, A_REVERSE);

            if (j == 0)
                mvwaddstr(resize_win, 3, 3, " CANCEL ");
            else
                mvwaddstr(resize_win, 3, 23, " OK ");

            wattroff(resize_win, A_REVERSE);
        }

        wrefresh(resize_win);
    }
}

SliderGUI::~SliderGUI() {
    quit();
}

SliderGUI::SliderGUI(unsigned int w, unsigned int h, bool debug) : cField(GameField(w, h)) {
    _DEBUG = debug;
    SliderGUI(w, h);
}


const Node *SliderGUI::getLast(int i) {
    const Node *c = IDA_Star::end;
    int count = 0;
    while (!c->isEnd()) {
        c = c->getParent();
        count++;
    }

    c = IDA_Star::end;
    for (int j = 0; j < (count - i); ++j)
        c = c->getParent();

    return c;
}


SliderGUI::SliderGUI(unsigned int w, unsigned int h) : width(w), height(h), cField(GameField(w, h)) {
    new_size = -1;
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    clear();
    refresh();
    // INIT COlORS
    start_color();
    use_default_colors();
    init_pair(0, -1, -1);
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    signal(SIGINT, IDAS_Multithread::SIGINT_handler);
    //getch();

    for (short i = 1; i <= 7; i++)
        init_pair(short(9 + i), -1, i);

    init_pair(3, -1, COLOR_BLUE);

    init_win();

    int c = 0, val = 1;
    int y_menu = 0;
    bool change = true, redraw = true;

    goto draw;
    while ((c = getch()) != 'Q') {
        change = redraw = false;
        switch (c) {
            case KEY_UP:
                y_menu = (y_menu - 1) % MENU_SIZE;
                if (y_menu < 0)
                    y_menu = MENU_SIZE - 1;
                redraw = true;
                break;
            case KEY_DOWN:
                y_menu = (y_menu + 1) % MENU_SIZE;
                redraw = true;
                break;

            case 'w':
                if (IDA_Star::end && !IDAS_Multithread::stillRunning() && help && playing &&
                    getLast(current_hmv + 1)->lastAcc != U)
                    break;
                redraw = cField.mvU() == GOOD;
                change = redraw && !playing;
                user_mv += redraw;
                current_hmv++;
                break;
            case 's':
                if (IDA_Star::end && !IDAS_Multithread::stillRunning() && help && playing &&
                    getLast(current_hmv + 1)->lastAcc != D)
                    break;
                redraw = cField.mvD() == GOOD;
                change = redraw && !playing;
                user_mv += redraw;
                current_hmv++;

                break;
            case 'a':
                if (IDA_Star::end && !IDAS_Multithread::stillRunning() && help && playing &&
                    getLast(current_hmv + 1)->lastAcc != L)
                    break;
                redraw = cField.mvL() == GOOD;
                change = redraw && !playing;
                user_mv += redraw;
                current_hmv++;

                break;
            case 'd':
                if (IDA_Star::end && !IDAS_Multithread::stillRunning() && help && playing &&
                    getLast(current_hmv + 1)->lastAcc != R)
                    break;
                redraw = cField.mvR() == GOOD;
                change = redraw && !playing;
                user_mv += redraw;
                current_hmv++;

                break;
            case 'S':
                change = true;
                gen_mv = cField.shuffle();
                user_mv = 0;
                break;
            case 'R':
                if (!IDA_Star::is_goal(cField.getField())) {
                    change = true;
                    cField.reset();
                }
                user_mv = 0;
                redraw = true;
                break;
            case 10:
                pro_menu(y_menu, true);
                change = true;
                redraw = true;
                break;
            default:
                break;
        }

        if (playing && IDA_Star::is_goal(cField.getField())) {
            playing = false;
            change = true;
        }

        if (!playing) {
            help = false;
            current_hmv = 0;
        }

        draw:
        if (change) {
            IDAS_Multithread::start_calc(cField, w, h);
            val = 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        if (val != IDAS_Multithread::getRet() && !IDAS_Multithread::stillRunning())
            redraw = true;
        val = IDAS_Multithread::getRet();

        if (redraw) {
            wclear(text_area);
            wclear(play_area);
            wclear(help_area);
            draw_win();
            wrefresh(play_area);

            draw_menu(y_menu);

            mvwaddstr(text_area, MENU_SIZE + 3, 2, "Stats:");
            mvwaddstr(text_area, MENU_SIZE + 4, 3, "Shuffled with: ");
            if (gen_mv > 0)
                waddstr(text_area, std::to_string(gen_mv).c_str());
            else
                waddstr(text_area, "-");
            mvwaddstr(text_area, MENU_SIZE + 5, 3, "Min moves: ");
            if (val < 1)
                waddstr(text_area, std::to_string(-val).c_str());
            else
                waddstr(text_area, "Wait...");
            mvwaddstr(text_area, MENU_SIZE + 6, 3, ("Your moves: " + std::to_string(user_mv)).c_str());

            if (!playing || help) {
                mvwaddstr(help_area, 0, 0, "Next move: \n");
                if (!IDAS_Multithread::stillRunning() && IDA_Star::end)
                    waddstr(help_area, (getLast(current_hmv + 1)->to_string(true)).c_str());
                else {
                    val = 1;
                    waddstr(help_area, "In calculation");
                }
            } else {
                mvwaddstr(help_area, 0, 0, "You're currently playing");
            }

            wrefresh(text_area);
            wrefresh(help_area);

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }


}