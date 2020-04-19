#pragma once
#include "ImageData.hpp"

#define CHANGES_BUFFER 10
#define MAX_COMMANG_LENGTH 15

const int initialCapacity = 2;

class Session
{
public:
    Session();
    Session(char* imageNames);
    Session(const Session& other);
    Session& operator= (const Session other);
    ~Session();

    static unsigned int fSize;
    int fSessionID;

    void addImage(char* name);
    char** fChangesMade; // DOVURSHI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

private:

    unsigned int fCapacity;
    ImageData* fImages;

    void copyFunc(const Session& other);
    void delMem();
    void changesInitializer();

    void resizeSession();
};

