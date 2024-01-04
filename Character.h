#ifndef CADILLACS_AND_DINOSAUR_GAME_CHARACTER_H
#define CADILLACS_AND_DINOSAUR_GAME_CHARACTER_H

#include "Game.h"


class Character {

public:

    static void character_get_center(character_t* character, vector3d_t* center);
    void player_calculate_hit_boxes(player_t* player);
    static bool rect_collision(FloatRect& rect1, FloatRect& rect2);
    static void enemy_calculate_hit_boxes(enemy_t* enemy);











};


#endif
