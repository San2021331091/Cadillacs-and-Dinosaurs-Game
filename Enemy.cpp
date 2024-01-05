#include "Enemy.h"
#include "Utils.h"
#include "Sprite.h"
#include "Character.h"
#include "Player.h"

void Enemy::enemy_cleanup(enemy_t* enemy) {
    free(enemy->hit_boxes);
    enemy->sprite.clear();
}

enemy_t * Enemy ::enemy_spawn(int x, int z, int type) {


    if (game->enemy_count >= game->enemy_max) {
        return nullptr;
    }
    enemy_t* enemy = &game->enemies[game->enemy_count];
    game->enemy_count++;

    enemy->type = type;
    enemy->health = 100.0f;

    enemy->animation_time = 0.0f;
    enemy->movement_speed = 2.5;
    enemy->direction = CHARACTER_DIRECTION_LEFT;
    enemy->prev_state = 0;
    enemy->visible = 1;
    enemy->hit_reset = 0.0;
    enemy->being_hit = 0;
    enemy->hit_recover_time = 0.0f;
    enemy->time_to_attack = 0;
    enemy->engaging = 0;
    enemy->marked_for_delete = 0;
    enemy->walk_time = 0;
    enemy->death_blink_time = 0;

    Utils :: vector3d_zero(&enemy->position);
    Utils :: vector3d_zero(&enemy->velocity);

    enemy->on_ground = 1;

    enemy->position.x = (float) x;
    enemy->position.y = game->ground_y;
    enemy->position.z = (float)z;

    enemy->hit_boxes = new FloatRect [10];
    enemy->hit_box_count = 0;

    enemy->damage_takes = 20;
    enemy->hit_streak = 0;

    animation_t* a;

    enemy->state_on_enter = 1;
    enemy->state = CHARACTER_STATE_IDLE;

    vector<animation_t> * walks = &(enemy->walk);

    for(auto &walk : *walks){



        switch (type) {

            case ENEMY_TYPE_FERRIS:
                enemy->sprite.push_back(*Sprite_Handle :: sprite_new("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/enemy-1.bmp", 120, 90, 2.5));
                enemy->name="Ferris & Driver";
                a = Animation_Handle::sprite_add_animation(enemy->sprite, "run", 6, 11, 0);
                a->speed = 0.9f;
                walk = *Animation_Handle :: sprite_add_animation(enemy->sprite, "walk", 23, 29, 0);
                a = Animation_Handle :: sprite_add_animation(enemy->sprite, "punch", 12, 14, 1);
                a->speed = 1.3f;
                Animation_Handle::sprite_add_animation(enemy->sprite, "punch_pre", 23, 23, 1);
                Animation_Handle :: sprite_add_animation(enemy->sprite,"hit", 21, 21, 1);
                a = Animation_Handle::sprite_add_animation(enemy->sprite, "fall", 15, 19, 0);
                a->speed = 0.7f;
                a->play_once = 1;


                break;

            case ENEMY_TYPE_GNEISS:
                enemy->sprite.push_back(*Sprite_Handle :: sprite_new("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/enemy-2.bmp", 120, 90, 2.5));
                enemy->name="Gneiss";
                a = Animation_Handle::sprite_add_animation(enemy->sprite, "run", 6, 11, 0);
                a->speed = 0.9f;
                walk = *Animation_Handle::sprite_add_animation(enemy->sprite, "walk", 23, 29, 0);
                a = Animation_Handle::sprite_add_animation(enemy->sprite, "punch", 12, 14, 1);
                a->speed = 1.3f;
               Animation_Handle:: sprite_add_animation(enemy->sprite, "punch_pre", 23, 23, 1);
               Animation_Handle::sprite_add_animation(enemy->sprite, "hit", 21, 21, 1);
                a = Animation_Handle::sprite_add_animation(enemy->sprite, "fall", 15, 19, 0);
                a->speed = 0.7f;
                a->play_once = 1;
                break;



            case ENEMY_TYPE_BUTCHER:
                enemy->sprite.push_back(*Sprite_Handle::sprite_new("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/enemy-butcher.bmp", 200, 130, 2.3));
                enemy->frames_per_row = 3;
                enemy->name = "Butcher";
                enemy->damage_takes = 2.0f;
                enemy->movement_speed = 3.2;

                a = Animation_Handle::sprite_add_animation(enemy->sprite, "run", 3, 8, 0);
                a->speed = 0.9f;
                walk = *Animation_Handle :: sprite_add_animation(enemy->sprite, "walk", 3, 8, 0);
                a = Animation_Handle::sprite_add_animation(enemy->sprite, "punch", 9, 11, 1);
                a->speed = 0.9f;
                Animation_Handle:: sprite_add_animation(enemy->sprite, "punch_pre", 0, 0, 1);
                Animation_Handle::sprite_add_animation(enemy->sprite, "hit", 12, 12, 1);
                a = Animation_Handle::sprite_add_animation(enemy->sprite, "fall", 12, 14, 0);
                a->speed = 0.3f;
                a->play_once = 1;
                break;
        }

        }

    a = Animation_Handle::sprite_add_animation(enemy->sprite, "idle", 0, 2, 1);
    a->speed = 0.8f;

   Animation_Handle:: sprite_add_animation(enemy->sprite, "hit2", 20, 20, 1);
    Animation_Handle::sprite_add_animation(enemy->sprite, "hit3", 22, 22, 1);


    enemy->walk_target_idx = -1;
    enemy->target_x = 0;
    enemy->target_z = 0;

    return enemy;

    }

