#pragma once

#include "ImageData.hpp"

#define CHANGES_BUFFER 10
#define MAX_COMMAND_LENGTH 15

const int initialCapacity = 2;

class Session
{
public:
    Session();

    //Session(char* imageNames);
    Session(const Session &other);

    Session &operator=(const Session other);

    ~Session();

    bool addImage(char *name);

    void setOpen();
    void setClose();

    void setSessionID(int newID);

private:
    static unsigned int fSize;
    int fSessionID;
    char **fChangesMade;
    unsigned short int fNumberOfChanges;
    unsigned int fCapacity;
    ImageData *fImages;
    bool fSessionIsOpen;


    void copyFunc(const Session &other);

    void delMem();

    void changesInitializer();

    void resizeSession();
};

