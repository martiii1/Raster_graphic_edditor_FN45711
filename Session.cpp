#include "Session.hpp"
#include "ImageData.hpp"
#include <fstream>
#include<cstring>


void Session::copyFunc(const Session& other)
{
    fImages = new(std::nothrow) ImageData[other.fSize];
    if (fImages == nullptr)
    {
        std::cout << "Not enough memory"; //////////////////////////////////////////////MAYBE ?
        return;
    }


    for (unsigned int i = 0; i < other.fSize; i++)
    {
        fImages[i] = other.fImages[i];
    }

    fCapacity = other.fCapacity;
    fSessionID = other.fSessionID;
}


void Session::delMem()
{
    for (unsigned int i = 0; i < fSize; i++)
        fImages[i].~ImageData();

    delete[] fImages;
}

void Session::changesInitializer()
{
    fChangesMade = new(std::nothrow) char* [CHANGES_BUFFER];
    if(fChangesMade == nullptr)
    {
        std::cout << "Memory fail(for changes)!" << std::endl;
        return;
    }

    for (int i = 0; i < CHANGES_BUFFER; i++)
        fChangesMade[i] = new(std::nothrow) char[MAX_COMMANG_LENGTH];
    if (fChangesMade == nullptr);
    // TO DO ...........................................................//// TODO

}


void Session::resizeSession()
{
    ImageData* tempImages = new(std::nothrow) ImageData[fCapacity*2];
    if (tempImages == nullptr)
    {
        std::cout << "Error while allocating memory for bigger session! " << std::endl;
        return;
    }
    fCapacity = fCapacity * 2;

    for (unsigned int i = 0; i < fSize; i++)
    {
        tempImages[i] = fImages[i];
        fImages[i].delImage();
    }

    fImages = tempImages;
}

void Session::addImage(char* name)
{
    if (fSize == fCapacity)
        resizeSession();

    fImages[fSize].loadImage(name);
    //if(fImages[fSize].fImageFormat != BROKEN)//// TODO
    fSize++;
}


Session::CurrentSession() : ImageData()// ???????????????
{

    fCapacity = initialCapacity;
    fImages = nullptr;
    fSessionID = -1;
}

Session::CurrentSession(char* imageNames) //////////////////////////////////////////////////// WTF
{
    fCapacity = initialCapacity;
    fImages = new (std::nothrow) ImageData[fCapacity];
    fSessionID = -1;

    if (fImages == nullptr)
    {
        std::cout << "Error while allocating memory, deleting session! " << std::endl;
        delMem();
        return;
    }


    char* token = strtok(imageNames, " ");
    if (strcmp(token, "load") == 0)
        token = strtok(NULL, " ");

    while (token != NULL)
    {
        addImage(token);
        token = strtok(NULL, " "); /// MAYBE VALIDATE FILE NAMES
    }

}

Session::CurrentSession(const Session& other)
{
    copyFunc(other);
}


Session& Session::operator=(const Session other)
{
    if (this != &other)
    {
        delMem();
        copyFunc(other);
    }

    return *this;
}

Session::~CurrentSession()
{
    //std::cout << "Current Session Destruct \n";

    /*if (fImages != nullptr)
        for (unsigned int i = 0; i < fSize; i++) // FIXED!
        {
            fImages[i].~ImageData();
        }*/
    delete[] fImages;

}