void Enemy :: enemy_draw(enemy_t* enemy, RenderWindow &window) {
    if (enemy->visible == 1) {
        Sprite_Handle :: sprite_draw(enemy->sprite, &enemy->position, enemy->direction);

    }
}

int game_random(int lower, int upper) {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(lower, upper);

    return dist(mt); // Generate a random number
}

void Enemy ::enemy_do_ai(enemy_t *enemy, float dt) {

    vector<player_t> player = game->player;
    character_t *ch = enemy;
    int type = enemy->type;
    vector3d_t player_center{}, enemy_center{};
    float x, z;

    for (auto &pl: player) {
        Character::character_get_center(&pl, &player_center);
        Character::character_get_center(enemy, &enemy_center);
        x = fabs(enemy_center.x - player_center.x);
        z = fabs(enemy_center.z - player_center.z);
        float player_distance_x = sqrtf(x * x);
        float player_distance_z = sqrtf(z * z);

        const float idle_range_x = 500;
        const float idle_range_z = 100;

        float fight_range_x = 150;
        const float fight_range_z = 10;

        if (type == ENEMY_TYPE_BUTCHER) {
            fight_range_x = 220;
        }

        if (game->state == GAME_STATE_INTRO) {
            ch->state = CHARACTER_STATE_IDLE;
        }


        if (enemy->being_hit) {
            enemy->being_hit = 0;

            vector<sprite_t> sprites = pl.sprite;
            for (auto &sprite: sprites) {

                auto anim = static_cast<vector<animation_t> *>(Animation_Handle::sprite_current_animation(&sprite));


                if (anim == &pl.anim_upper_cut) {
                    // double damage
                    ch->health -= enemy->damage_takes * 2;
                    ch->state = CHARACTER_STATE_HIT_FALL;
                    enemy->engaging = 0;
                    enemy->hit_recover_time = 50;
                    pl.hit_streak = 0;
                    ch->velocity.y = -10.0;
                } else {
                    ch->health -= enemy->damage_takes + (float) Utils::get_rand(0, 5);
                    if (ch->health < 1) {
                        ch->health = 0;
                        ch->state = CHARACTER_STATE_HIT_FALL;
                        enemy->engaging = 0;
                        enemy->hit_recover_time = 70;
                        pl.hit_streak = 0;
                        ch->velocity.y = -5.0;
                    } else {
                        ch->state = CHARACTER_STATE_HIT;
                        if (enemy->hit_recover_time <= 0) {
                            enemy->hit_recover_time = 30;
                        }
                    }
                }


            }


        }
        const float walk_time_wait = 100;
        vector<sprite_t> sp = ch->sprite;

        for (auto &sprite1: sp) {


            if (ch->state == CHARACTER_STATE_IDLE) {
                auto *prev = reinterpret_cast<animation_t *>(Animation_Handle::sprite_current_animation(
                        &sprite1));
                animation_t *idle = Animation_Handle::sprite_set_animation(ch->sprite, "idle");

                if (prev != idle) {
                    sprite1.frame = game_random(idle->frame_start, idle->frame_end);
                }
                // while in idle state, enemy looks for player
                if (pl.health > 0 && player_distance_x < idle_range_x && player_distance_z < idle_range_z) {
                    float dir_x = player_center.x - enemy_center.x;

                    if (dir_x > 0)
                        ch->direction = CHARACTER_DIRECTION_RIGHT;
                    else if (dir_x < 0)
                        ch->direction = CHARACTER_DIRECTION_LEFT;
                    {
                        // run for player to attack
                        ch->state = CHARACTER_STATE_RUNNING;
                    }
                }
                ch->velocity.x = 0.0;
                ch->velocity.z = 0.0;
            } else if (ch->state == CHARACTER_STATE_WALKING) {
                // walk around the player randomly (waiting to fight)

                float dir_x = player_center.x - enemy_center.x;

                if (dir_x > 0)
                    ch->direction = CHARACTER_DIRECTION_RIGHT;
                else if (dir_x < 0)
                    ch->direction = CHARACTER_DIRECTION_LEFT;


                Animation_Handle::sprite_set_animation(ch->sprite, "walk");

                float target_distance_x = 0;
                float target_distance_z = 0;

                if (enemy->walk_target_idx >= 0) {


                    if (enemy->target_z >= game->max_z) {
                        enemy->target_z = game->max_z;
                    }

                    if (enemy->target_z <= game->min_z) {
                        enemy->target_z = game->min_z;
                    }

                    float dx = fabs(enemy->target_x - enemy_center.x);
                    float dz = fabs(enemy->target_z - enemy_center.z);
                    target_distance_x = sqrtf(dx * dx);
                    target_distance_z = sqrtf(dz * dz);
                }
                if (target_distance_x > 10 || target_distance_z > 10) {
                    animation_t *anim;
                    anim->play_once = 0;

                    dir_x = enemy->target_x - enemy_center.x;
                    float dir_z = enemy->target_z - enemy_center.z;

                    ch->velocity.x = 0.0;
                    ch->velocity.z = 0.0;

                    if (dir_x != 0) {
                        float dist = sqrtf(dir_x * dir_x);
                        dir_x /= dist;
                        ch->velocity.x = (float) (dir_x * enemy->movement_speed * 1.5);
                    }
                    if (dir_z != 0) {
                        float dist = sqrtf(dir_z * dir_z);
                        dir_z /= dist;
                        ch->velocity.z = (float) (dir_z * enemy->movement_speed * 1.5);
                    }
                } else {
                    animation_t *anim;
                    anim->play_once = 1;
                    ch->velocity.x = 0.0;
                    ch->velocity.z = 0.0;

                    if (enemy->walk_time < walk_time_wait) {
                        enemy->walk_time += dt;
                    } else {
                        enemy->walk_time = 0;

                        float x_list[4];
                        float z_list[4];

                        x_list[0] = player_center.x - 300;
                        z_list[0] = player_center.z - 50;

                        x_list[1] = player_center.x + 300;
                        z_list[1] = player_center.z + 50;

                        x_list[2] = player_center.x + 100;
                        z_list[2] = player_center.z + 150;

                        x_list[3] = player_center.x + 100;
                        z_list[3] = player_center.z - 150;

                        enemy->walk_target_idx = game_random(0, 3);

                        enemy->target_x = x_list[enemy->walk_target_idx];
                        enemy->target_z = z_list[enemy->walk_target_idx];

                        if (enemy->target_z >= game->max_z) {
                            enemy->target_z = game->max_z;
                        }

                        if (enemy->target_z <= game->min_z) {
                            enemy->target_z = game->min_z;
                        }
                    }

                    // this enemy won't engage while any other enemy is engaging
                    bool can_fight = true;
                    for (int i = 0; i < game->enemy_count; i++) {
                        enemy_t *another_enemy = &game->enemies[i];
                        if (another_enemy != enemy) {

                            if (another_enemy->engaging) {
                                can_fight = false;
                            }
                        }
                    }
                    if (can_fight) {
                        ch->state = CHARACTER_STATE_PUNCH_PRE;
                        enemy->engaging = 1;
                        enemy->time_to_attack = 8;
                    }


                }


            } else if (ch->state == CHARACTER_STATE_MOVE) {
                Animation_Handle::sprite_set_animation(ch->sprite, "walk");

                float dir_x = enemy->target_x - enemy_center.x;
                float dir_z = enemy->target_z - enemy_center.z;
                float distance_x = sqrtf(dir_x * dir_x);
                float distance_z = sqrtf(dir_z * dir_z);

                if (distance_x > 2.0) {
                    dir_x /= distance_x;
                    ch->velocity.x = (float) (dir_x * enemy->movement_speed * 1.2);
                } else {
                    ch->velocity.x = 0;
                }

                if (distance_z > 2.0) {
                    dir_z /= distance_z;
                    ch->velocity.z = (float) (dir_z * enemy->movement_speed * 1.2);
                } else {
                    ch->velocity.z = 0;
                }

                if (fabs(distance_x) < 2.0 && fabs(distance_z) < 2.0) {
                    ch->state = ch->prev_state;
                    ch->prev_state = CHARACTER_STATE_MOVE;


                }
                // face towards the player? maybe
                float face_dir = player_center.x - enemy_center.x;

                if (face_dir > 0)
                    ch->direction = CHARACTER_DIRECTION_RIGHT;
                else if (face_dir < 0)
                    ch->direction = CHARACTER_DIRECTION_LEFT;


            } else if (ch->state == CHARACTER_STATE_RUNNING) {

                float dir_x = player_center.x - enemy_center.x;
                float dir_z = player_center.z - enemy_center.z;

                if (player_distance_x < fight_range_x && player_distance_z < fight_range_z) {
                    // this enemy won't engage while any other enemy is engaging
                    bool can_fight = true;
                    for (int i = 0; i < game->enemy_count; i++) {
                        enemy_t *another_enemy = &game->enemies[i];
                        if (another_enemy != enemy) {
                            if (another_enemy->engaging) {
                                can_fight = false;
                                break;
                            }
                        }
                    }
                    if (can_fight) {
                        ch->state = CHARACTER_STATE_PUNCH_PRE;
                        enemy->engaging = 1;
                        enemy->time_to_attack = 8;
                    } else {
                        ch->state = CHARACTER_STATE_WALKING;
                        enemy->walk_time = walk_time_wait;
                    }
                }


                else {
                    Animation_Handle :: sprite_set_animation(ch->sprite, "run");
                    float dist_x = sqrtf(dir_x * dir_x);
                    if (dist_x > 2.0) {
                        dir_x /= dist_x;
                        ch->velocity.x = dir_x * enemy->movement_speed;
                    } else {
                        ch->velocity.x = 0;
                    }
                    float dist_z = sqrtf(dir_z * dir_z);
                    if (dist_z > 2.0) {
                        dir_z /= dist_z;
                        ch->velocity.z = dir_z * enemy->movement_speed;
                    } else {
                        ch->velocity.z = 0;
                    }
                }

                if (ch->velocity.x > 0)
                    ch->direction = CHARACTER_DIRECTION_RIGHT;
                else if (ch->velocity.x < 0)
                    ch->direction = CHARACTER_DIRECTION_LEFT;

            }

            else if (ch->state == CHARACTER_STATE_PUNCH_PRE) {
               Animation_Handle:: sprite_set_animation(ch->sprite, "punch_pre");
                if (enemy->time_to_attack <= 0) {
                    ch->state = CHARACTER_STATE_FIGHT;
                } else {
                    enemy->time_to_attack -= dt * 0.5f;
                }
                ch->velocity.x = 0.0;
                ch->velocity.z = 0.0;
            }

            else if (ch->state == CHARACTER_STATE_RANGE_ATTACK) {
                Animation_Handle::sprite_set_animation(ch->sprite, "idle");

                float dir_x = player_center.x - enemy_center.x;

                if (dir_x > 0)
                    ch->direction = CHARACTER_DIRECTION_RIGHT;
                else if (dir_x < 0)
                    ch->direction = CHARACTER_DIRECTION_LEFT;

                if (player_distance_x < 300) {
                    // step back if too close
                    enemy->target_x = enemy_center.x + (float)(-ch->direction * 220);
                    enemy->target_z = enemy_center.z;

                    ch->prev_state = ch->state;
                    ch->state = CHARACTER_STATE_MOVE;
                    ch->state_on_enter = 1;
                }

                if (ch->state_on_enter == 1) {
                    ch->state_on_enter = 0;
                } else {
                    if (game->enemy_count == 1) {
                        ch->prev_state = ch->state;
                        ch->state = CHARACTER_STATE_RUNNING;
                        ch->state_on_enter = 1;
                    }
                }

            }

            else if (ch->state == CHARACTER_STATE_FIGHT) {
                Animation_Handle::sprite_set_animation(ch->sprite, "punch");

                float dir_x = player_center.x - enemy_center.x;

                if (dir_x > 0)
                    ch->direction = CHARACTER_DIRECTION_RIGHT;
                else if (dir_x < 0)
                    ch->direction = CHARACTER_DIRECTION_LEFT;

                // what if player gets too close for the enemy to punch?
                if (player_distance_x < (fight_range_x / 1.2)) {
                    // step back a little bit
                    enemy->target_x = enemy_center.x + (float) (-ch->direction * 50);
                    enemy->target_z = enemy_center.z;

                    ch->prev_state = ch->state;
                    ch->state = CHARACTER_STATE_MOVE;
                }
                else if (player_distance_x > fight_range_x || player_distance_z > fight_range_z) {
                    // run for player to attack
                    ch->state = CHARACTER_STATE_RUNNING;
                } else {
                    ch->velocity.x = 0.0;
                    ch->velocity.z = 0.0;
                }

                if (pl.health < 1) {
                    ch->prev_state = ch->state;
                    ch->state = CHARACTER_STATE_IDLE;
                }

            }

            else if (ch->state == CHARACTER_STATE_HIT) {
                Animation_Handle::sprite_set_animation(ch->sprite, "hit");
                if (enemy->hit_recover_time <= 0) {
                    // run for player to attack
                    ch->state = CHARACTER_STATE_RUNNING;
                }
                enemy->hit_recover_time -= dt * 0.5f;

                ch->velocity.x = 0.0;
                ch->velocity.z = 0.0;

            }
            else if (ch->state == CHARACTER_STATE_RANGE_ATTACK) {
                Animation_Handle::sprite_set_animation(ch->sprite, "idle");

                float dir_x = player_center.x - enemy_center.x;

                if (dir_x > 0)
                    ch->direction = CHARACTER_DIRECTION_RIGHT;
                else if (dir_x < 0)
                    ch->direction = CHARACTER_DIRECTION_LEFT;


                if (player_distance_x < 300) {
                    // step back if too close
                    enemy->target_x = enemy_center.x + (float)(-ch->direction * 220);
                    enemy->target_z = enemy_center.z;

                    ch->prev_state = ch->state;
                    ch->state = CHARACTER_STATE_MOVE;
                    ch->state_on_enter = 1;
                }

                if (ch->state_on_enter == 1) {
                    ch->state_on_enter = 0;
                } else {
                    if (game->enemy_count == 1) {
                        ch->prev_state = ch->state;
                        ch->state = CHARACTER_STATE_RUNNING;
                        ch->state_on_enter = 1;
                    }
                }

            } else if (ch->state == CHARACTER_STATE_FIGHT) {
                Animation_Handle::sprite_set_animation(ch->sprite, "punch");

                float dir_x = player_center.x - enemy_center.x;

                if (dir_x > 0)
                    ch->direction = CHARACTER_DIRECTION_RIGHT;
                else if (dir_x < 0)
                    ch->direction = CHARACTER_DIRECTION_LEFT;

                // what if player gets too close for the enemy to punch?
                if (player_distance_x < (fight_range_x / 1.2)) {
                    // step back a little bit
                    enemy->target_x = enemy_center.x + (float)(-ch->direction * 50);
                    enemy->target_z = enemy_center.z;

                    ch->prev_state = ch->state;
                    ch->state = CHARACTER_STATE_MOVE;
                }
                else if (player_distance_x > fight_range_x || player_distance_z > fight_range_z) {
                    // run for player to attack
                    ch->state = CHARACTER_STATE_RUNNING;
                } else {
                    ch->velocity.x = 0.0;
                    ch->velocity.z = 0.0;
                }

                if (pl.health < 1) {
                    ch->prev_state = ch->state;
                    ch->state = CHARACTER_STATE_IDLE;
                }

            } else if (ch->state == CHARACTER_STATE_HIT) {
                Animation_Handle::sprite_set_animation(ch->sprite, "hit");
                if (enemy->hit_recover_time <= 0) {
                    // run for player to attack
                    ch->state = CHARACTER_STATE_RUNNING;
                }
                enemy->hit_recover_time -= dt * 0.5f;

                ch->velocity.x = 0.0;
                ch->velocity.z = 0.0;

            } else if (ch->state == CHARACTER_STATE_HIT_FALL) {
               Animation_Handle:: sprite_set_animation(ch->sprite, "fall");

                int last_frame = 19;

                if (type == ENEMY_TYPE_BUTCHER) {
                    last_frame = 14;
                }
               sprite_t *s1;
                if (s1->frame >= last_frame) {
                    if (ch->health > 0) {
                        if (enemy->hit_recover_time <= 0) {
                            ch->prev_state = ch->state;
                            if (type == ENEMY_TYPE_BUTCHER) {
                                ch->state = CHARACTER_STATE_RANGE_ATTACK;
                                ch->state_on_enter = 1;

                                enemy_t* e;

                                if (ch->health <= 30) {
                                    e = enemy_spawn((int)(game->view_x + game->rect_width + 200), 0, ENEMY_TYPE_GNEISS);
                                    e->state = CHARACTER_STATE_RUNNING;

                                    e = enemy_spawn((int)(game->view_x + game->rect_width + 100), 25, ENEMY_TYPE_GNEISS);
                                    e->state = CHARACTER_STATE_RUNNING;

                                    e = enemy_spawn((int)(game->view_x + game->rect_width), 50, ENEMY_TYPE_FERRIS);
                                    e->state = CHARACTER_STATE_RUNNING;

                                    e = enemy_spawn((int)(game->view_x - 200), 0, ENEMY_TYPE_GNEISS);
                                    e->state = CHARACTER_STATE_RUNNING;

                                    e = enemy_spawn((int)(game->view_x - 100), 25, ENEMY_TYPE_GNEISS);
                                    e->state = CHARACTER_STATE_RUNNING;

                                    e = enemy_spawn((int)(game->view_x) , 50, ENEMY_TYPE_FERRIS);
                                    e->state = CHARACTER_STATE_RUNNING;
                                } else if (ch->health <= 50) {
                                    e = enemy_spawn((int)(game->view_x + game->rect_width + 100), 0, ENEMY_TYPE_GNEISS);
                                    e->state = CHARACTER_STATE_RUNNING;

                                    e = enemy_spawn((int)(game->view_x + game->rect_width), 50, ENEMY_TYPE_FERRIS);
                                    e->state = CHARACTER_STATE_RUNNING;

                                    e = enemy_spawn((int)(game->view_x - 100), 00, ENEMY_TYPE_GNEISS);
                                    e->state = CHARACTER_STATE_RUNNING;

                                    e = enemy_spawn((int)(game->view_x), 50, ENEMY_TYPE_FERRIS);
                                    e->state = CHARACTER_STATE_RUNNING;
                                } else if (ch->health <= 80) {
                                    e = enemy_spawn((int)(game->view_x + game->rect_width), 0, ENEMY_TYPE_GNEISS);
                                    e->state = CHARACTER_STATE_RUNNING;

                                    e = enemy_spawn((int)(game->view_x - 100), 50, ENEMY_TYPE_FERRIS);
                                    e->state = CHARACTER_STATE_RUNNING;
                                }
                            } else {
                                ch->state = CHARACTER_STATE_RUNNING;
                            }
                        } else {
                            enemy->hit_recover_time -= dt * 0.5f;
                        }
                    } else {
                        if (enemy->hit_recover_time <= 0) {
                            enemy->marked_for_delete = 1;
                            if (enemy->type == ENEMY_TYPE_BUTCHER) {
                                game->state = GAME_STATE_WIN;

                                auto *player1 = reinterpret_cast<player_t *>(&game->player);

                                    Player::player_set_state(player1, CHARACTER_STATE_WIN);

                            }
                        } else {
                            enemy->hit_recover_time -= dt * 0.5f;

                            if (enemy->death_blink_time >= 8) {
                                enemy->death_blink_time = 0;
                                ch->visible = !ch->visible;
                            }
                            enemy->death_blink_time += dt;
                        }
                    }
                }

                if (type == ENEMY_TYPE_BUTCHER) {
                    sprite_t *sp1;
                    switch(sp1->frame) {
                        case 12:
                            ch->velocity.x = -(float)(ch->direction * (enemy->movement_speed * 5.0));
                            break;
                        case 13:

                            ch->velocity.x = - (float)(ch->direction * (enemy->movement_speed * 2.0));
                            break;
                        case 14:

                            ch->velocity.x = 0.0;
                            break;
                    }
                } else {
                    sprite_t *sp1;
                    switch(sp1->frame) {
                        case 15:
                            ch->velocity.x = -(float)(ch->direction * (enemy->movement_speed * 8.0));
                            break;
                        case 16:
                            ch->velocity.x = -(float)(ch->direction * (enemy->movement_speed * 4.0));
                            break;
                        case 17:
                            ch->velocity.x = -(float)(ch->direction * (enemy->movement_speed * 3.0));
                            break;
                        case 18:
                            ch->velocity.x = -(float)(ch->direction * (enemy->movement_speed * 2.0));
                            break;
                        case 19:
                            ch->velocity.x = 0.0;
                            break;
                    }
                }
                ch->velocity.z = 0.0;
            }

            else {
                ch->velocity.x = 0.0;
                ch->velocity.z = 0.0;
            }
        }
    }
}


void Enemy :: enemy_update(enemy_t* enemy,int index, float dt) {

    Vector2u windowSize = game->window.getSize();
    FloatRect floatRect;
    floatRect.left = 0.0f;
    floatRect.top = 0.0f;
    floatRect.width = static_cast<float>(windowSize.x);
    floatRect.height = static_cast<float>(windowSize.y);
    character_t* ch = enemy;
    vector<sprite_t> sprite = ch->sprite;

   Sprite_Handle:: sprite_update(sprite, dt);

    // this will only change the states, won't modify position or velocity
    Enemy:: enemy_do_ai(enemy, dt);

    ch->velocity.y += (float)(game->gravity * 0.3 * dt);

    ch->position.x += ch->velocity.x * dt;
    ch->position.y += ch->velocity.y * dt;
    ch->position.z += ch->velocity.z * dt;

    if (ch->position.y > game->ground_y) {

        if (ch->velocity.y > 2) {
            ch->velocity.y = -(float)(ch->velocity.y * 0.6);
        } else {
            ch->position.y = game->ground_y;
            ch->velocity.y = 0;
            ch->on_ground = 1;
        }
    } else {
        ch->on_ground = 0;
    }

   Character::enemy_calculate_hit_boxes(enemy);

}
