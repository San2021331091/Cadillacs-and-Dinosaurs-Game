#ifndef CADILLACS_AND_DINOSAUR_GAME_PLAYER_H
#define CADILLACS_AND_DINOSAUR_GAME_PLAYER_H

#include "Game.h"

class Player {


public:
    static player_t * player_new();
    static void player_delete(player_t* player);
    static void player_draw(vector<player_t>* player);
    static void player_update_state(player_t* player, float dt);
    static void player_update(player_t* player, float dt);
    static void player_set_state(player_t* player, int state);



};


#endif
