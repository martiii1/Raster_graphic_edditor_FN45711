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

    void addImage(char *name);

    void setSessionID(int newID);

    void setSize(unsigned int size);

    void setNumberOfChanges(unsigned short int newNumber);

    unsigned short int getNumberOfChanges();

    int getSessionID() const;

    unsigned int getSize() const;

    unsigned short int getNumberOfChanges() const;

    void addNewChange(const char* change);

    void removeLastChange();

    void printSessionInfo() const;

    void rotateSessionLeft();

    void rotateSessionRight();

    void saveImages();

    void saveImagesAs();

    void grayscaleSession();

    void monochromeSession();

    void undoLastChange();

    //void negativeSession();

    void addCollage(const char *image1Name, const char *image2Name, const char *outImageName, bool isVertical);

private:
    unsigned int fSize;
    int fSessionID;
    unsigned int fCapacity;
    ImageData *fImages;
    ImageData *fImagesPrevious;
    bool fSessionIsOpen;

    char **fChangesMade;
    unsigned short int fNumberOfChanges;
    unsigned short int fNumberOfChangesCapacity;


    void copyFunc(const Session &other);

    void delMem();

    void changesInitializer();

    void resizeSession();
};

