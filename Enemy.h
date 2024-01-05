#ifndef CADILLACS_AND_DINOSAUR_GAME_ENEMY_H
#define CADILLACS_AND_DINOSAUR_GAME_ENEMY_H

#include "Game.h"


class Enemy {


public:

    static enemy_t * enemy_spawn(int x, int z, int type);
    static void enemy_draw(enemy_t* enemy,RenderWindow &window);
    static void enemy_update(enemy_t* enemy, float dt);
    static void enemy_cleanup(enemy_t* enemy);
    static void enemy_do_ai(enemy_t* enemy, float dt);




};


#endif
