#include "Player.h"
#include "Sprite.h"


player_t * Player :: player_new() {

    auto player = make_unique<player_t>();
    player->sprite = Sprite_Handle::sprite_new("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/player-jack.png", 120, 110, 1.2);
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
    vector3d_zero(&player->position);
    vector3d_zero(&player->velocity);
    animation_t *a;
    Animation_Handle :: sprite_add_animation(player->sprite, "idle", 0, 0, 0);
    a = Animation_Handle :: sprite_add_animation(player->sprite, "walk", 1, 12, 0);
    a->speed = 1.2f;
    a = Animation_Handle::sprite_add_animation(player->sprite, "punch", 13, 15, 1);
    a->speed = 3.5f;
    a->play_once = 1;
    player->anim_upper_cut.push_back(*Animation_Handle::sprite_add_animation(player->sprite, "punch2", 18, 21, 1));
    player->anim_upper_cut.back().speed = 1.5f;
    player->anim_upper_cut.back().play_once = 1;
    Animation_Handle :: sprite_add_animation(player->sprite, "hit", 16, 16, 0);
    a = Animation_Handle::sprite_add_animation(player->sprite, "fall", 30, 35, 0);
    a->speed = 0.5f;
    a->play_once = 1;
    a =Animation_Handle:: sprite_add_animation(player->sprite, "jump", 24, 26, 1);
    a->speed = 1.3f;
    a->play_once = 1;
    a = Animation_Handle::sprite_add_animation(player->sprite, "win", 27, 29, 0);
    a->speed = 0.5f;
    a->play_once = 1;

    player_set_state(player.get(), CHARACTER_STATE_IDLE);

    return player.release();

}


void Player :: player_delete(player_t* player) {
    for (auto& sprite : player->sprite) {
        Sprite_Handle :: sprite_delete(&sprite);
    }
    delete player;
}


void Player :: player_draw(player_t* player) {
    if (player->visible == 1) {
        Sprite_Handle :: sprite_draw(player->sprite, &player->position, player->direction);
    }
}

void Player ::player_set_state(player_t *player, int state) {

           character_t *ch = player;
           vector<sprite_t> * sprites = &(ch->sprite);

    for (auto &sprite : *sprites) {

        if (ch->state == CHARACTER_STATE_FIGHT) {
            animation_t* anim = Animation_Handle::sprite_current_animation(&sprite);
            if (!anim->animation_ended) {
                return;
            }
        }

        if (player->prev_state == CHARACTER_STATE_HIT_FALL) {
            return;
        }


        switch(state) {
            case CHARACTER_STATE_IDLE:
                Animation_Handle::sprite_set_animation(player->sprite, "idle");
                break;
            case CHARACTER_STATE_WALKING:
                Animation_Handle::sprite_set_animation(player->sprite, "walk");
                break;
            case CHARACTER_STATE_JUMP:
                Animation_Handle::sprite_set_animation(player->sprite, "jump");
                if (ch->on_ground) {
                    ch->velocity.y = -18.0f;
                }
                break;
            case CHARACTER_STATE_FIGHT:
                // if we are about to punch 3rd time in a row
                if (player->hit_streak >= 2) {
                    Animation_Handle::sprite_set_animation(player->sprite, "punch2");
                } else {
                    Animation_Handle::sprite_set_animation(player->sprite, "punch");
                }
                break;
            case CHARACTER_STATE_HIT:
                Animation_Handle::sprite_set_animation(player->sprite, "hit");
                if (player->state != state) {
                    player->hit_recover_time = 10;
                }
                break;
            case CHARACTER_STATE_HIT_FALL:
                Animation_Handle::sprite_set_animation(player->sprite, "fall");
                if (player->state != state) {
                    player->hit_recover_time = 300;
                }
                break;
            case CHARACTER_STATE_WIN:
                Animation_Handle::sprite_set_animation(player->sprite, "win");
                break;
        }

        if (player->state != state) {
            player->prev_state = player->state;
            player->state = state;
        }

    }


}

void Player :: player_update_state(player_t *player, float dt) {

    character_t *ch = player;
    vector<sprite_t> *sprites = &(ch->sprite);

    for (auto &sprite: *sprites) {


        if (ch->state != CHARACTER_STATE_HIT_FALL) {
            if (ch->health < 1) {
                player_set_state(player, CHARACTER_STATE_HIT_FALL);
            }
        }
        if (ch->state == CHARACTER_STATE_FIGHT) {
            animation_t *anim = Animation_Handle::sprite_current_animation(&sprite);
            if (anim->animation_ended) {
                player_set_state(player, CHARACTER_STATE_IDLE);
            }
        } else if (ch->state == CHARACTER_STATE_HIT) {
            if (player->hit_recover_time <= 0) {
                player_set_state(player, CHARACTER_STATE_IDLE);
            }
            player->hit_recover_time -= dt;
        } else if (ch->state == CHARACTER_STATE_HIT_FALL) {
            if (player->hit_recover_time <= 0) {
                game->state = GAME_STATE_OVER;
            } else {
                player->hit_recover_time -= dt;
            }


        }
        else if (ch->state == CHARACTER_STATE_JUMP) {
            animation_t* anim = Animation_Handle::sprite_current_animation(&sprite);

            if (anim->animation_ended) {
                player_set_state(player, CHARACTER_STATE_IDLE);
                ch->velocity.y = 0;
            }
        }


    }
}



void Player::player_update(player_t* player, float dt) {
    sf::Vector2u windowSize = game->window.getSize();

    character_t* ch = player;

    if (ch->velocity.x > 0)
        ch->direction = CHARACTER_DIRECTION_RIGHT;
    else if (ch->velocity.x < 0)
        ch->direction = CHARACTER_DIRECTION_LEFT;

    Sprite_Handle::sprite_update(ch->sprite, dt);

    Player::player_update_state(player, dt);

    if (game->state == GAME_STATE_PLAYING) {
        vector3d_t old_pos = ch->position;

        ch->velocity.y += game->gravity * dt;

        ch->position.x += ch->velocity.x * dt;
        ch->position.y += ch->velocity.y * dt;
        ch->position.z += ch->velocity.z * dt;

        if (ch->position.z < game->min_z || ch->position.z > game->max_z) {
            ch->position.z = old_pos.z;
        }

        if (ch->position.y >= game->ground_y) {
            ch->position.y = game->ground_y;
            ch->velocity.y = 0;
            ch->on_ground = 1;
        } else {
            ch->on_ground = 0;
        }

        Player :: player_calculate_hit_boxes(player);

        FloatRect viewport = game->window.getView().getViewport();
        float left = viewport.left * static_cast<float>(windowSize.x);
        float width = viewport.width * static_cast<float>(windowSize.x);

        if (ch->position.x >= left + width / 2) {
            game->view_x = ch->position.x - width / 2;
        }

        if (ch->position.x <= left + width / 4) {
            game->view_x = ch->position.x - width / 4;
        }

        if (game->view_x > game->max_view_x) {
            game->view_x = game->max_view_x;
        }

        if (game->view_x < 0) {
            game->view_x = 0;
        }

        game->view_x_far = game->view_x * 0.6f;
    }
}
