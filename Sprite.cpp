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


void Sprite_Handle :: sprite_draw(sprite_t* sprite, vector3d_t* pos, int direction, RenderWindow& window) {
    // Assuming sprite->image is loaded and valid
    Texture texture;
    texture.loadFromImage(sprite->image);

    // Set the texture for the sprite
    Sprite spriteSF;
    spriteSF.setTexture(texture);

    // Set position
    spriteSF.setPosition(pos->x, pos->y);

    // Set scale
    spriteSF.setScale(sprite->scale, sprite->scale);

    // Set the texture rectangle for animation frame
    int frameWidth = sprite->width / sprite->frames_per_row;
    int frameHeight = static_cast<int>(sprite->height / (!sprite->animations.empty() ? sprite->animations.size() : 1));

    IntRect textureRect(
            sprite->frame_x * frameWidth,
            sprite->frame_y * frameHeight,
            frameWidth,
            frameHeight
    );
    spriteSF.setTextureRect(textureRect);

    // Draw the sprite to the window
    window.draw(spriteSF);

    // Update animation frame
    sprite->animation_time += 0.1; // Assuming a constant frame update for simplicity
    if (sprite->animation_time >= sprite->animations[sprite->animation_current].speed) {
        sprite->frame_x++;
        if (sprite->frame_x >= sprite->frames_per_row) {
            sprite->frame_x = 0;
            sprite->frame_y++;
            if (sprite->frame_y >= sprite->animations[sprite->animation_current].frame_end) {
                sprite->frame_y = sprite->animations[sprite->animation_current].frame_start;
                if (sprite->animations[sprite->animation_current].play_once) {
                    sprite->animations[sprite->animation_current].animation_ended = true;
                }
            }
        }
        sprite->animation_time = 0.0;
    }
}