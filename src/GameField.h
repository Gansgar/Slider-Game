//
// Created by Georg Friedrich on 13/05/16.
//

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
