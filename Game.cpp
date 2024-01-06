#include "Game.h"
#include "Sprite.h"
#include "Utils.h"
#include "Player.h"
#include "Enemy.h"
#include "Graphics.h"
#include "Character.h"

game_t* game;

const float PI = 22.0f/7.0;

static LRESULT CALLBACK main_window_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void game_spawn_enemies();
void game_draw_debug(HDC hdc);

void game_init_dblBuffer() {
    RECT rcClient;
    GetClientRect(game->hwnd, &rcClient);
    if (game->dbl_buffer != nullptr) {
        DeleteObject(game->dbl_buffer);
    }
    HDC hdc = GetDC(game->hwnd);
    game->dbl_buffer = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
    ReleaseDC(game->hwnd, hdc);
}

bool game_init_window() {
    game->height = 800;
    game->width = 1500;
    game->top_margin = 30;
    game->dbl_buffer = nullptr;

    const wchar_t window_class_name[] = L"main_window_class";

    wcscpy(game->title, L"Cadillacs & Dinosaurs");

    WNDCLASSEXW winCl;

    winCl.hInstance = GetModuleHandleW(0);
    winCl.lpszClassName = window_class_name;
    winCl.lpfnWndProc = main_window_proc;
    winCl.style = CS_DBLCLKS;
    winCl.cbSize = sizeof (WNDCLASSEX);

    winCl.hIcon = LoadIconW(nullptr, (LPCWSTR) IDI_APPLICATION);
    winCl.hIconSm = LoadIconW(nullptr, (LPCWSTR) IDI_APPLICATION);
    winCl.hCursor = LoadCursorW(nullptr, (LPCWSTR) IDC_ARROW);
    winCl.lpszMenuName = nullptr;
    winCl.cbClsExtra = 0;
    winCl.cbWndExtra = 0;
    winCl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    if (RegisterClassExW(&winCl)) {
        game->hwnd = CreateWindowExW(
                0,
                window_class_name,
                game->title,
                WS_VISIBLE | WS_OVERLAPPED,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                game->width,
                game->height,
                HWND_DESKTOP,
                nullptr,
                winCl.hInstance,
                nullptr);
    } else {
        return false;
    }

    game_init_dblBuffer();

    ShowWindow(game->hwnd, SW_SHOW);
    return true;
}

