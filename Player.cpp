#include "Player.h"
#include "Sprite.h"
#include "Utils.h"
#include "Character.h"
#include "FileLoad.h"
#include "Queue.h"

Queue<const wchar_t *> p;
vector<const wchar_t * > v1;

player_t* Player :: player_new(float speed) {
    auto* player = new player_t;
    player->base.sprite = Sprite_Handle::sprite_new(FileLoad::getImageFile1("player-jack.bmp"), 120, 110, 2.5);
    player->hit_recover_time = 0.0f;
    player->movement_speed = speed;
    player->score = 0;
    player->base.direction = CHARACTER_DIRECTION_RIGHT;
    player->base.state = 0;
    player->base.prev_state = 0;
    player->base.health = 100.0f;
    player->base.visible = 1;
    player->base.on_ground = 1;
    Utils::vector3d_zero(&player->base.position);
    Utils::vector3d_zero(&player->base.velocity);

    player->hit_reset = 0;
    player->hit_streak = 0;

    player->base.position.x = 350;
    player->base.position.y = game->ground_y;

    player->base.hit_boxes = new RECT [10];
    player->base.hit_box_count = 0;

    animation_t* a;

    p.enqueue(L"run");
    p.enqueue(L"walk");
    p.enqueue(L"punch");
    p.enqueue(L"punch2");
    p.enqueue(L"hit");
    p.enqueue(L"fall");
    p.enqueue(L"idle");
    p.enqueue(L"jump");
    p.enqueue(L"win");


    while (!p.isEmpty()) {
        try {
            v1.push_back(p.getFront());
            p.dequeue();
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;

        }
    }

    Animation_Handle::sprite_add_animation(player->base.sprite, v1[6], 0, 0, 0);
    a = Animation_Handle::sprite_add_animation(player->base.sprite, v1[1], 1, 12, 0);
    a->speed = 1.2f;
    a = Animation_Handle::sprite_add_animation(player->base.sprite, v1[2], 13, 15, 1);
    a->speed = 3.5f;
    a->play_once = 1;

    player->anim_upper_cut = Animation_Handle::sprite_add_animation(player->base.sprite, v1[3], 18, 21, 1);
    player->anim_upper_cut->speed = 1.5f;
    player->anim_upper_cut->play_once = 1;

    Animation_Handle::sprite_add_animation(player->base.sprite, v1[4], 16, 16, 0);

    a = Animation_Handle::sprite_add_animation(player->base.sprite, v1[5], 30, 35, 0);
    a->speed = 0.5f;
    a->play_once = 1;

    a = Animation_Handle::sprite_add_animation(player->base.sprite, v1[7], 24, 26, 1);
    a->speed = 1.3f;
    a->play_once = 1;

    a = Animation_Handle::sprite_add_animation(player->base.sprite, v1[8], 27, 29, 0);
    a->speed = 0.5f;
    a->play_once = 1;

    player_set_state(player, CHARACTER_STATE_IDLE);

    return player;
};


void Player:: player_delete(player_t* player) {
    free(player->base.hit_boxes);
    Sprite_Handle::sprite_delete(player->base.sprite);
    free(player);
}


void Player::player_draw(player_t* player, HDC hdc) {
    if (player->base.visible == 1) {
        Sprite_Handle::sprite_draw(player->base.sprite, &player->base.position, player->base.direction, hdc);
    }
}


void  Player :: player_set_state(player_t* player, int state) {
    character_t* ch = &player->base;
    sprite_t* sprite = ch->sprite;
    if (ch->state == CHARACTER_STATE_FIGHT) {
        animation_t* anim = Animation_Handle:: sprite_current_animation(sprite);
        if (!anim->animation_ended) {
            return;
        }
    }

    if (player->base.prev_state == CHARACTER_STATE_HIT_FALL) {
        return;
    }
    p.enqueue(L"run");
    p.enqueue(L"walk");
    p.enqueue(L"punch");
    p.enqueue(L"punch2");
    p.enqueue(L"hit");
    p.enqueue(L"fall");
    p.enqueue(L"idle");
    p.enqueue(L"jump");
    p.enqueue(L"win");


    while (!p.isEmpty()) {
        try {
            v1.push_back(p.getFront());
            p.dequeue();
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;

        }
    }
    switch(state) {
        case CHARACTER_STATE_IDLE:
            Animation_Handle:: sprite_set_animation(player->base.sprite, v1[6]);
            break;
        case CHARACTER_STATE_WALKING:
            Animation_Handle::sprite_set_animation(player->base.sprite, v1[1]);
            break;
        case CHARACTER_STATE_JUMP:
            Animation_Handle::sprite_set_animation(player->base.sprite, v1[7]);
            if (ch->on_ground) {
                ch->velocity.y = -18.0f;
            }
            break;
        case CHARACTER_STATE_FIGHT:
            // if we are about to punch 3rd time in a row
            if (player->hit_streak >= 2) {
                Animation_Handle::sprite_set_animation(player->base.sprite, v1[3]);
            } else {
                Animation_Handle::sprite_set_animation(player->base.sprite, v1[2]);
            }
            break;
        case CHARACTER_STATE_HIT:
            Animation_Handle::sprite_set_animation(player->base.sprite, v1[4]);
            if (player->base.state != state) {
                player->hit_recover_time = 10;
            }
            break;
        case CHARACTER_STATE_HIT_FALL:
            Animation_Handle::sprite_set_animation(player->base.sprite, v1[5]);
            if (player->base.state != state) {
                player->hit_recover_time = 300;
            }
            break;
        case CHARACTER_STATE_WIN:
            Animation_Handle::sprite_set_animation(player->base.sprite, v1[8]);
            break;
    }
    if (player->base.state != state) {
        player->base.prev_state = player->base.state;
        player->base.state = state;
    }
}


void Player :: player_update_state(player_t* player, float dt) {
    character_t* ch = &player->base;
    sprite_t* sprite = ch->sprite;

    if (ch->state != CHARACTER_STATE_HIT_FALL) {
        if ( ch->health < 1) {
            player_set_state(player, CHARACTER_STATE_HIT_FALL);
        }
    }

    if (ch->state == CHARACTER_STATE_FIGHT) {
        animation_t* anim = Animation_Handle::sprite_current_animation(sprite);
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
    } else if (ch->state == CHARACTER_STATE_JUMP) {
        animation_t* anim = Animation_Handle::sprite_current_animation(sprite);
        if (anim->animation_ended) {
            player_set_state(player, CHARACTER_STATE_IDLE);
            ch->velocity.y = 0;
        }
    }
}

void Player :: player_update(player_t* player, float dt) {
    RECT rcClient;
    GetClientRect(game->hwnd, &rcClient);
    character_t* ch = &player->base;

    if (ch->velocity.x > 0)
        ch->direction = CHARACTER_DIRECTION_RIGHT;
    else if (ch->velocity.x < 0)
        ch->direction = CHARACTER_DIRECTION_LEFT;

    Sprite_Handle::sprite_update(ch->sprite, dt);

    player_update_state(player, dt);

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


        Character :: player_calculate_hit_boxes(player);

        if (ch->position.x >= 400) {
            game->view_x = ch->position.x - 400;
        }

        if (ch->position.x <= 150) {
            game->view_x = ch->position.x - 150;
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


