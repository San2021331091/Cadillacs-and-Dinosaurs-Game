
#ifndef CADILLACS_AND_DINOSAURS_GAME_SPRITE_H
#define CADILLACS_AND_DINOSAURS_GAME_SPRITE_H

#include "Game.h"

class Sprite_Handle {


public:

    static sprite_t* sprite_new(const wchar_t* filename, int width, int height, float scale);
    static void sprite_delete(sprite_t* sprite);
    static void sprite_update(sprite_t* sprite, float dt);
    static void sprite_draw(sprite_t* sprite, vector3d_t* pos, int direction, HDC hdc);
    static void sprite_draw_2(sprite_t* sprite,  draw_t* draw);

};



class Animation_Handle{



public:
    static animation_t* sprite_add_animation(sprite_t* sprite, const wchar_t* name, int start, int end, int reverse_loop);
    static animation_t* sprite_set_animation(sprite_t* sprite, const wchar_t* name);
    static animation_t* sprite_current_animation(sprite_t* sprite);



};

#endif
