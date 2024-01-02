#include "Player.h"
#include "Sprite.h"


player_t * Player :: player_new() {

    auto player = make_unique<player_t>();
    player->sprite = Sprite_Handle::sprite_new("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/jack.png", 120, 110, 1.2);
    player->hit_recover_time = 0.0f;
    player->movement_speed = 2.7;
    player->score = 0;
    player->direction = CHARACTER_DIRECTION_RIGHT;
    player->state = 0;
    player->prev_state = 0;
    player->health = 100.0f;
    player->visible = 1;
    player->on_ground = 1;
    player->hit_reset = 0;
    player->hit_streak = 0;
    player->position.x = 350;
    player->position.y = game->ground_y;
    player->hit_boxes.resize(10);
    player->hit_box_count = 0;
    animation_t *a;
    a = Animation_Handle :: sprite_add_animation(player->sprite, "idle", 0, 0, 0);
    a->speed = 1.2f;
    a = Animation_Handle::sprite_add_animation(player->sprite, "punch", 13, 15, 1);
    a->speed = 3.5f;
    a->play_once = 1;
    player->anim_upper_cut.push_back(*Animation_Handle::sprite_add_animation(player->sprite, "punch2", 18, 21, 1));
    player->anim_upper_cut.back().speed = 1.5f;



}