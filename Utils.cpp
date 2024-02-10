
#ifndef CADILLACS_AND_DINOSAURS_GAME_UTILS_H
#define CADILLACS_AND_DINOSAURS_GAME_UTILS_H
#include "Game.h"

class Utils {

public:
    static void vector3d_zero(vector3d_t* vec);
    static float get_RandF(float a, float b);
    static int get_rand(int min, int max);
    static HFONT create_font(const wchar_t* name, int size);


};

class XorShift32 {
private:
    uint32_t state;

public:
    XorShift32(uint32_t seed) : state(seed) {}

    float nextFloat(float min, float max);
};







#endif
