#include "Graphics.h"



void Graphics ::graphics_clear() {

        game->draw_count = 0;



}

void Graphics ::graphics_draw(draw_t *args) {

    if (game->draw_count >= game->draw_max) {
        return;
    }

    draw_t* draw = &game->draw_list[game->draw_count];
    copy(args, args + sizeof(draw_t), draw);
    game->draw_count++;


}


void Graphics ::graphics_draw_rect(RenderWindow &window, FloatRect &rect, Color &color) {


    RectangleShape rectangle(Vector2f(rect.width, rect.height));
    rectangle.setPosition(rect.left - game->view_x, rect.top);
    rectangle.setFillColor(color);

    window.draw(rectangle);



}

void Graphics ::graphics_draw_surface(RenderWindow &window, Image &image, int x, int y, int w, int h, int src_x,
                                      int src_y, int src_w, int src_h, int flip, Color &key) {

        Texture texture;
        texture.loadFromImage(image);
        Sprite sprite(texture);
        sprite.setTextureRect(IntRect(src_x, src_y, src_w, src_h));
        sprite.setPosition((float)x, (float)y);
        sprite.setScale(flip ? -1.0f : 1.0f, 1.0f);
        sprite.setColor(key);

        window.draw(sprite);
    }


int compare_draw(const void* l, const void* r) {
    const auto* a = static_cast<const draw_t*>(l);
    const auto* b = static_cast<const draw_t*>(r);

    if (a->draw_top)
        return 1;
    else if (b->draw_top)
        return -1;

    if (a->z == b->z)
        return 0;
    else if (a->z < b->z)
        return -1;

    return 1;
}


void Graphics::graphics_swap_buffer(RenderWindow &window) {

    qsort(game->draw_list, game->draw_count, sizeof(draw_t), compare_draw);


    for (int i = 0; i < game->draw_count; i++) {
        struct draw_t *draw = &game->draw_list[i];

        float depth = draw->z;

        // to prevent flicker (same z position)
        if (!draw->draw_top) {
            for (int j = 0; j < game->draw_count; j++) {
                draw_t *jDraw = &game->draw_list[j];
                float diff = jDraw->z - depth;
                if (diff < 1) {
                    depth += 1;
                }
            }
        }

        float x = (-game->view_x) + draw->x;
        float y = draw->y + depth;


        Graphics::graphics_draw_surface(window, draw->image,
                                        (int)x, (int)y - draw->height, draw->width, draw->height,
                              draw->src_x, draw->src_y, draw->src_width, draw->src_height,
                              draw->flip, draw->color_mask);




    }

}



void Graphics :: graphics_draw_lifeBar(RenderWindow& window, int x, int y, int height, int width, float life) {
    RectangleShape lifeBar(Vector2f(static_cast<float>(width) * (life / 100.0f), static_cast<float>(height)));
    lifeBar.setPosition((float)x, (float)y);
    lifeBar.setFillColor(Color(255, 255, 0));

    window.draw(lifeBar);

    RectangleShape border(Vector2f((float)width, (float)height));
    border.setPosition((float)x,(float) y);
    border.setFillColor(Color::Transparent);
    border.setOutlineThickness(2);
    border.setOutlineColor(Color(255, 255, 255));

    window.draw(border);
}


