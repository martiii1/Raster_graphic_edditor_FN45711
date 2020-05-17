#include "Session.hpp"
#include "ImageData.hpp"
#include <fstream>
#include <cstring>


void Session::copyFunc(const Session &other)
{
    fImages = new(std::nothrow) ImageData[other.fSize];
    fChangesMade = new(std::nothrow) char *[other.fNumberOfChanges];

    if (fImages == nullptr || fChangesMade == nullptr)
    {
        std::cout << "Not enough memory" << std::endl;
        delMem();
        return;
    }

    for (int i = 0; i < other.fNumberOfChanges; i++)
    {
        fChangesMade[i] = new(std::nothrow) char[strlen(other.fChangesMade[i]) + 1];
        if (fChangesMade[i] == nullptr)
        {
            std::cout << "Not enough memory";
            delMem();
            return;
        }

        strcpy(fChangesMade[i], other.fChangesMade[i]);
    }

    for (unsigned int i = 0; i < other.fSize; i++)
    {
        fImages[i] = other.fImages[i];
    }

    fSize = other.fSize;
    fSessionID = other.fSessionID;
    fNumberOfChanges = other.fNumberOfChanges;
    fCapacity = other.fCapacity;
    fSessionIsOpen = other.fSessionIsOpen;
}


void Session::delMem()
{
    for (unsigned int i = 0; i < fSize; i++)
        fImages[i].~ImageData();

    for (int i = 0; i < fNumberOfChanges; i++)
        delete[] fChangesMade[i];

    delete[] fChangesMade;
    delete[] fImages;
}

void Session::changesInitializer()
{
    fChangesMade = new(std::nothrow) char *[CHANGES_BUFFER];
    if (fChangesMade == nullptr)
    {
        std::cout << "Memory fail(for changes)!" << std::endl;
        return;
    }

    for (int i = 0; i < CHANGES_BUFFER; i++)
        fChangesMade[i] = new(std::nothrow) char[MAX_COMMAND_LENGTH];
    if (fChangesMade == nullptr);
    // TO DO ...........................................................//// TODO

}


void Session::resizeSession()
{
    ImageData *tempImages = new ImageData[fCapacity * 2];

    fCapacity = fCapacity * 2;

    for (unsigned int i = 0; i < fSize; i++)
    {
        tempImages[i] = fImages[i];
        fImages[i].delImage();
    }

    fImages = tempImages;
}

void Session::addImage(char *name)
{
    try
    {
        if (fImages == nullptr)
            fImages = new ImageData[fCapacity];

        if (fSize == fCapacity)
        {
            try
            {
                resizeSession();
            }
            catch (...)
            {
                throw;
            }
        }

        try
        {
            fImages[fSize].loadImage(name);
        }
        catch (...)
        {
            throw;
        }

        //if(fImages[fSize].getimagestatst == BROKEN)       TODO!!!!!!
        //return true;***
    }
    catch (...)
    {
        throw;
    }
    std::cout <<fImages[fSize].getFileName()<< " successfully loaded,  ";
    fSize++;
}


Session::Session()
{
    fSize = 0;
    fSessionID = -1;
    fChangesMade = nullptr;
    fNumberOfChanges = 0;
    fCapacity = initialCapacity;
    fImages = nullptr;
    fSessionIsOpen = false;
}

//Session::Session(char* imageNames) //////////////////////////////////////////////////// WTF
//{
//    fCapacity = initialCapacity;
//    fImages = new (std::nothrow) ImageData[fCapacity];
//    fSessionID = -1;
//
//    if (fImages == nullptr)
//    {
//        std::cout << "Error while allocating memory, deleting session! " << std::endl;
//        delMem();
//        return;
//    }
//
//
//    char* token = strtok(imageNames, " ");
//    if (strcmp(token, "load") == 0)
//        token = strtok(NULL, " ");
//
//    while (token != nullptr)
//    {
//        addImage(token);
//        token = strtok(nullptr, " "); /// MAYBE VALIDATE FILE NAMES
//    }
//
//}

Session::Session(const Session &other)
{
    copyFunc(other);
}


Session &Session::operator=(const Session other)
{
    if (this != &other)
    {
        delMem();
        copyFunc(other);
    }

    return *this;
}

Session::~Session()
{
    //std::cout << "Current Session Destruct \n";

    /*if (fImages != nullptr)
        for (unsigned int i = 0; i < fSize; i++) // FIXED!
        {
            fImages[i].~ImageData();
        }*/

    delete[] fImages;
}

void Session::setOpen()
{
    fSessionIsOpen = true;
}

void Session::setClose()
{
    fSessionIsOpen = false;
}

void Session::setSessionID(int newID)
{
    fSessionID = newID;
}

bool Session::isSessionOpen() const
{
    return fSessionIsOpen;
}

void Session::printSessionInfo() const
{
    for(int i=0;i<fSize;i++)
    {
        std::cout << fImages[i].getFileName() << "is in Session " << fSessionID << std::endl;

    }
}

void Session::setSize(unsigned int size)
{
    fSize = size;
}
int Session::getSessionID() const
{
    return fSessionID;
}

unsigned short int Session::getNumberOfChanges() const
{
    return fNumberOfChanges;
}

unsigned int Session::getSize() const
{
    return fSize;
}

void Session::rotateSessionLeft()
{
    for(int i=0;i<fSize;i++)
    {
        fImages[i].rotateImageLeft();
    }
}

void Session::saveImages()
{


    for(int i=0;i<fSize;i++)
    {
       fImages[i].saveImageToFile(); // TODO !
    }

}

void Session::rotateSessionRight()
{
    for(int i=0;i<fSize;i++)
    {
        fImages[i].rotateImageRight();
    }
}

void Session::saveImagesAs()
{
    for(int i=0;i<fSize;i++)
    {
        fImages[i].saveAsImageToFile();
    }
}

void Session::grayscaleSession()
{
    for(int i=0;i<fSize;i++)
    {
        fImages[i].makeImageGrayscale();
    }
}

void Session::monochromeSession()
{
    for(int i=0;i<fSize;i++)
    {
        fImages[i].makeImageMonochrome();
    }
}