HGDIOBJ game_load_image(const wchar_t* path) {
    return LoadImageW(nullptr, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}


void game_load_resources() {
    game->bmp_bg = game_load_image(L"E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/ep-5.bmp");
    game->bmp_bg_far = game_load_image(L"E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/ep-5-far.bmp");
    game->bmp_level_layers.reserve(10);
    game->bmp_level_layers[0] = game_load_image(L"E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/ep-5-layer.bmp");

    game->bmp_text = game_load_image(L"E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/ep-5.bmp/smack.bmp");

    game->level_fire = Sprite_Handle::sprite_new(L"E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/ep-5-animation.bmp", 320, 224, 1.0);

    game->level_fire->frames_per_row = 4;
    Animation_Handle::sprite_add_animation(game->level_fire, L"fire", 0, 3, 0);
    Animation_Handle::sprite_set_animation(game->level_fire, L"fire");

    game->bmp_avatar_player = game_load_image(L"E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/jack.bmp");

    game->font = Utils::create_font(L"Arial", 20);

    game->fx_punch.sprite = Sprite_Handle::sprite_new(L"E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/punch.bmp", 120, 120, 1.4);
    game->fx_punch.sprite->frames_per_row = 4;
    game->fx_punch.anim = Animation_Handle::sprite_add_animation(game->fx_punch.sprite, L"punch1", 0, 3, 0);
    game->fx_punch.sprite->draw_top = 1;
    game->fx_punch.draw_punch = 0;
    Utils::vector3d_zero(&game->fx_punch.pos);

    game->fx_blood.sprite = Sprite_Handle::sprite_new(L"E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/blood.bmp", 120, 120, 1.2);
    game->fx_blood.sprite->frames_per_row = 4;
    game->fx_blood.anim = Animation_Handle::sprite_add_animation(game->fx_blood.sprite, L"punch1", 0, 6, 0);
    game->fx_blood.anim->speed = 1.5f;

    game->fx_blood.sprite->draw_top = 1;
    game->fx_blood.draw_punch = 0;
    Utils::vector3d_zero(&game->fx_blood.pos);

    game->fx_smack_time = 0;
    game->fx_smack_x = 0;
    game->fx_smack_y = 0;
}

void game_start_new() {
    game->view_x = 0;
    game->view_x_far = 0;

    game->enemy_count = 0;

    game_spawn_enemies();

    game->state = GAME_STATE_INTRO;
}

bool game_init() {
    game = new game_t;
    memset(game->keyboard_state, 0, sizeof(game->keyboard_state));

    if (!game_init_window()) {
        return false;
    }

    game_load_resources();

    game->draw_max = 100;
    game->draw_list = new draw_t[game->draw_max];
    game->draw_count = 0;

    game->time_text_blink = 0;
    game->intro_time = 300;

    game->max_z = 70;
    game->min_z = -70;

    game->view_x = 0.0f;
    game->view_x_far = 0.0f;

    game->max_view_x = 0;

    game->ground_y = 520;
    game->gravity = 0.8f;

    game->state = GAME_STATE_MENU;

    game->player = Player::player_new();

    game->enemy_max = 100;
    game->enemies = new enemy_t[game->enemy_max];
    game->enemy_count = 0;

    game->spawn_trigger = 0;

    return true;
}


void game_spawn_enemies() {

    Enemy::enemy_spawn(10, 40, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(800, 80, ENEMY_TYPE_GNEISS);
    Enemy::enemy_spawn(900, -50, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(1000, 80, ENEMY_TYPE_FERRIS);
    Enemy::enemy_spawn(1150, 0, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(1200, 80, ENEMY_TYPE_FERRIS);
    Enemy::enemy_spawn(1350, 0, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(1500, 80, ENEMY_TYPE_FERRIS);
    Enemy::enemy_spawn(1600, 20, ENEMY_TYPE_GNEISS);
    Enemy::enemy_spawn(1700, 30, ENEMY_TYPE_FERRIS);
    Enemy::enemy_spawn(1900, 70, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(2200, 80, ENEMY_TYPE_FERRIS);
    Enemy::enemy_spawn(2500, 20, ENEMY_TYPE_GNEISS);
    Enemy::enemy_spawn(2800, 30, ENEMY_TYPE_FERRIS);
    Enemy::enemy_spawn(3100, 70, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(3550, 0, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(3800, 80, ENEMY_TYPE_FERRIS);
    Enemy::enemy_spawn(3900, 20, ENEMY_TYPE_GNEISS);
    Enemy::enemy_spawn(4300, 30, ENEMY_TYPE_FERRIS);
    Enemy::enemy_spawn(4700, 70, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(5200, 20, ENEMY_TYPE_GNEISS);
    Enemy::enemy_spawn(5300, -50, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(6500, -20, ENEMY_TYPE_GNEISS);
    Enemy::enemy_spawn(7000, 30, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(8500, 40, ENEMY_TYPE_BUTCHER);


}

void game_cleanup_enemies() {
    for(int i = 0;i < game->enemy_count;i++) {
        enemy_t* enemy = &game->enemies[i];
        Enemy::enemy_cleanup(enemy);
    }
    free(game->enemies);
    game->enemy_count = 0;
}

void game_cleanup_resources() {
    DeleteObject(game->bmp_avatar_player);
    DeleteObject(game->bmp_bg);

}

void game_delete() {
    if (game != nullptr) {
        game_cleanup_enemies();
        Player::player_delete(game->player);
        free(game->draw_list);
        game_cleanup_resources();
        DeleteObject(game->font);
        DeleteObject(game->dbl_buffer);
        free(game);
    }
}

void game_draw_avatar(HDC hdc, int x, int y) {
    HDC hdc_bg = CreateCompatibleDC(hdc);
    SelectObject(hdc_bg, game->bmp_avatar_player);

    StretchBlt(hdc, x, y, 48, 48, hdc_bg, 0, 0, 24, 24, SRCCOPY);

    DeleteDC(hdc_bg);
}

void game_draw_player_statistics(HDC hdc, int x) {
    wchar_t str_score[256];

    game_draw_avatar(hdc, x, 5);

    SetTextColor(hdc, RGB(255, 255, 255));
    swprintf(str_score, 256, L"Jack");
    TextOutW(hdc, x + 60, 20, str_score, wcslen(str_score));

    SetTextColor(hdc, RGB(255, 255, 0));
    swprintf(str_score, 256, L"%05d", game->player->score);
    TextOutW(hdc, x + 200, 20, str_score, wcslen(str_score));

    Graphics::graphics_draw_lifeBar(hdc, x, 60, 20, 300, game->player->base.health);
}

float level_stretch = 0.4f;
float org_width = 3664;

void game_draw_level(HDC hdc, RECT* rect) {
    HDC hdc_bg = CreateCompatibleDC(hdc);

    float level_scaled = (1.0 / level_stretch);

    game->max_view_x = (org_width * level_scaled - rect->right);

    float stretched_width = (float)rect->right * level_stretch;

    game->rect_width = rect->right;

    SelectObject(hdc_bg, game->bmp_bg_far);

    //SetBkColor(hdc, RGB(0xFF, 0xFF, 0xFF));
    StretchBlt(hdc, 0, 0, rect->right, rect->bottom * 0.7,
               hdc_bg, game->view_x_far * (level_stretch - 0.3), 0, stretched_width, 144, SRCCOPY);

    SelectObject(hdc_bg, game->bmp_bg);

    TransparentBlt(hdc, 0, 0, rect->right, rect->bottom,

                   hdc_bg, game->view_x * level_stretch, 0, stretched_width, 224, RGB(255, 174, 201));

    DeleteDC(hdc_bg);
}

void game_draw_level_layers(HDC hdc, RECT* rect) {

    HDC hdc_bg = CreateCompatibleDC(hdc);
    SelectObject(hdc_bg, game->bmp_level_layers[0]);

    float stretched_width = (float)rect->right * level_stretch;

    TransparentBlt(hdc, 0, 0, rect->right, rect->bottom,
                   hdc_bg, game->view_x * level_stretch, 0, stretched_width, 224, RGB(255, 174, 201));

    DeleteDC(hdc_bg);
}


void effect_draw(effect_t* fx, HDC hdc) {
    if (fx->draw_punch) {
        Sprite_Handle::sprite_draw(fx->sprite, &fx->pos, CHARACTER_DIRECTION_RIGHT, hdc);
    }
}

void game_draw_level_fire(HDC hdc, RECT* rect) {
    float level_scaled = 1.0 / level_stretch;

    draw_t tDraw{};
    draw_t* draw = &tDraw;
    Sprite_Handle::sprite_draw_2(game->level_fire, draw);

    float scaled_width = draw->width * level_scaled;
    float gap = rect->right - scaled_width;

    Graphics::graphics_draw_surface(hdc, draw->hbitmap,
                          (-game->view_x) + game->max_view_x + gap, 0, scaled_width, rect->bottom,
                          draw->src_x, draw->src_y, draw->src_width, draw->src_height,
                          draw->flip, draw->color_mask);
}

void game_draw(HDC hdc, RECT* rect) {
    const int state = game->state;
    wchar_t buf[256];

    Graphics::graphics_clear();

    RECT rcText = *rect;

    SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, game->font);
    SetTextColor(hdc, RGB(255, 255, 0));

    if (state == GAME_STATE_MENU) {
        // draw the moving background
        game_draw_level(hdc, rect);
        if (game->time_text_blink < 50 ) {
            swprintf(buf, 256, L"Press Enter to Start");
            DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    } else if (state == GAME_STATE_PLAYING ||
               state == GAME_STATE_PAUSED ||
               state == GAME_STATE_OVER ||
               state == GAME_STATE_INTRO ||
               state == GAME_STATE_CUTSCENE ||
               state == GAME_STATE_WIN) {
        // draw the moving background
        game_draw_level(hdc, rect);
        game_draw_level_fire(hdc, rect);

        for(int i = 0;i < game->enemy_count;i++) {
            enemy_t* enemy = &game->enemies[i];
            Enemy::enemy_draw(enemy, hdc);
        }

        Player::player_draw(game->player, hdc);

        effect_draw(&game->fx_punch, hdc);
        effect_draw(&game->fx_blood, hdc);

        Graphics::graphics_swap_buffer(hdc, rect);

        for(int i = 0;i < game->enemy_count;i++) {
            enemy_t* enemy = &game->enemies[i];
            character_t* ch = &enemy->base;
            if (ch->health > 0 && (enemy->engaging == 1 || ch->state == CHARACTER_STATE_HIT)) {
                vector3d_t center{};
               Character::character_get_center(ch, &center);
                int width = 70;
                int height = 8;
                float x = (-game->view_x) + center.x - (width/2);
                float y = ch->position.y + ch->position.z - (ch->sprite->height * ch->sprite->scale);

                Graphics::graphics_draw_lifeBar(hdc, (int)x, (int)y, height, width, ch->health);
            }
        }

        if (game->fx_smack_time > 0) {
            HDC hdc_smack = CreateCompatibleDC(hdc);
            SelectObject(hdc_smack, game->bmp_text);

            float width = 130 * 1.2;
            float height = 107 * 1.2;

            float x = ((-game->view_x) + game->fx_smack_x) - (width / 2);
            float y = game->fx_smack_y - (height / 2);

            TransparentBlt(hdc, x, y, width, height,
                           hdc_smack, 0, 0, 130, 107, RGB(0, 0, 248));

            DeleteDC(hdc_smack);
        }


        game_draw_level_layers(hdc, rect);

        SetBkMode(hdc, TRANSPARENT);
        SelectObject(hdc, game->font);

        int player_stat_x = 10;

        game_draw_player_statistics(hdc, player_stat_x);



        if(state == GAME_STATE_PAUSED) {
            if (game->time_text_blink < 50 ) {
                swprintf(buf, 256, L"Press Enter to Resume");
                DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }

            rcText.top -= 50;
            rcText.bottom -= 50;

            SelectObject(hdc, game->font_big);
            swprintf(buf, 256, L"PAUSED");
            DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        } else if(state == GAME_STATE_WIN) {
            if (game->time_text_blink < 50 ) {
                swprintf(buf, 256, L"YOU WIN!!!");
                DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        } else if(state == GAME_STATE_OVER) {
            if (game->time_text_blink < 50 ) {
                swprintf(buf, 256, L"YOU DIED");
                DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        } else if (state == GAME_STATE_INTRO) {
            if (game->intro_time > 200) {
                swprintf(buf, 256, L"READY?");
                DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            } else if (game->intro_time > 100) {
                swprintf(buf, 256, L"STEADY??");
                DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            } else {
                swprintf(buf, 256, L"GO FIGHT!!!");
                if (game->time_text_blink < 50 ) {
                    DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
            }
        }
    }
}


void game_draw_debug(HDC hdc) {
    for (int i = 0; i < game->enemy_count; i++) {
        enemy_t *enemy = &game->enemies[i];
        character_t *ch = &enemy->base;
        for (int j = 0; j < ch->hit_box_count; j++) {
            RECT *box = &ch->hit_boxes[j];
            Graphics::graphics_draw_rect(hdc, box, RGB(255, 0, 0));
        }
    }

    for (int i = 0; i < game->player->base.hit_box_count; i++) {
        RECT *box = &game->player->base.hit_boxes[i];
        Graphics::graphics_draw_rect(hdc, box, RGB(0, 255, 0));
    }
}


void enemy_remove(int index) {
    int count = game->enemy_count - 1;
    for(int i = index; i < count; i++) {
        game->enemies[i] = game->enemies[i + 1];
    }
    game->enemy_count--;
}

void effect_update(effect_t* fx, float dt) {
    Sprite_Handle::sprite_update(fx->sprite, dt);

    animation_t* anim = fx->anim;
    if (anim->animation_ended) {
        fx->draw_punch = 0;
    }
}


void game_update_enemies(float dt) {
    int idx_remove = -1;

    for(int i = 0;i < game->enemy_count;i++) {
        enemy_t* enemy = &game->enemies[i];
        if (enemy->marked_for_delete) {
            idx_remove = i;
        } else {
            Enemy::enemy_update(enemy, i, dt);
        }
    }
    if (idx_remove != -1) {
        enemy_remove(idx_remove);
    }
}

void game_trigger_spawn_enemies(float dt) {
    float x = game->player->base.position.x;
    enemy_t* e;
    if (game->spawn_trigger == 0) {
        if (x > 1000) {
            game->spawn_trigger++;

            e = Enemy::enemy_spawn(game->view_x - 100, -50, ENEMY_TYPE_FERRIS);
            e->base.state = CHARACTER_STATE_RUNNING;

            e = Enemy::enemy_spawn(game->view_x - 200, 0, ENEMY_TYPE_FERRIS);
            e->base.state = CHARACTER_STATE_RUNNING;
        }
    } else if (game->spawn_trigger == 1) {
        if (x > 2000) {
            game->spawn_trigger++;

            e = Enemy::enemy_spawn(game->view_x + game->rect_width, 0, ENEMY_TYPE_GNEISS);
            e->base.state = CHARACTER_STATE_RUNNING;

            e = Enemy::enemy_spawn(game->view_x - 100, 50, ENEMY_TYPE_FERRIS);
            e->base.state = CHARACTER_STATE_RUNNING;
        }
    } else if (game->spawn_trigger == 2) {
        if (x > 4000) {
            game->spawn_trigger++;

            e = Enemy::enemy_spawn(game->view_x + game->rect_width, 0, ENEMY_TYPE_GNEISS);
            e->base.state = CHARACTER_STATE_RUNNING;

            e = Enemy::enemy_spawn(game->view_x - 100, 50, ENEMY_TYPE_FERRIS);
            e->base.state = CHARACTER_STATE_RUNNING;
        }
    } else if (game->spawn_trigger == 3) {
        if (x > 5000) {
            game->spawn_trigger++;

            e = Enemy::enemy_spawn(game->view_x + game->rect_width, 50, ENEMY_TYPE_GNEISS);
            e->base.state = CHARACTER_STATE_RUNNING;

            e = Enemy::enemy_spawn(game->view_x - 100, -50, ENEMY_TYPE_FERRIS);
            e->base.state = CHARACTER_STATE_RUNNING;
        }
    } else if (game->spawn_trigger == 4) {
        if (x > 6000) {
            game->spawn_trigger++;

            e = Enemy::enemy_spawn(game->view_x + game->rect_width, 50, ENEMY_TYPE_GNEISS);
            e->base.state = CHARACTER_STATE_RUNNING;

            e = Enemy::enemy_spawn(game->view_x - 100, -50, ENEMY_TYPE_GNEISS);
            e->base.state = CHARACTER_STATE_RUNNING;
        }
    } else if (game->spawn_trigger == 5) {
        if (x > 7000) {
            game->spawn_trigger++;

            e = Enemy::enemy_spawn(game->view_x - 100, -50, ENEMY_TYPE_FERRIS);
            e->base.state = CHARACTER_STATE_RUNNING;

            e = Enemy::enemy_spawn(game->view_x - 200, 0, ENEMY_TYPE_FERRIS);
            e->base.state = CHARACTER_STATE_RUNNING;
        }
    }
}

void game_update(float dt) {
    const int state = game->state;

    if (state == GAME_STATE_PLAYING) {

        Sprite_Handle::sprite_update(game->level_fire, dt);

        Player::player_update(game->player, dt);

        game_update_enemies(dt);

        effect_update(&game->fx_punch, dt);
        effect_update(&game->fx_blood, dt);

        if (game->fx_smack_time > 0) {
            game->fx_smack_time -= dt;
        }

        // spawn
        game_trigger_spawn_enemies(dt);

    } else if (state == GAME_STATE_INTRO) {
        if (game->time_text_blink > 100) {
            game->time_text_blink = 0;
        }
        game->time_text_blink += dt * 3.5;
    } else {
        if (game->time_text_blink > 100) {
            game->time_text_blink = 0;
        }
        game->time_text_blink += dt;
    }

    if (state == GAME_STATE_WIN) {
        Sprite_Handle::sprite_update(game->level_fire, dt);
        Player::player_update(game->player, dt);
    } else if (state == GAME_STATE_INTRO) {
        Player::player_update(game->player, dt);
        game_update_enemies(dt);

        if (game->intro_time > 0) {
            game->intro_time -= dt;
        } else {
            game->state = GAME_STATE_PLAYING;
        }

    } else if (state == GAME_STATE_MENU) {
        game->view_x += (float)1.1 * dt;
        game->view_x_far = game->view_x * 0.6f;

        if (game->view_x >= game->max_view_x) {
            game->view_x = 0;
        }
    }
}


void game_exit() {
    PostQuitMessage(0);
}

void game_check_input() {
    if (game->state == GAME_STATE_PLAYING) {
        if (game->keyboard_state[VK_LEFT]||game->keyboard_state['A']) {
            if (game->player->base.health > 0) {
                game->player->base.velocity.x = -game->player->movement_speed;
                if (game->player->base.on_ground)
                    Player::player_set_state(game->player, CHARACTER_STATE_WALKING);
            }

        } else if (game->keyboard_state[VK_RIGHT] || game->keyboard_state['D']) {
            if (game->player->base.health > 0) {
                game->player->base.velocity.x = game->player->movement_speed;
                if (game->player->base.on_ground)
                    Player::player_set_state(game->player, CHARACTER_STATE_WALKING);
            }
        } else {
            game->player->base.velocity.x = 0;
        }

        if (game->keyboard_state[VK_UP] || game->keyboard_state['W'] ) {
            if (game->player->base.health > 0) {
                game->player->base.velocity.z = -game->player->movement_speed;
                if (game->player->base.on_ground)
                   Player::player_set_state(game->player, CHARACTER_STATE_WALKING);
            }
        } else if (game->keyboard_state[VK_DOWN] || game->keyboard_state['S']) {
            if (game->player->base.health > 0) {
                game->player->base.velocity.z = game->player->movement_speed;
                if (game->player->base.on_ground)
                    Player::player_set_state(game->player, CHARACTER_STATE_WALKING);
            }
        } else {
            game->player->base.velocity.z = 0;
        }
    }
}


bool player_can_attack = true;

void game_input(int key, int down) {
    if (key == VK_ESCAPE) {
        game_exit();
    }

    if (down) {
        game->keyboard_state[key] = 1;

        if (key == VK_RETURN) {
            if (game->state == GAME_STATE_MENU)
                game_start_new();
            else if (game->state == GAME_STATE_PLAYING)
                game->state = GAME_STATE_PAUSED;
            else if (game->state == GAME_STATE_PAUSED)
                game->state = GAME_STATE_PLAYING;
        }

        if (game->state == GAME_STATE_PLAYING) {
            if (game->player->base.health > 0) {
                if (key == VK_SPACE && player_can_attack == 1) {
                    Player::player_set_state(game->player, CHARACTER_STATE_FIGHT);
                    player_can_attack = false;
                }
                if (key == VK_CONTROL) {
                    Player::player_set_state(game->player, CHARACTER_STATE_JUMP);
                }
                if ((key == 'C' || key == 'c') && game->player->base.on_ground) {

                    Player::player_set_state(game->player, CHARACTER_STATE_FIGHT);
                }
            }
        }

        if (key == 'P' || key == 'p') {
            if (game->state == GAME_STATE_PLAYING)
                game->state = GAME_STATE_PAUSED;
            else if (game->state == GAME_STATE_PAUSED)
                game->state = GAME_STATE_PLAYING;
        }
    } else {
        if (game->state == GAME_STATE_PLAYING) {
            if (game->player->base.health > 0) {
                switch (key) {
                    case VK_LEFT:
                    case VK_RIGHT:
                    case VK_UP:
                    case VK_DOWN:
                       Player:: player_set_state(game->player, CHARACTER_STATE_IDLE);
                        break;
                    case VK_SPACE:
                    case 'C':
                    case 'c':
                        player_can_attack = true;
                        break;
                }
            }
        }
        game->keyboard_state[key] = 0;
    }
}



LRESULT CALLBACK main_window_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_KEYDOWN: {
            game_input((signed int)wParam, 1);
            break;
        }
        case WM_KEYUP: {
            game_input((signed int)wParam, 0);
            break;
        }
        case WM_ERASEBKGND: {
            return 1;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            HDC hdc = ps.hdc;
            HDC hdcBuffer = CreateCompatibleDC(hdc);
            SelectObject(hdcBuffer, game->dbl_buffer);

            RECT rcClient;
            GetClientRect(hwnd, &rcClient);

            HBRUSH page_bgk_brush = CreateSolidBrush(RGB(127, 184, 145));
            FillRect(hdcBuffer, &rcClient, page_bgk_brush);

            //SetStretchBltMode(hdcBuffer, HALFTONE);

            game_draw(hdcBuffer, &rcClient);

            BitBlt(hdc, 0,0, rcClient.right, rcClient.bottom, hdcBuffer, 0,0, SRCCOPY);

            DeleteDC(hdcBuffer);

            DeleteObject(page_bgk_brush);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hwnd, message, wParam, lParam);
    }
    return 0;
}


void game_run() {
    MSG msg;
    LARGE_INTEGER start_counter, end_counter, frequency;
    double seconds_per_frame;
    QueryPerformanceCounter(&start_counter);
    QueryPerformanceFrequency(&frequency);
    while (true) {
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if(msg.message == WM_QUIT)
            break;
        QueryPerformanceCounter(&end_counter);

        seconds_per_frame = (((double)(end_counter.QuadPart -
                                       start_counter.QuadPart) * 100.0f) / (double)frequency.QuadPart);
        start_counter = end_counter;
        game_check_input();
        game_update((float)seconds_per_frame);
        InvalidateRect(game->hwnd, nullptr, 1);
    }
}




    int main() {
        // Create the window
        RenderWindow window(sf::VideoMode(1400, 560), "Player Selection");

        // Load the background image
        Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/bg1.png")) {
            return 1;
        }
        Sprite backgroundSprite(backgroundTexture);

        // Load the four images
        Texture texture1, texture2, texture3, texture4;
        if (!texture1.loadFromFile("E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/image1.png") ||
            !texture2.loadFromFile("E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/image2.png") ||
            !texture3.loadFromFile("E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/image3.png") ||
            !texture4.loadFromFile("E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/image4.png")) {

            return 1;
        }

        // Create sprites for each image
        Sprite sprite1(texture1), sprite2(texture2), sprite3(texture3), sprite4(texture4);

        // Set the positions of the images
        sprite1.setPosition(150, 50);
        sprite2.setPosition(350, 50);
        sprite3.setPosition(550, 50);
        sprite4.setPosition(750, 50);


        // Create text for the button and selection message
        Font font;
        if (!font.loadFromFile("E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Font/RobotoCondensed-Italic.ttf")) {
            return 1;
        }


        Text selectionText("Select Player Jack if you want", font, 20);
        selectionText.setPosition(50, 400);

        while (window.isOpen()) {
            Event event{};
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::G) {
                        selectionText.setString("Player Jack selected!");
                    }

                    if (event.key.code == Keyboard::U) {
                        selectionText.setString("Confirmation!");
                        window.close();
                        if (game_init()) {
                            game_run();
                            game_delete();
                        }
                    }
                }
            }

            window.clear();

            // Draw the background
            window.draw(backgroundSprite);

            // Draw the images and the button
            window.draw(sprite1);
            window.draw(sprite2);
            window.draw(sprite3);
            window.draw(sprite4);



            // Draw the selection message
            window.draw(selectionText);

            window.display();



        }

        return 0;
    }
