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

#ifndef SLIDER_GAME_EXCERISE_IDA_STAR_H
#define SLIDER_GAME_EXCERISE_IDA_STAR_H

#include <cstdlib>
#include <vector>
#include "GameField.h"
#include "Node.h"


#define ITEM(x,y) (*(cField.getField()+x+y*width))
#define EVEN(n) (n-n%2)
#define FOUND 0
#define NOT_FOUND std::numeric_limits<int>::min()
#define INF std::numeric_limits<int>::max()

class IDA_Star {
private:
    static int width, height;

public:
    static Node* end;

    static int h(GameField& cField);
    static bool _DEBUG;
    static bool STOP;

    static int start(GameField root, unsigned int wi, unsigned int hi,const int id);
    static int search(Node* node, unsigned int g, int bound,const int id);
    static bool is_goal(const unsigned short* board);
    static std::vector<Node*> successors(Node& cField);
};


#endif //SLIDER_GAME_EXCERISE_IDA_STAR_H
