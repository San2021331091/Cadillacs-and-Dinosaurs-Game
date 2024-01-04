#include "Character.h"
#include "Player.h"
#include "Sprite.h"
#include "Utils.h"

void Character ::character_get_center(character_t *character, vector3d_t *center) {

    vector<sprite_t> *sprites = &(character->sprite);

    for(auto & sprite : *sprites){
        float scaled_width = (float)sprite.width * sprite.scale;
        float scaled_height = (float)sprite.height * sprite.scale;
        vector3d_t* pos = &character->position;

        center->x = pos->x + (scaled_width/2);
        center->y = pos->y + (scaled_height/2);
        center->z = pos->z;
    }

}


bool Character ::rect_collision(FloatRect &rect1, FloatRect &rect2) {


    if (rect1.left < rect2.left + rect2.width &&
        rect1.left + rect1.width > rect2.left &&
        rect1.top < rect2.top + rect2.height &&
        rect1.top + rect1.height > rect2.top) {
        return true;
    }
    return false;


}


void Character ::enemy_calculate_hit_boxes(enemy_t *enemy) {


   character_t *character = enemy;
   FloatRect *box;
    int type = enemy->type;
    vector3d_t* pos = &character->position;
    vector<sprite_t> *sprites = &(character->sprite);
    int state = character->state;
    int direction = character->direction;

    for(auto &sprite : *sprites){

        float scaled_width = (float)sprite.width * sprite.scale;
        float scaled_height = (float)sprite.height * sprite.scale;
        character->hit_box_count = 0;
        float center_x = pos->x + (scaled_width/2);
        box = &character->hit_boxes[BBOX_MAIN];
        character->hit_box_count++;

         float currentBoxRight,currentBoxBottom;
         box->width = currentBoxRight-box->left;
         box->height = currentBoxBottom - box->top;

        if (type == ENEMY_TYPE_BUTCHER) {
            box->left = center_x - (50 * sprite.scale);
            box->top = (pos->y + pos->z) - (120 * sprite.scale);

            currentBoxRight = center_x + (50 * sprite.scale);
            currentBoxBottom = box->top + scaled_height;
        } else {
            box->left = pos->x + (scaled_width/4);
            box->top = pos->y + pos->z - scaled_height;

            currentBoxRight = box->left + (scaled_width/2);
            currentBoxBottom = box->top + scaled_height;
        }

        float offset_head = 0,offset_chest = 0;

        if (type == ENEMY_TYPE_BUTCHER) {
            offset_head = 90;
            offset_chest = 70;
        } else {
            offset_head = 70;
            offset_chest = 60;
        }

        if (state == CHARACTER_STATE_IDLE ||
            state == CHARACTER_STATE_WALKING ||
            state == CHARACTER_STATE_RUNNING ||
            state == CHARACTER_STATE_RANGE_ATTACK ||
            state == CHARACTER_STATE_HIT ||
            state == CHARACTER_STATE_MOVE ||
            state == CHARACTER_STATE_HIT_FALL) {

            box = &character->hit_boxes[BBOX_HEAD];
            character->hit_box_count++;

            box->left = center_x - (16 * sprite.scale);
            box->top = (pos->y + pos->z) - (offset_head * sprite.scale);

            currentBoxRight = box->left + 15 * sprite.scale;
            currentBoxBottom = box->top + (15 * sprite.scale);


            box = &character->hit_boxes[BBOX_CHEST];
            character->hit_box_count++;

            box->left = pos->x + (scaled_width / 2) - (12 * sprite.scale);
            box->top = (pos->y + pos->z) - (offset_chest * sprite.scale);

            currentBoxRight = pos->x + (scaled_width / 2) + (12 * sprite.scale);
            currentBoxBottom = box->top + (20 * sprite.scale);


        }
        else if (state == CHARACTER_STATE_FIGHT) {


            box = &character->hit_boxes[BBOX_HEAD];
            character->hit_box_count++;

            box->left = center_x - (10 * sprite.scale);
            box->top = (pos->y + pos->z) - (offset_head * sprite.scale);

            currentBoxRight = box->left + 15 * sprite.scale;
           currentBoxBottom = box->top + (15 * sprite.scale);

            box = &character->hit_boxes[BBOX_CHEST];
            character->hit_box_count++;

            box->left = center_x - (20 * sprite.scale);
            box->top = (pos->y + pos->z) - (offset_chest * sprite.scale);

          currentBoxRight = box->left + (60 * sprite.scale);
          currentBoxBottom = box->top + (40 * sprite.scale);

            box = &character->hit_boxes[BBOX_FIST];
            character->hit_box_count++;

            float fistOffsetX = 0.0f;
            float fistOffsetY = 0.0f;

            int last_frame = 14;

            if (type == ENEMY_TYPE_BUTCHER) {
                last_frame = 11;

                if (sprite.frame == 9) {
                    fistOffsetX = -100.0f;
                    fistOffsetY = -50;
                    enemy->hit_reset = 0;
                }
                else if (sprite.frame == 10) {
                    fistOffsetX = -20.0f;
                    fistOffsetY = 80;
                }
                else if (sprite.frame == 11) {
                    fistOffsetX = 180.0f;
                    fistOffsetY = 110;
                }
            } else {
                if (sprite.frame == 12) {
                    fistOffsetX = -50.0f;
                    enemy->hit_reset = 0;
                }
                else if (sprite.frame == 13)
                    fistOffsetX = 40.0f;
                else if (sprite.frame == 14)
                    fistOffsetX = 140.0f;
            }

            fistOffsetX *= -(float)direction;

            box->left = center_x - (fistOffsetX + 10 * sprite.scale);
            box->top = pos->y + pos->z - scaled_height + (fistOffsetY + 21 * sprite.scale);

            currentBoxRight = box->left + (12 * sprite.scale);
           currentBoxBottom = box->top + (12 * sprite.scale);

            effect_t* damageFx = &game->fx_punch;

            if (type == ENEMY_TYPE_BUTCHER) {
                damageFx = &game->fx_blood;
            }

            vector<sprite_t>* damage_sprite = &(damageFx->sprite);



            for(auto &sprite1 : *damage_sprite){


                if (sprite1.frame == last_frame) {
                    float box_center_x = box->left + ((currentBoxRight - box->left) / 2);
                    float box_center_y = box->top + ((currentBoxBottom - box->top) / 2);

                    float fx_half_w = ((float)sprite1.width * sprite1.scale) / 2;
                    float fx_half_h = ((float)sprite1.height * sprite1.scale) / 2;
                    damageFx->x = (box_center_x - fx_half_w);
                    damageFx->y = box_center_y + fx_half_h;
                    damageFx->z = 0;

                    if (type == ENEMY_TYPE_BUTCHER) {
                        damageFx->x -= 50;
                        damageFx->y += 10;
                    }
                }


                vector<player_t>* p = &(game->player);


                for(auto &player : *p){

                    FloatRect * player_head = &player.hit_boxes[BBOX_HEAD];
                    FloatRect *player_chest = &player.hit_boxes[BBOX_CHEST];

                    if (enemy->hit_reset == 0) {
                        if (rect_collision(reinterpret_cast<FloatRect &>(box),
                                           reinterpret_cast<FloatRect &>(player_chest)) ||
                            rect_collision(reinterpret_cast<FloatRect &>(box),
                                           reinterpret_cast<FloatRect &>(player_head))) {
                            if (player.health > 1) {
                                if (type == ENEMY_TYPE_BUTCHER) {
                                    player.health -= 30;
                                } else {
                                    player.health -= 15;
                                }

                                if ( player.health < 1) {
                                    Player :: player_set_state(&player, CHARACTER_STATE_HIT_FALL);
                                } else {
                                    Player::player_set_state(&player, CHARACTER_STATE_HIT);
                                }

                                if (player.health < 0) {
                                    player.health = 0;
                                }
                                damageFx->draw_punch = 1;
                                sprite1.frame = damageFx->anim->frame_start;
                                enemy->hit_reset = 1;
                            }
                        }
                    }
                }


            }


        }



    }


}


