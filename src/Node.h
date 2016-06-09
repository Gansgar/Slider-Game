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

#ifndef SLIDER_GAME_EXCERISE_HISTORY_H
#define SLIDER_GAME_EXCERISE_HISTORY_H


#include <string>
#include <iostream>
#include <cstdlib>
#include "GameField.h"

#define WNH(node) node.getW(), node.getH()

#define NULL_MV 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define U UP
#define D DOWN
#define L LEFT
#define R RIGHT

class Node : public GameField {
private:
    Node* p = nullptr;
    bool end = false;

public:

    Node(Node& parent, unsigned int w, unsigned int h):GameField(parent, w,h), p(&parent) {}

    Node(const GameField& parent, unsigned int mW, unsigned int mH) : GameField(parent, mW,mH), p(nullptr) { }

    const Node* getParent() const {
        if(!end)
            return p;
        return nullptr;
    }

    unsigned char getMove() const {
        return lastAcc;
    }

    std::string to_string() const {
        return to_string(false);
    }

    std::string to_string(bool l) const {
        switch (lastAcc) {
            case UP:
                return l?"Up":"U";
            case DOWN:
                return l?"Down":"D";
            case LEFT:
                return l?"Left":"L";
            case RIGHT:
                return l?"Right":"R";
            default:
                return l?"None":"";
        }
    }

    void fullDel() {
        if(p && !end)
            p->fullDel();
        delete(this);
    }

    void setEnd(bool e) {
        end = e;
    }

    bool isEnd() const {
        return end;
    }
};


#endif //SLIDER_GAME_EXCERISE_HISTORY_H
