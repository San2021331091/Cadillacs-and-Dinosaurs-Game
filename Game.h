#ifndef CADILLACS_AND_DINOSAURS_GAME_GAME_H
#define CADILLACS_AND_DINOSAURS_GAME_GAME_H

#include<bits/stdc++.h>
#include <windows.h>
#include <winuser.h>
#include<SFML/Graphics.hpp>

using namespace std;
using namespace sf;



#define CHARACTER_STATE_IDLE 1
#define CHARACTER_STATE_WALKING 2
#define CHARACTER_STATE_RUNNING 3
#define CHARACTER_STATE_PUNCH_PRE 4
#define CHARACTER_STATE_FIGHT 5
#define CHARACTER_STATE_HIT 6
#define CHARACTER_STATE_HIT_FALL 7
#define CHARACTER_STATE_MOVE 8
#define CHARACTER_STATE_JUMP 9
#define CHARACTER_STATE_RANGE_ATTACK 10
#define CHARACTER_STATE_WIN 11

#define CHARACTER_DIRECTION_RIGHT 1
#define CHARACTER_DIRECTION_LEFT -1

#define GAME_STATE_MENU 0
#define GAME_STATE_INTRO 1
#define GAME_STATE_CUTSCENE 2
#define GAME_STATE_PLAYING 3
#define GAME_STATE_PAUSED 4
#define GAME_STATE_OVER 5
#define GAME_STATE_WIN 6



class vector3d_t {
public:
    float x, y, z;
};


class animation_t {
public:
    wchar_t name[256];
    int frame_start;
    int frame_end;
    float speed;
    int play_once;
    int reverse_loop;
    int flag;
    int animation_ended;
};


class sprite_t {
public:
    wchar_t file_name[256];
    HBITMAP hbitmap;
    COLORREF color_mask;
    int width;
    int height;
    float scale;
    int draw_top;
    int frames_per_row;
    int frame;
    int frame_x;
    int frame_y;
    int flag;
    animation_t* animations;
    int animation_max;
    int animation_count;
    int animation_current;
    double animation_time;
};




class game_object_t {
public:
    vector3d_t position;
};


#define BBOX_MAIN 0
#define BBOX_HEAD 1
#define BBOX_CHEST 2
#define BBOX_FIST 3
#define BBOX_LEGS 4

class character_t {
public:
    wchar_t name[256];
     vector3d_t position;
     vector3d_t velocity;
    int on_ground;
    int direction;
    int prev_state;
    int state;
    int state_on_enter;
    sprite_t* sprite;
    float health;
    RECT* hit_boxes;
    int hit_box_count;
    int visible;
};

#define ENEMY_TYPE_FERRIS 1
#define ENEMY_TYPE_GNEISS 2
#define ENEMY_TYPE_BUTCHER 3
#define ENEMY_TYPE_DINOSAUR 4

class enemy_t {
public:
    character_t base;
    float movement_speed;
    double animation_time;
    float hit_reset;
    float hit_recover_time;
    float time_to_attack;
    int being_hit;
    int engaging;
    int marked_for_delete;
    animation_t* walk;
    float walk_time;
    int walk_target_idx;
    float target_x;
    float target_z;
    float death_blink_time;
    int type;
    float damage_takes;
    float hit_streak;
};



class player_t {
public:
    character_t base;
    float movement_speed;
    float hit_recover_time;
    int score;
    float hit_reset;
    float hit_streak;
    animation_t* anim_upper_cut;
};

class draw_t {
public:
    HBITMAP hbitmap;
    int flip;
    COLORREF color_mask;
    vector3d_t position;
    int width;
    int height;
    int src_x;
    int src_y;
    int src_width;
    int src_height;
    int draw_top;
};


class effect_t {
public:
    sprite_t* sprite;
    vector3d_t pos;
    int draw_punch;
    animation_t* anim;
};

class game_t {
public:
    HWND hwnd;
    wchar_t title[256];
    int height;
    int width;
    int top_margin;
    HBITMAP dbl_buffer;
    HFONT font;
    HFONT font_big;
    double time_elapsed;

    float view_x;
    float view_x_far;

    float rect_width;

    float ground_y;
    float gravity;

    int state;
    char keyboard_state[256];

    player_t* player;

    enemy_t* enemies;
    int enemy_max;
    int enemy_count;

    HGDIOBJ bmp_bg;
    HGDIOBJ bmp_bg_far;
    vector<HGDIOBJ>bmp_level_layers;

    sprite_t* level_fire;

    HGDIOBJ bmp_avatar_player;

    effect_t fx_punch;
    effect_t fx_blood;
    HGDIOBJ bmp_text;
    float fx_smack_x;
    float fx_smack_y;
    float fx_smack_time;

    // things to draw on screen
    draw_t* draw_list;
    int draw_max;
    int draw_count;

    float max_z;
    float min_z;
    float max_view_x;
    float time_text_blink;

    float intro_time;
    int spawn_trigger;
};



extern game_t* game;


#endif

