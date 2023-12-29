#ifndef CADILLACS_AND_DINOSAUR_GAME_SPRITE_H
#define CADILLACS_AND_DINOSAUR_GAME_SPRITE_H

#include "Game.h"

class Sprite {

public:
    sprite_t *sprite_new(wstring &filename, int width, int height, float scale);
    void sprite_delete( sprite_t* sprite);
    void sprite_update( sprite_t* sprite, float dt);
    void sprite_draw(sprite_t* sprite, vector3d_t* pos, int direction, RenderWindow &window);


};


class Animation{


public:
    animation_t * sprite_add_animation(sprite_t* sprite, wstring &name, int start, int end, int reverse_loop);
    animation_t * sprite_set_animation(sprite_t* sprite, wstring &name);
    animation_t * sprite_current_animation(sprite_t* sprite);
    animation_t* sprite_set_Animation(sprite_t* sprite, wstring &name);
    animation_t* sprite_current_Animation(sprite_t* sprite);


};



#endif
