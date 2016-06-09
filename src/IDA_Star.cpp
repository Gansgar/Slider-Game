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

#include "IDA_Star.h"
#include "Node.h"
#include "IDAS_Multithread.h"
#include <iostream>
#include <thread>

int IDA_Star::width = 0;
int IDA_Star::height = 0;
bool IDA_Star::_DEBUG = false;
bool IDA_Star::STOP = false;
Node* IDA_Star::end = nullptr;

int IDA_Star::h(GameField& cField)  { //linear conflict
    if(cField.heuristic==-1) {
        unsigned int MD = 0;
        short max = -1;

        for(int x=0; x<cField.getW(); x++) {
            max = -1;
            for(int y=0; y<cField.getH(); y++) {
                if( ITEM(x,y) == cField.getMax()  )
                    continue;

                MD += std::abs(ITEM(x,y)%(cField.getW())-x) + std::abs(ITEM(x,y)/(cField.getW())-y);
                if(ITEM(x,y)%cField.getW() == x) {
                    if( ITEM(x,y)>max )
                        max = ITEM(x,y);
                    else
                        MD += 2;
                }
            }
        }

        for(int y=0; y<cField.getH(); y++) {
            max = -1;
            for(int x=0; x<cField.getW(); x++) {
                if (ITEM(x,y)!=cField.getMax() && ITEM(x,y)/cField.getW() == y) {
                    if (ITEM(x,y)>max)
                        max = ITEM(x,y);
                    else
                        MD+=2;
                }
            }
        }

        cField.heuristic = MD;
        return MD;
    } else {
        return cField.heuristic;
    }
}

int IDA_Star::start(GameField root, unsigned int wi, unsigned int hi, const int id)  {
    width = wi;
    height = hi;

    Node *n = new Node(root, WNH(root));
    if(end) {
        end->fullDel();
        end = nullptr;
    }

    n->setEnd(true);
    root.lastAcc = '\0';
    int bound = h(root);
    while(true){
        int t = search(n, 0, bound, id);

        if(STOP) {
            STOP = false;
            return -1;
        }

        if (t <= FOUND)
            return t;
        if (t==INF)
            return NOT_FOUND;
        bound+=2;
        if(_DEBUG)
            std::cout << "New tree with bound of " << bound << " | the return of search was:" << t << std::endl;
    }

    //return h(root);
}

int IDA_Star::search(Node* node, unsigned int g, int bound, const int id) {
    unsigned int f = g+h(*node);
    if (f>bound)
        return f;
    if (is_goal(node->getField())) {
        end = node;
        return -g;
    }
    int min = INF;

    for( Node* elem : successors(*node)) {
        int t = INF;
        if(min>FOUND)
            t = search(elem, g+1, bound, id);

        if(STOP || id!=IDAS_Multithread::getCurrentThreadID())
            t = -1;

        if(_DEBUG) {
            std::cout << g << " | " << t << " | " << elem->to_string() << std::endl;
            for (int i = 0; i < width*height; ++i) {
                if (i%width==0 && i!=0)
                    std::cout << "\n";
                std::cout << *(elem->getField()+i) << " ";
            }
            std::flush(std::cout<<"\nh: " << h(*elem) << "\n\n");
        }

        if (t < min && min>FOUND)
            min = t;

        if (t <= FOUND)
            min = t;
        else
            delete(elem);
    }

    return min;
}

bool IDA_Star::is_goal(const unsigned short *board) {
    short delta = 0;
    for (int i = 0; i < width*height-1; ++i) {
        delta = *(board+i+1)-*(board+i);
        if(delta!=1)
            return false;
    }

    return true;
}

std::vector<Node*> IDA_Star::successors(Node& node) {
    std::vector<Node*> tmp;

    Node* tmpNode = new Node(node, WNH(node));
    if( tmpNode->mvD() == GOOD && node.lastAcc!=U)
        tmp.push_back(tmpNode);
    else
        delete(tmpNode);

    tmpNode = new Node(node, WNH(node));
    if( tmpNode->mvU() == GOOD && node.lastAcc!=D)
        tmp.push_back(tmpNode);
    else
        delete(tmpNode);


    tmpNode = new Node(node, WNH(node));
    if( tmpNode->mvR() == GOOD && node.lastAcc!=L)
        tmp.push_back(tmpNode);
    else
        delete(tmpNode);


    tmpNode = new Node(node, WNH(node));
    if( tmpNode->mvL() == GOOD && node.lastAcc!=R)
        tmp.push_back(tmpNode);
    else
        delete(tmpNode);



    return tmp;
}