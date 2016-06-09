//
// Created by Georg Friedrich on 13/05/16.
//

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
