#ifndef CADILLACS_AND_DINOSAUR_GAME_GAME_H

#define CADILLACS_AND_DINOSAUR_GAME_GAME_H


#include <bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>


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
    wstring name; //using wstring class
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

    wstring file_name; //using wstring class
    Image bitmap;
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
    vector<animation_t>animations;  //using vector class provided by STL
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

    wstring name;
    vector3d_t position,velocity;
    int on_ground;
    int direction;
    int prev_state;
    int state;
    int state_on_enter;
    vector<sprite_t> sprite;
    float health;
    vector<FloatRect> hit_boxes; //using FloatRect class for handling 2d rectangles with floating-point precision
    int hit_box_count;
    int visible;

};

#define ENEMY_TYPE_FERRIS 1
#define ENEMY_TYPE_GNEISS 2
#define ENEMY_TYPE_BUTCHER 3









#endif






