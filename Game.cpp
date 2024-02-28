#include "Game.h"
#include "Sprite.h"
#include "Utils.h"
#include "Player.h"
#include "Enemy.h"
#include "Graphics.h"
#include "Character.h"
#include "FileLoad.h"
#include "Stack.h"
#include "RbTree.h"
#include "LevelGraph.h"
#include "Fenwick.h"

game_t* game;







static LRESULT CALLBACK main_window_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void game_spawn_enemies();

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
    game->height = 900;
    game->width = 1600;
    game->top_margin = 30;
    game->dbl_buffer = nullptr;

    const wchar_t window_class_name[] = L"main_window_class";

    wcscpy(game->title, L"Cadillacs & Dinosaurs Game");

    WNDCLASSEXW winCl;



    winCl.hInstance = GetModuleHandleW(nullptr);
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
                WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                game->width,
                game->height,
                HWND_DESKTOP,
                nullptr,
                winCl.hInstance,
                nullptr);
    }


    else {
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


    game->bmp_bg = game_load_image(FileLoad::getImageFile1("ep-5.bmp"));
    game->bmp_bg1 = game_load_image(FileLoad::getImageFile1("ep-1_01.bmp"));
    game->bmp_bg_far = game_load_image(FileLoad::getImageFile1("ep-5-far.bmp"));
    game->bmp_level_layers.reserve(10);
    game->bmp_level_layers[0] = game_load_image(FileLoad::getImageFile1("ep-5-layer.bmp"));

    game->bmp_text = game_load_image(FileLoad::getImageFile1("smack.bmp"));

    game->level_fire = Sprite_Handle::sprite_new(FileLoad::getImageFile1("ep-5-animation.bmp"), 320, 224, 1.0);

    game->level_fire->frames_per_row = 4;
    Animation_Handle::sprite_add_animation(game->level_fire, L"fire", 0, 3, 0);
    Animation_Handle::sprite_set_animation(game->level_fire, L"fire");

    game->bmp_avatar_player = game_load_image(FileLoad::getImageFile1("jack.bmp"));

    game->font = Utils::create_font(L"Arial", 20);

    game->fx_punch.sprite = Sprite_Handle::sprite_new(FileLoad::getImageFile1("punch.bmp"), 120, 120, 1.4);
    game->fx_punch.sprite->frames_per_row = 4;
    game->fx_punch.anim = Animation_Handle::sprite_add_animation(game->fx_punch.sprite, L"punch1", 0, 3, 0);
    game->fx_punch.sprite->draw_top = 1;
    game->fx_punch.draw_punch = 0;
    Utils::vector3d_zero(&game->fx_punch.pos);

    game->fx_blood.sprite = Sprite_Handle::sprite_new(FileLoad::getImageFile1("blood.bmp"), 120, 120, 1.2);
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



float mi = 1.0f,mx=3.0f;


void speedChange(float v1,float v2){
      mi = v1;
      mx = v2;
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

     FenwickTree fenwick(100, mi, mx); // Choose an appropriate size for the Fenwick tree
    vector<float> arr = {1.2f, 1.5f, 1.6,1.8f,1.9, 2.1f,2.2f,2.4f,2.7f,3.0f,3.1f,3.4f}; // Sample data
    for (float val : arr) {
        fenwick.addIndex(val); // Add all indices to the map
    }
    for (float val : arr) {
        fenwick.update(val, val); // Update values at indices
    }

    uint32_t seed = 123456789;
    XorShift32 rng(seed);
  

    game->player = Player::player_new(rng.nextFloat(mi,mx));

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

    auto level_scaled =(float) (1.0 / level_stretch);

    game->max_view_x = (org_width * level_scaled - rect->right);

    float stretched_width = (float)rect->right * level_stretch;

    game->rect_width = (float)rect->right;

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
                                    (int)((-game->view_x) + game->max_view_x + gap), 0, scaled_width, rect->bottom,
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




   red_black_tree  rbTree;

 // Insert nodes
    rbTree.insert(0,L"Press Enter to Start Now");
    rbTree.insert( 1,L"Press Enter to Resume");
    rbTree.insert( 2,L"PAUSED");
    rbTree.insert(  3,L"YOU WIN!!!");
    rbTree.insert(  4,L"YOU DIED");
    rbTree.insert( 5 ,L"READY?");
    rbTree.insert(  6,L"STEADY??");
    rbTree.insert(  7,L"GO FIGHT!!");

    // Keys to search
    vector<int> keysToSearch;
    for (int i = 0; i < 8; i++) {
        keysToSearch.emplace_back(i);
    }


vector<const wchar_t*> gameState;


for(const auto i : keysToSearch){
    const wchar_t * j = rbTree.search(i);
 gameState.emplace_back(j);
}

    if (state == GAME_STATE_MENU) {
        // draw the moving background
        game_draw_level(hdc, rect);
        if (game->time_text_blink < 50 ) {
            swprintf(buf, 256,gameState[0]);
            DrawTextW(hdc, buf, (int)wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
                swprintf(buf, 256, gameState[1]);
                DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }

            rcText.top -= 50;
            rcText.bottom -= 50;

            SelectObject(hdc, game->font_big);
            swprintf(buf, 256, gameState[2]);
            DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        } else if(state == GAME_STATE_WIN) {
            if (game->time_text_blink < 50 ) {
                swprintf(buf, 256, gameState[3]);
                DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        } else if(state == GAME_STATE_OVER) {
            if (game->time_text_blink < 50 ) {
                swprintf(buf, 256, gameState[4]);
                DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        } else if (state == GAME_STATE_INTRO) {
            if (game->intro_time > 200) {
                swprintf(buf, 256, gameState[5]);
                DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            } else if (game->intro_time > 100) {
                swprintf(buf, 256, gameState[6]);
                DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            } else {
                swprintf(buf, 256, gameState[7]);
                if (game->time_text_blink < 50 ) {
                    DrawTextW(hdc, buf, wcslen(buf), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
            }
        }
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
                if (key == VK_SPACE && player_can_attack) {
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
    static POINTS lastMousePos;

    switch (message) {
        case WM_KEYDOWN:
            game_input(static_cast<signed int>(wParam), 1);
            break;

        case WM_KEYUP:
            game_input(static_cast<signed int>(wParam), 0);
            break;

        case WM_ERASEBKGND:
            return 1;

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

            // SetStretchBltMode(hdcBuffer, HALFTONE);

            game_draw(hdcBuffer, &rcClient);

            BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hdcBuffer, 0, 0, SRCCOPY);

            DeleteDC(hdcBuffer);
            DeleteObject(page_bgk_brush);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_LBUTTONDOWN:
            // Save the initial mouse position when left mouse button is pressed
            lastMousePos = MAKEPOINTS(lParam);
            SetCapture(hwnd);  // Capture mouse input
            break;

        case WM_LBUTTONUP:
            ReleaseCapture();  // Release mouse input capture
            break;

        case WM_MOUSEMOVE:
            // Move the window based on the difference in mouse position
            if (wParam & MK_LBUTTON) {
                POINTS currentMousePos = MAKEPOINTS(lParam);
                int deltaX = currentMousePos.x - lastMousePos.x;
                int deltaY = currentMousePos.y - lastMousePos.y;
                SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOZORDER);
                SetWindowPos(hwnd, nullptr, deltaX, deltaY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                lastMousePos = currentMousePos;
            }
            break;

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
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                return;  // exit the loop if WM_QUIT is received

        }


        QueryPerformanceCounter(&end_counter);

        seconds_per_frame = (((double)(end_counter.QuadPart -
                                       start_counter.QuadPart) * 100.0f) / (double)frequency.QuadPart);
        start_counter = end_counter;

        game_check_input();
        game_update((float)seconds_per_frame);




        // InvalidateRect to trigger window repaint
        InvalidateRect(game->hwnd, nullptr, 1);
    }
}







int main() {


    // Create the first window
    RenderWindow window(VideoMode(1600, 800), "Cadillacs and Dinosaurs Game",Style::Close|Style::Titlebar);


    Image icon;
    icon.loadFromFile(FileLoad::getImageFile("logo.png"));


    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    // Load the background image for the first window
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(FileLoad::getImageFile("bg.png"))) {
        return 1;
    }
    Sprite backgroundSprite(backgroundTexture);

    // Create a button for the first window
    Font font;
    if (!font.loadFromFile(FileLoad::getFontFile("RobotoCondensed-Italic.ttf"))) {
        return 1;
    }

bool audioOn = false; 
    Text newGameButton("New Game", font, 30),audioButton("Audio:OFF",font,30);
    newGameButton.setPosition(700, 300);
    newGameButton.setFillColor(Color::White);
    newGameButton.setStyle(Text::Style::Bold);
    audioButton.setPosition(700, 400);
    audioButton.setFillColor(Color::White);
    audioButton.setStyle(Text::Style::Bold);
    RectangleShape buttonBackground(Vector2f(newGameButton.getGlobalBounds().width + 20, newGameButton.getGlobalBounds().height + 30)),
            buttonBackground1(Vector2f(audioButton.getGlobalBounds().width + 20, audioButton.getGlobalBounds().height + 30));
    buttonBackground.setPosition(newGameButton.getPosition().x - 10, newGameButton.getPosition().y - 5);
    buttonBackground.setFillColor(Color::Blue);
    buttonBackground1.setPosition(audioButton.getPosition().x - 10, audioButton.getPosition().y - 5);
    buttonBackground1.setFillColor(Color::Green);

    // Set the positions of the images
    float scaleFactorX1 = static_cast<float>(window.getSize().x) / (float) backgroundTexture.getSize().x;
    float scaleFactorY1 = static_cast<float>(window.getSize().y) / (float) backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleFactorX1, scaleFactorY1);
    Event event{};
    bool winClose = true;
    Music music, music1;
    music.openFromFile(FileLoad::getAudioFile("gameRun.ogg"));
    music1.openFromFile(FileLoad::getAudioFile("game1.mp3"));

    music.setLoop(true);
    music1.setLoop(true);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                winClose = false;
            }

            if (event.type == Event::MouseButtonReleased) {
                // Check if the mouse click is inside the new game button
                if (newGameButton.getGlobalBounds().contains((float)event.mouseButton.x, (float)event.mouseButton.y)) {
                    window.close();
                    break;
                }
                if (audioButton.getGlobalBounds().contains((float)event.mouseButton.x, (float)event.mouseButton.y)) {
                    audioOn = !audioOn;


                    if (audioOn) {
                        audioButton.setString("Audio: ON");
                        music.play();
                        music1.stop();
                    } else {
                        audioButton.setString("Audio: OFF");
                        music.stop();
                        music1.stop();
                    }
                }
            }
        }

        window.clear();
        // Draw the background
        window.draw(backgroundSprite);
        window.draw(buttonBackground);
        // Draw the new game button
        window.draw(newGameButton);
        window.draw(buttonBackground1);
        window.draw(audioButton);
        window.display();
    }

    if (winClose) {


RenderWindow wind(VideoMode(1600, 860), "Level Selection", Style::Close | Style::Titlebar);
wind.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
// Background
Texture backgroundTexture;
if (!backgroundTexture.loadFromFile(FileLoad::getImageFile("bg2.png"))) {
    cerr << "Failed to load background image" << endl;
    return EXIT_FAILURE;
}

Sprite background(backgroundTexture);
background.setScale(wind.getSize().x / background.getLocalBounds().width,
                    wind.getSize().y / background.getLocalBounds().height);

// Buttons
RectangleShape buttonShape(Vector2f(200, 100));
buttonShape.setFillColor(Color::Magenta);
buttonShape.setPosition(wind.getSize().x / 2 - 100, wind.getSize().y / 2 - 50);
LevelGraph levelGraph;

levelGraph.addLevel("Level 1");
levelGraph.addLevel("Level 2");
levelGraph.addLevel("Level 3");

// Add connections between levels
levelGraph.addConnection("Level 1", "Level 2");
levelGraph.addConnection("Level 2", "Level 3");
vector<string> visitedLevels = levelGraph.traverseSequentially("Level 1");

Text buttonText;
buttonText.setFont(font);
buttonText.setString(visitedLevels[0]);
buttonText.setCharacterSize(20);
buttonText.setFillColor(Color::White);
FloatRect textBounds = buttonText.getLocalBounds();
buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                    textBounds.top + textBounds.height / 2.0f);
buttonText.setPosition(Vector2f(wind.getSize().x / 2, wind.getSize().y / 2 - 25));

RectangleShape buttonShape2(Vector2f(200, 100));
buttonShape2.setFillColor(Color::Red);
buttonShape2.setPosition(wind.getSize().x / 2 - 100, wind.getSize().y / 2 + 75);

Text buttonText2;
buttonText2.setFont(font);
buttonText2.setString(visitedLevels[1]);
buttonText2.setCharacterSize(20);
buttonText2.setFillColor(Color::White);
FloatRect textBounds2 = buttonText2.getLocalBounds();
buttonText2.setOrigin(textBounds2.left + textBounds2.width / 2.0f,
                     textBounds2.top + textBounds2.height / 2.0f);
buttonText2.setPosition(Vector2f(wind.getSize().x / 2, wind.getSize().y / 2 + 100));

RectangleShape buttonShape3(Vector2f(200, 100));
buttonShape3.setFillColor(Color::Blue);
buttonShape3.setPosition(wind.getSize().x / 2 - 100, wind.getSize().y / 2 + 200);

Text buttonText3;
buttonText3.setFont(font);
buttonText3.setString(visitedLevels[2]);
buttonText3.setCharacterSize(20);
buttonText3.setFillColor(Color::White);
FloatRect textBounds3 = buttonText3.getLocalBounds();
buttonText3.setOrigin(textBounds3.left + textBounds3.width / 2.0f,
                     textBounds3.top + textBounds3.height / 2.0f);
buttonText3.setPosition(Vector2f(wind.getSize().x / 2, wind.getSize().y / 2 + 225));

bool winClose1 = true;

while (wind.isOpen()) {
    Event event{};
    while (wind.pollEvent(event)) {
        switch (event.type) {
            case Event::Closed:
                wind.close();
                winClose1 = false;
                break;
            case Event::MouseButtonPressed:
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = wind.mapPixelToCoords(Mouse::getPosition(wind));
                    if (buttonShape.getGlobalBounds().contains(mousePos)) {
                        wind.close();
                         speedChange(2.7,2.8);
                        
                
                    } else if (buttonShape2.getGlobalBounds().contains(mousePos)) {
                        wind.close();
                        speedChange(2.6,2.7);
                  
                    } else if (buttonShape3.getGlobalBounds().contains(mousePos)) {
                        wind.close();
                        speedChange(2.5,2.6);
                       
                     
                    }
                }
                break;
            default:
                break;
        }
    }

    wind.clear();

    // Draw background
    wind.draw(background);

    // Draw buttons
    wind.draw(buttonShape);
    wind.draw(buttonText);
    wind.draw(buttonShape2);
    wind.draw(buttonText2);
    wind.draw(buttonShape3);
    wind.draw(buttonText3);

    wind.display();
}


