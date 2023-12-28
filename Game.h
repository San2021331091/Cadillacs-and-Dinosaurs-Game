#ifndef CADILLACS_AND_DINOSAUR_GAME_GAME_H

#define CADILLACS_AND_DINOSAUR_GAME_GAME_H


#include <bits/stdc++.h>
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
    wchar_t name[256];
    int frame_start;
    int frame_end;
    float speed;
    int play_once;
    int reverse_loop;
    int flag;
    int animation_ended;


};















#endif






