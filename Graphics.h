
#ifndef CADILLACS_AND_DINOSAURS_GAME_GRAPHICS_H
#define CADILLACS_AND_DINOSAURS_GAME_GRAPHICS_H
#include "Game.h"

class Graphics {


public:
    static void graphics_clear();
    static void graphics_draw(draw_t* draw);
    static void graphics_draw_rect(HDC hdc, RECT* rect, COLORREF color);
    static void graphics_draw_surface(HDC hdc, HBITMAP bitmap, int x, int y, int w, int h,
                                      int src_x, int src_y, int src_w, int src_h, int flip, COLORREF key);
    static void graphics_draw_lifeBar(HDC hdc, int x, int y, int height, int width, float life);
    static void graphics_swap_buffer(HDC hdc, RECT* rect);







};


#endif