// Create the third window
if(winClose1){
        RenderWindow window1(VideoMode(1600, 860), "Player Selection", Style::Close |Style::Titlebar);
        window1.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        // Load the background image for the second window
        Image image;
        image.loadFromFile(FileLoad::getImageFile("bg1.png"));
        Texture backgroundTexture1;
        backgroundTexture1.loadFromImage(image);
        Sprite backgroundSprite1(backgroundTexture1);

        Stack<string>filePaths;
        for(int i = 4 ; i>=1 ;i--)
        {
            string file("image"),ex(".png");
            string filePath = (file + to_string(i)) .append(ex);
            filePaths.push(filePath);

        }

        // Create vector of textures
        vector<Texture> ts;

        Texture texture;

        // Load textures and create sprites
        while (!filePaths.isEmpty()) {
            string currentFilePath = filePaths.peek();
            string fullFilePath = FileLoad::getImageFile(currentFilePath);



            if (texture.loadFromFile(fullFilePath)) {
                ts.emplace_back(texture);

            } else {
                cerr << "Failed to load texture from file: " << fullFilePath << endl;
            }

                 filePaths.pop();
        }


    // Set the positions of the bg-images
        float scaleFactorX = static_cast<float>(window1.getSize().x) / (float) backgroundTexture1.getSize().x;
        float scaleFactorY = static_cast<float>(window1.getSize().y) / (float) backgroundTexture1.getSize().y;
        backgroundSprite1.setScale(scaleFactorX, scaleFactorY);


        //create vector of sprites
        vector<Sprite>s;
        for(auto & t : ts)
        {
            Sprite sprite(t);
            s.emplace_back(sprite);
        }


        // Set specific positions for each sprite
        s[0].setPosition(150 * scaleFactorX, 50 * scaleFactorY);
        s[1].setPosition(250 * scaleFactorX, 50 * scaleFactorY);
        s[2].setPosition(350 * scaleFactorX, 50 * scaleFactorY);
        s[3].setPosition(450 * scaleFactorX, 50 * scaleFactorY);

        // Create text for the button and selection message
        Text selectionText("Only Player Jack is currently available", font, 20);
        selectionText.setPosition(50, 400);
        selectionText.setCharacterSize(40);
        selectionText.setFillColor(Color(255, 0, 0));
        selectionText.setStyle(Text::Style::Bold);
        // Calculate the center position for the text
        float centerX = static_cast<float>(window1.getSize().x) / 2.0f - selectionText.getLocalBounds().width / 2.0f;
        float centerY = 400.0f; // Keep the vertical position fixed at 400
        Clock clock;
        float scrollSpeed = 100.0f; // Duration for one animation cycle in seconds
        float returnSpeed = 50.0f; // Adjust the return speed as needed
        bool isReturning = false;

        // Set the position of the text
        selectionText.setPosition(centerX, centerY);
        String s1 = "Player Jack is selected";
        bool isGKeyPressed = false;
        SoundBuffer buffer;
        buffer.loadFromFile(FileLoad::getAudioFile("start.wav"));
        Sound sound;
        sound.setBuffer(buffer);


        while (window1.isOpen()) {
            while (window1.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window1.close();

                }

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::G) {
                        selectionText.setString(s1);
                        s[0].setColor(Color(0,0,255));
                        isGKeyPressed = true;
                        if(audioOn)
                            sound.play();

                    }

                    if (isGKeyPressed && event.key.code == Keyboard::U) {
                        window1.close();
                        music.stop();
                             if (game_init()) {

                             game_run();
                             game_delete();
                             }


                    }

                }
            }

            if(audioOn)
                music1.play();



            float timeElapsed = clock.restart().asSeconds();
            if (!isReturning) {
                // Scroll from right to left
                float offsetX = scrollSpeed * timeElapsed;
                selectionText.move(-offsetX, 0);

                // Check if the text has moved entirely off the window
                if (selectionText.getPosition().x + selectionText.getGlobalBounds().width < 0) {
                    isReturning = true;
                }
            } else {
                // Return to the original position
                float offsetX = returnSpeed * timeElapsed;
                selectionText.move(offsetX, 0);

                // Check if the text has returned to the original position
                if (selectionText.getPosition().x <= (float)window.getSize().x) {
                    selectionText.setPosition((float)window.getSize().x, 400);
                    isReturning = false;
                }
            }


            window1.clear();

            // Draw the background
            window1.draw(backgroundSprite1);
            //Draw the sprite
            for (auto& sp : s) {
                window1.draw(sp);
            }
            // Draw the selection message
            window1.draw(selectionText);
            window1.display();
        }

    }

    }

    return 0;

}
