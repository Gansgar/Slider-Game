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

#include <cstdlib>
#include <time.h>
#include <iostream>
#include "GameField.h"
#include "Node.h"

#define x props.cX
#define y props.cY

GameField::GameField(unsigned int w, unsigned int h) {
    props = {.width = w, .height = h, .arr_len = w*h};
    field = (unsigned short*)std::malloc(sizeof(short)*w*h);

    for(size_t i=0; i<props.arr_len; ++i) {
        *(field+i) = (unsigned short)(i);
    }
    //*(field+props.arr_len-1) = -1;
    props.MAX = *(field+props.arr_len-1);
    x = w-1;
    y = h-1;
}

GameField::GameField(const GameField& parent, unsigned int w, unsigned int h) : GameField(w,h) {
    free(field);
    props = parent.props;
    field = (unsigned short*)std::malloc(sizeof(short)*props.width*props.height);
    std::memcpy(field, parent.field, sizeof(short)*w*h);
}

GameField::~GameField() {
    free (field);
}

int GameField::mvU() {
    if( y==0 )
        return BAD;
    switchN(x+y*props.width, x+(y-1)*props.width);
    y--;
    lastAcc = UP;
    return GOOD;
}

int GameField::mvD() {
    if (y>=props.height-1)
        return BAD;
    switchN(x+y*props.width, x+(y+1)*props.width);
    y++;
    lastAcc = DOWN;
    return GOOD;
}

int GameField::mvL() {
    if (x==0)
        return BAD;
    switchN(x+y*props.width, x+y*props.width-1);
    x--;
    lastAcc = LEFT;
    return GOOD;
}

int GameField::mvR() {
    if (x>=props.width-1)
        return BAD;
    switchN(x+y*props.width, x+y*props.width+1);
    x++;
    lastAcc = RIGHT;
    return GOOD;
}


void GameField::switchN(unsigned int xy1, unsigned int xy2) {
    unsigned short tmp = *(field+(xy1));
    *(field+(xy1)) = *(field+xy2);
    *(field+(xy2)) = tmp;
}

void GameField::reset() {
    for(size_t i=0; i<props.arr_len; ++i)
        *(field+i) = (unsigned short)(i);
    x = props.width-1;
    y = props.height-1;
}

int GameField::shuffle() {
    return shuffle((unsigned int)time(NULL));
}

int GameField::shuffle(unsigned int seed) {
    srand(seed);
    unsigned int i = (unsigned int)rand()%1000;
    for(int n=i;n>0;--n) {
        unsigned int m = (unsigned int)rand()%4;
        switch (m) {
            case 0:
                if(mvU() == GOOD)
                    break;
            case 1:
                if(mvD() == GOOD)
                    break;
            case 2:
                if(mvR() == GOOD)
                    break;
            case 3:
                if(mvL() == GOOD)
                    break;
            default:
                break;
        }
    }
    return i;
}

bool GameField::isInverse(unsigned char acc) {
    return (acc == U && lastAcc == D) ||
            (acc == D && lastAcc == U) ||
            (acc == L && lastAcc == R) ||
            (acc == R && lastAcc == L);
}

unsigned short* GameField::getField() const {
    return field;
}

size_t GameField::getArrlen() {
    return props.arr_len;
}

unsigned short GameField::getMax() const {
    return props.MAX;
}

