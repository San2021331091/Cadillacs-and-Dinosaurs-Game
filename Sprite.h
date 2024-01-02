#ifndef CADILLACS_AND_DINOSAUR_GAME_SPRITE_H
#define CADILLACS_AND_DINOSAUR_GAME_SPRITE_H

#include "Game.h"

class Sprite_Handle{

public:
    static initializer_list<sprite_t> sprite_new(const string& filename, int width, int height, float scale);
    static void sprite_delete( sprite_t* sprite);
    static void sprite_update( vector<sprite_t>&sprite, float dt);
    static void sprite_draw(vector<sprite_t>& sprites,vector3d_t *pos,int direction);
    static void sprite_draw_2(sprite_t* sprite, draw_t* draw);

};


class Animation_Handle{


public:
    static animation_t * sprite_add_animation(vector<sprite_t>& sprites, const string &name, int start, int end, int reverse_loop);
   static animation_t * sprite_set_animation(vector<sprite_t>& sprites, const string &name);
   static animation_t * sprite_current_animation(sprite_t* sprite);



};






#endif
