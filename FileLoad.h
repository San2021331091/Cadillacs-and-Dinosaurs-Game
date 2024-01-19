#ifndef CADILLACS_AND_DINOSAURS_GAME_FILELOAD_H
#define CADILLACS_AND_DINOSAURS_GAME_FILELOAD_H

#include "Game.h"




class FileLoad {

public:
    static string getImageFile(const string &path);

    static string getFontFile(const string &path);

    static string getAudioFile(const string &path);

    static wchar_t *getImageFile1(const string &path);

};


#endif
