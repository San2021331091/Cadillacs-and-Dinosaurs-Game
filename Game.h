#ifndef CADILLACS_AND_DINOSAUR_GAME_GAME_H

#define CADILLACS_AND_DINOSAUR_GAME_GAME_H


#include <bits/stdc++.h>
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


class vector3d_t{

public:

    float x,y,z;


};



class animation_t{

public:


    string name; //using string class
    int frame_start;
    int frame_end;
    float speed;
    int play_once;
    int reverse_loop;
    int flag;
    int animation_ended;






};

class sprite_t{

public:


    string file_name; //using string class
    Image image;
    Color color_mask;
    int width;
    int height;
    float scale;
    int draw_top;
    int frames_per_row;
    int frame;
    int frame_x;
    int frame_y;
    bool flag;
    vector<animation_t>animations;
    int animation_max;
    int animation_count;
    int animation_current;
    double animation_time;




};

class game_object_t{

public:

    vector3d_t position;



};

#define BBOX_MAIN 0
#define BBOX_HEAD 1
#define BBOX_CHEST 2
#define BBOX_FIST 3
#define BBOX_LEGS 4


class character_t{

public:

    string name;
    vector3d_t position,velocity;
    int on_ground;
    int direction;
    int prev_state;
    int state;
    int state_on_enter;
    vector<sprite_t>sprite;
    float health;
    FloatRect * hit_boxes;
    int hit_box_count;
    int visible;

};

#define ENEMY_TYPE_FERRIS 1
#define ENEMY_TYPE_GNEISS 2
#define ENEMY_TYPE_BUTCHER 3


class enemy_t : public character_t{

public:

    float movement_speed;
    double animation_time;
    float hit_reset,hit_recover_time,time_to_attack;
    int being_hit,engaging,marked_for_delete;
    vector<animation_t>walk;
    float walk_time;
    int walk_target_idx;
    float target_x,target_z,death_blink_time;
    int type;
    float damage_takes,hit_streak;



};

class player_t : public character_t{

public:

    float movement_speed, hit_recover_time;
    int score;
    float hit_reset,hit_streak;
    vector<animation_t>anim_upper_cut;


};


class draw_t : public vector3d_t{

public:

    Image image;
    int flip;
    Color color_mask;
    int width;
    int height;
    int src_x;
    int src_y;
    int src_width;
    int src_height;
    int draw_top;

};

class effect_t : public vector3d_t{

public:


    vector<sprite_t>sprite;
    int draw_punch;
    animation_t *anim;



};

class game_t{

public:


    RenderWindow window;
    Image dbl_buffer;
    string title;
    int height,width,top_margin;
    Font font,font_big;
    double time_elapsed;
    float view_x,view_x_far,rect_width,ground_y,gravity;
    int state;
    effect_t fx_punch,fx_blood;
    string keyboard_state;
    vector<player_t>player;
    vector<enemy_t>enemies;
    int enemy_max,enemy_count;
    Texture img_bg,img_bg_far,img_avatar_player,img_text;
    vector<Texture> img_level_layers;  //using vector class provided by STL
    vector<sprite_t>level_fire;
    float fx_smack_x,fx_smack_y,fx_smack_time;
    draw_t *draw_list;
    int draw_max;
    int draw_count;
    float max_z;
    float min_z;
    float max_view_x;
    float time_text_blink;
    float intro_time;
    int spawn_trigger;



};



extern game_t *game;

void graphics_clear();
void graphics_draw(draw_t * draw);
void graphics_draw_rect( RenderWindow & window, FloatRect &rect, Color &color);
void graphics_draw_surface(RenderWindow & window, Image &image, int x, int y, int w, int h,
                           int src_x, int src_y, int src_w, int src_h, int flip, Color &key);
void graphics_draw_lifeBar(RenderWindow &window, int x, int y, int height, int width, float life);
void graphics_swap_buffer(RenderWindow &window,FloatRect &rect);
enemy_t * enemy_spawn(int x, int y, int type);
void enemy_draw(struct enemy_t* enemy,RenderWindow &window);
void enemy_update(struct enemy_t* enemy, int index, float dt);
void enemy_cleanup(struct enemy_t* enemy);


#endif






