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

#ifndef SLIDER_GAME_EXCERISE_GAMEFIELD_H
#define SLIDER_GAME_EXCERISE_GAMEFIELD_H


#include <cstdio>
#include <cstdlib>
#include <histedit.h>

#define GOOD 0
#define BAD -1

#define CORD unsigned int y, unsigned int x

class GameField {
protected:
    struct FieldProperties {
        size_t arr_len;
        unsigned int width;
        unsigned int height;
        unsigned int cX;
        unsigned int cY;
        unsigned short MAX;
    };

    unsigned short *field;

    FieldProperties props;


public:
    GameField(unsigned int, unsigned int);
    GameField(const GameField& parent, unsigned int, unsigned int);
    ~GameField();
    unsigned short* getField() const;
    size_t getArrlen();
    unsigned short getMax() const;
    unsigned char lastAcc = '\0';

    int heuristic = -1;

    int mvU();
    int mvD();
    int mvL();
    int mvR();


    GameField(const GameField &ret ) {
        props = ret.props;
        field = (unsigned short*)std::malloc(sizeof(short)*props.width*props.height);
        std::memcpy(field, ret.getField(), sizeof(unsigned short)*props.width*props.height);
        lastAcc = ret.lastAcc;
    }

    void switchN(unsigned int xy1, unsigned int xy2);
    void reset();
    int shuffle();
    int shuffle(unsigned int);
    bool isInverse(unsigned char acc);

    int getW() const {
        return props.width;
    }
    int getH() const {
        return props.height;
    }
};


#endif //SLIDER_GAME_EXCERISE_GAMEFIELD_H
