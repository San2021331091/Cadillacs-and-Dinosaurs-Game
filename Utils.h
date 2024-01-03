#ifndef CADILLACS_AND_DINOSAUR_GAME_UTILS_H
#define CADILLACS_AND_DINOSAUR_GAME_UTILS_H

#include "Game.h"

class Utils {



public:


    static void vector3d_zero(vector3d_t * vec);
    static int get_rand(int min, int max);
    static float get_Random(float a, float b);
    static Font create_font(const string &name, unsigned int size);








};


#endif
