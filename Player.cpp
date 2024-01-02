#include "Player.h"
#include "Sprite.h"


player_t * Player :: player_new() {

    auto player = make_unique<player_t>();
    player->sprite = Sprite_Handle::sprite_new("C:/Projects/mostafa-the-game/bin/jack.bmp", 120, 110, 1.2);
    player->hit_recover_time = 0.0f;
    player->movement_speed = 2.7;
    player->score = 0;
    player->direction = CHARACTER_DIRECTION_RIGHT;
    player->state = 0;
    player->prev_state = 0;
    player->health = 100.0f;
    player->visible = 1;
    player->on_ground = 1;
   // vector3d_zero(&player->position);
    //vector3d_zero(&player->velocity);
    player->hit_reset = 0;
    player->hit_streak = 0;
    player->position.x = 350;
    player->position.y = game->ground_y;
    player->hit_boxes.resize(10);
    player->hit_box_count = 0;
    animation_t *a;
    a = Animation_Handle :: sprite_add_animation(player->sprite, "idle", 0, 0, 0);


}