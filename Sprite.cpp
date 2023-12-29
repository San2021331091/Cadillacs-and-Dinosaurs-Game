#include "Sprite.h"


sprite_t * Sprite_Handle :: sprite_new(string &filename, int width, int height, float scale){

    auto * s = new sprite_t();
    s->image.loadFromFile(filename);
    Texture texture;
    texture.loadFromImage(s->image);
    Sprite sprite(texture);
    s->color_mask.r=0;
    s->color_mask.g=0;
    s->color_mask.b=248;
    s->width = width;
    s->height = height;
    s->scale = scale;
    s->draw_top = 0;
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

void Sprite_Handle :: sprite_delete( sprite_t* sprite){

         sprite->animations.clear();
         delete sprite;

}

animation_t * Animation_Handle :: sprite_add_animation(sprite_t * sprite, wstring &name, int start, int end, int reverse_loop) {
    if (sprite->animation_count >= sprite->animation_max) {
        return nullptr;
    }
    if (sprite->animation_count >= sprite->animations.size()) {
        sprite->animations.resize(sprite->animation_count + 1);
    }

    animation_t *anim = &sprite->animations[sprite->animation_count];
    anim->name = name;
    anim->frame_start = start;
    anim->frame_end = end;
    anim->speed = 1.0f;
    anim->reverse_loop = reverse_loop;
    anim->play_once = 0;
    anim->flag = 0;
    anim->animation_ended = 0;
    sprite->animation_current = sprite->animation_count;
    sprite->animation_count++;

    return anim;
}

 animation_t * sprite_current_animation(sprite_t * sprite) {
    return &sprite->animations[sprite->animation_current];
}

animation_t * Animation_Handle :: sprite_set_animation(sprite_t *sprite, wstring &name) {




}