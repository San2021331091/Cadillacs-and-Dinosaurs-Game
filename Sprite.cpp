#include "Sprite.h"

sprite_t* Sprite_Handle::sprite_new(const string& filename, int width, int height, float scale) {
    auto* s = new sprite_t;

    s->image.loadFromFile(filename);
    Texture texture;
    texture.loadFromImage(s->image);
    Sprite sprite(texture);
    s->color_mask.r = 0;
    s->color_mask.g = 0;
    s->color_mask.b = 248;
    s->width = width;
    s->height = height;
    s->scale = scale;
    s->draw_top = false;
    s->frames_per_row = 6;
    s->frame = 0;
    s->frame_x = 0;
    s->frame_y = 0;
    s->flag = false;
    s->animation_max = 10;
    s->animations.reserve(s->animation_max);
    s->animation_time = 0.0f;
    s->animation_count = 0;
    s->animation_current = 0;

    return s;
}


void Sprite_Handle :: sprite_delete(sprite_t* sprite) {
    sprite->animations.clear();
    free(sprite);
}


animation_t *Animation_Handle::sprite_add_animation(vector<sprite_t>& sprites, const string& name, int start, int end, int reverse_loop) {
    if (sprites.empty()) {
        return nullptr;
    }

    for (sprite_t& sprite : sprites) {
        if (sprite.animation_count >= sprite.animations.size()) {
            sprite.animations.resize(sprite.animation_count + 1);
        }

        animation_t* anim = &sprite.animations[sprite.animation_count];
        anim->name = name;
        anim->frame_start = start;
        anim->frame_end = end;
        anim->speed = 1.0f;
        anim->reverse_loop = reverse_loop;
        anim->play_once = 0;
        anim->flag = 0;
        anim->animation_ended = 0;
        sprite.animation_current = sprite.animation_count;
        sprite.animation_count++;
    }

    return &sprites[0].animations[sprites[0].animation_count - 1];
}


 vector<animation_t> * Animation_Handle :: sprite_current_animation(sprite_t * sprite) {
    return reinterpret_cast<vector<animation_t> *>(&sprite->animations[sprite->animation_current]);
}

animation_t* Animation_Handle::sprite_set_animation(vector<sprite_t>& sprites, const string& name) {
    for (sprite_t& sprite : sprites) {
        for (int i = 0; i < sprite.animation_count; ++i) {
            animation_t* anim = &sprite.animations[i];
            if (anim->name == name) {
                if (sprite.animation_current != i) {
                    sprite.animation_current = i;
                    sprite.frame = anim->frame_start;
                    anim->flag = 0;
                    anim->animation_ended = 0;
                }
                return anim;
            }
        }
    }

    return nullptr;
}

void Sprite_Handle::sprite_update(vector<sprite_t>& sprites, float dt) {
    for (sprite_t& sprite : sprites) {
        if (sprite.animation_count) {
            animation_t* anim = &sprite.animations[sprite.animation_current];
            if (sprite.animation_time <= 0.0) {
                anim->animation_ended = 0;
                if (anim->reverse_loop) {
                    if (anim->flag == 0) {
                        if (sprite.frame < anim->frame_end) {
                            sprite.frame += 1;
                        } else {
                            anim->flag = 1;
                        }
                    } else {
                        if (sprite.frame > anim->frame_start) {
                            sprite.frame -= 1;
                        } else {
                            if (anim->play_once == 1) {
                                sprite.frame = anim->frame_start;
                                anim->animation_ended = 1;
                            } else {
                                anim->flag = 0;
                            }
                        }
                    }
                } else {
                    if (sprite.frame < anim->frame_end)
                        sprite.frame += 1;
                    else {
                        if (anim->play_once == 0) {
                            sprite.frame = anim->frame_start;
                        } else {
                            sprite.frame = anim->frame_end;
                        }
                        anim->animation_ended = 1;
                    }
                }
                sprite.animation_time = 8;
            }

            sprite.animation_time -= anim->speed * dt;
        }
    }
}



void Sprite_Handle :: sprite_draw(vector<sprite_t>& sprites, vector3d_t* pos, int direction) {
    for (auto& sprite : sprites) {
        int width = sprite.width;
        int height = sprite.height;

        sprite.frame_x = sprite.frame % sprite.frames_per_row;
        sprite.frame_y = floor(sprite.frame / sprite.frames_per_row);

        draw_t draw;

        if (direction == CHARACTER_DIRECTION_RIGHT)
            draw.flip = 0;
        else {
            draw.flip = 1;
        }
        draw.image = sprite.image;
        draw.x = pos->x;
        draw.y = pos->y;
        draw.z = pos->z;
        draw.width = width * (int)sprite.scale;
        draw.height = height * (int) sprite.scale;
        draw.src_x = (sprite.frame_x * width);
        draw.src_y = (sprite.frame_y * height);
        draw.src_width = width;
        draw.src_height = height;
        draw.color_mask = sprite.color_mask;
        draw.draw_top = sprite.draw_top;
        //graphics_draw(&draw);

    }
}


void Sprite_Handle :: sprite_draw_2(sprite_t* sprite, draw_t* draw) {
    int width = sprite->width;
    int height = sprite->height;

    sprite->frame_x = sprite->frame % sprite->frames_per_row;
    sprite->frame_y = floor(sprite->frame / sprite->frames_per_row);

    draw->flip = 0;
    draw->image = sprite->image;
    draw->width = width * (int)sprite->scale;
    draw->height = height * (int) sprite->scale;

    draw->src_x = (sprite->frame_x * width);
    draw->src_y = (sprite->frame_y * height);
    draw->src_width = width;
    draw->src_height = height;
    draw->color_mask = sprite->color_mask;
    draw->draw_top = sprite->draw_top;
}