#ifndef CADILLACS_AND_DINOSAUR_GAME_SPRITE_H
#define CADILLACS_AND_DINOSAUR_GAME_SPRITE_H

#include "Game.h"

class Sprite_Handle{

public:
     static sprite_t *sprite_new(string &filename, int width, int height, float scale);
    static void sprite_delete( sprite_t* sprite);
    static void sprite_update( sprite_t* sprite, float dt);
    void sprite_draw(sprite_t* sprite, vector3d_t* pos, int direction, RenderWindow &window);


};


class Animation_Handle{


public:
    static animation_t * sprite_add_animation(sprite_t* sprite, wstring &name, int start, int end, int reverse_loop);
   static animation_t * sprite_set_animation(sprite_t* sprite, wstring &name);
   static animation_t * sprite_current_animation(sprite_t* sprite);



};



#endif
