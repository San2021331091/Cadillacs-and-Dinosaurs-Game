#ifndef CADILLACS_AND_DINOSAUR_GAME_GRAPHICS_H
#define CADILLACS_AND_DINOSAUR_GAME_GRAPHICS_H

#include "Game.h"

class Graphics {

public:
    static void graphics_clear();
    static void graphics_draw(draw_t * args);
    static void graphics_draw_rect( RenderWindow & window, FloatRect &rect, Color &color);
   static void graphics_draw_surface(RenderWindow & window, Image &image, int x, int y, int w, int h,
                               int src_x, int src_y, int src_w, int src_h, int flip, Color &key);
    static void graphics_draw_lifeBar(RenderWindow &window, int x, int y, int height, int width, float life);
    static void graphics_swap_buffer(RenderWindow &window);












};


#endif