void Character ::player_calculate_hit_boxes(player_t *player) {

    character_t *character = player;
    FloatRect *box;
    vector3d_t* pos = &character->position;
    vector <sprite_t>* sprites = &character->sprite;
    int state = character->state;
    int direction = character->direction;

    for(auto &sprite : *sprites){

        float scaled_width = (float) sprite.width * sprite.scale;
        float scaled_height = (float) sprite.height * sprite.scale;

        float center_x = pos->x + (scaled_width/2);

        character->hit_box_count = 0;

        float currentBoxRight,currentBoxBottom;
        box->width = currentBoxRight-box->left;
        box->height = currentBoxBottom - box->top;

        if (state == CHARACTER_STATE_IDLE ||
            state == CHARACTER_STATE_WALKING ||
            state == CHARACTER_STATE_RUNNING ||
            state == CHARACTER_STATE_HIT_FALL) {

            box = &character->hit_boxes[BBOX_MAIN];
            character->hit_box_count++;

            box->left = pos->x + (scaled_width/4);
            box->top = pos->y + pos->z - scaled_height;

            currentBoxRight = box->left + (scaled_width/2);
           currentBoxRight = box->top + scaled_height;
            box = &character->hit_boxes[BBOX_HEAD];
            character->hit_box_count++;

            box->left = pos->x + (scaled_width/2) - (8 * sprite.scale);
            box->top = (pos->y + pos->z) - (80 * sprite.scale);

            currentBoxRight = pos->x + (scaled_width/2) + 8 * sprite.scale;
            currentBoxBottom = box->top + 15 * sprite.scale;


            box = &character->hit_boxes[BBOX_CHEST];
            character->hit_box_count++;

            box->left = pos->x + (scaled_width/2) - (12 * sprite.scale);
            box->top = (pos->y + pos->z) - (70 * sprite.scale);

          currentBoxRight = pos->x + (scaled_width/2) + (12 * sprite.scale);
          currentBoxBottom = box->top + (20 * sprite.scale);


        }


        else if (state == CHARACTER_STATE_FIGHT) {

            box = &character->hit_boxes[BBOX_MAIN];
            character->hit_box_count++;

            box->left = pos->x + (scaled_width / 4);
            box->top = pos->y + pos->z - scaled_height;

            currentBoxRight = box->left + (scaled_width / 2);
            currentBoxBottom = box->top + scaled_height;


            box = &character->hit_boxes[BBOX_HEAD];
            character->hit_box_count++;

            box->left = pos->x + (scaled_width / 2) - (8 * sprite.scale);
            box->top = (pos->y + pos->z) - (80 * sprite.scale);
            currentBoxRight = pos->x + (scaled_width / 2) + 8 * sprite.scale;
            currentBoxBottom = box->top + 15 * sprite.scale;


            box = &character->hit_boxes[BBOX_CHEST];
            character->hit_box_count++;

            box->left = pos->x + (scaled_width / 2) - (12 * sprite.scale);
            box->top = (pos->y + pos->z) - (70 * sprite.scale);
            currentBoxRight = pos->x + (scaled_width / 2) + (12 * sprite.scale);
            currentBoxBottom = box->top + (20 * sprite.scale);

            box = &character->hit_boxes[BBOX_FIST];
            character->hit_box_count++;

            auto anim = static_cast<vector<animation_t>*>(Animation_Handle::sprite_current_animation(&sprite));


            float fistOffset = 0.0f;

            int last_frame = 15;


            if (anim == &player->anim_upper_cut) {
                last_frame = 21;
                if (sprite.frame == 18) {
                    fistOffset = 0.0f;
                    player->hit_reset = 0;
                }
                else if (sprite.frame == 19)
                    fistOffset = 50.0f;
                else if (sprite.frame == 20)
                    fistOffset = 110.0f;
                else if (sprite.frame == 21)
                    fistOffset = 150.0f;
            } else {
                if (sprite.frame == 13) {
                    fistOffset = 0.0f;
                    player->hit_reset = 0;
                }
                else if (sprite.frame == 14)
                    fistOffset = 80.0f;
                else if (sprite.frame == 15)
                    fistOffset = 140.0f;
            }


            fistOffset *= - (float)direction;

            box->left = center_x - (fistOffset + 10 * sprite.scale);
            box->top = (pos->y + pos->z) - (70 * sprite.scale);

           currentBoxRight = box->left + (12 * sprite.scale);
           currentBoxBottom = box->top + (12 * sprite.scale);

            effect_t* damageFx = &game->fx_punch;

            vector<sprite_t>* damage_sprite = &damageFx->sprite;

            float box_center_x = box->left + ((currentBoxRight - box->left) / 2);
            float box_center_y = box->top + ((currentBoxBottom - box->top) / 2);

            for(auto &sprite1 : *damage_sprite){


                if (sprite1.frame == last_frame) {

                    float fx_half_w = ((float)sprite1.width * sprite1.scale) / 2;
                    float fx_half_h = ((float)sprite1.height * sprite1.scale) / 2;
                    damageFx->x = box_center_x - fx_half_w;
                    damageFx->y = box_center_y + fx_half_h;
                    damageFx->z = 0;
                }

                if (player->hit_reset == 0 && sprite1.frame == last_frame) {
                    for (int i = 0;i < game->enemy_count;i++) {
                        enemy_t* enemy = &game->enemies[i];
                        if (enemy->health <= 0 || enemy->state == CHARACTER_STATE_HIT_FALL) {
                            continue;
                        }




                        FloatRect * enemy_head = &enemy->hit_boxes[BBOX_HEAD];
                        FloatRect * enemy_chest = &enemy->hit_boxes[BBOX_CHEST];

                        if (rect_collision(reinterpret_cast<FloatRect &>(box),
                                           reinterpret_cast<FloatRect &>(enemy_chest))  ||
                            rect_collision(reinterpret_cast<FloatRect &>(box),
                                           reinterpret_cast<FloatRect &>(enemy_head))) {
                            enemy->being_hit = 1;
                            player->score += Utils :: get_rand(50, 100);
                            player->hit_reset = 1;

                            player->hit_streak += 1;

                            damageFx->draw_punch = 1;
                            sprite1.frame = damageFx->anim->frame_start;

                            if (player->hit_streak > 2) {
                                game->fx_smack_time = 30;
                                game->fx_smack_x = box_center_x;
                                game->fx_smack_y = box_center_y;
                            }
                        }
                    }
                }





            }






        }



    }






}