
#ifndef CADILLACS_AND_DINOSAURS_GAME_CHARACTER_H
#define CADILLACS_AND_DINOSAURS_GAME_CHARACTER_H
#include "Game.h"

class Character {


public:
    static void player_calculate_hit_boxes(player_t* player);
    static void enemy_calculate_hit_boxes(enemy_t* enemy) ;
    static void character_get_center(character_t* character, vector3d_t* center);
};


#endif
