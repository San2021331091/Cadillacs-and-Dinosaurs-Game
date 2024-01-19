#include "FileLoad.h"
#include "LinkedList.h"


string imageFilePath = "E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Images/";
string fontFilePath = "E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/Font/";
string audioFilePath = "E:/ClionProjects/Cadillacs_and_Dinosaurs_Game/sound/";


string FileLoad::getImageFile(const string &path){

    LinkedList::append(imageFilePath+path);
    int lastIndex = LinkedList::getSize() - 1;

    string imagePathAtIndex = LinkedList::getFilePathAt(lastIndex);
    return imagePathAtIndex;

}

string FileLoad::getFontFile(const string &path){

    LinkedList::append(fontFilePath+path);
    int lastIn = LinkedList::getSize() - 1;

    string FontPathAtIndex = LinkedList::getFilePathAt(lastIn);
    return FontPathAtIndex;

}
string FileLoad :: getAudioFile(const string &path){

    LinkedList::append(audioFilePath+path);
    int lastIn = LinkedList::getSize() - 1;

    string audioPathAtIndex = LinkedList::getFilePathAt(lastIn);
    return audioPathAtIndex;

}



wstring concatenateWideString(const string& Str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    wstring wideStr = converter.from_bytes(Str);

    return wideStr;
}

wchar_t*  FileLoad :: getImageFile1(const string& path) {
    wstring ImageFilePath = concatenateWideString(imageFilePath);
    wstring iPath = ImageFilePath + concatenateWideString(path);

    auto* result = new wchar_t[iPath.length() + 1];
    wcsncpy(result, iPath.c_str(), iPath.length());
    result[iPath.length()] = L'\0'; // Ensure null-termination

    return result;
}

