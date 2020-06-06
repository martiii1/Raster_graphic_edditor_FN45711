#include "Session.hpp"
#include "ImageData.hpp"
#include <fstream>
#include <cstring>


void Session::copyFunc(const Session &other)
{
    fImages = new(std::nothrow) ImageData[other.fSize];
    fImagesPrevious = new(std::nothrow) ImageData[other.fSize];
    fChangesMade = new(std::nothrow) char *[other.fNumberOfChangesCapacity];

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
        fImagesPrevious[i] = other.fImages[i];
    }

    fSize = other.fSize;
    fSessionID = other.fSessionID;
    fNumberOfChanges = other.fNumberOfChanges;
    fNumberOfChangesCapacity = other.fNumberOfChangesCapacity;
    fCapacity = other.fCapacity;
    fSessionIsOpen = other.fSessionIsOpen;
}


void Session::delMem()
{
    for (unsigned int i = 0; i < fSize; i++)
    {
        fImages[i].~ImageData();
        fImagesPrevious[i].~ImageData();
    }

    for (int i = 0; i < fNumberOfChanges; i++)
        delete[] fChangesMade[i];

    delete[] fChangesMade;
    delete[] fImages;
    delete[] fImagesPrevious;
}


void Session::resizeSession()
{
    ImageData *tempImages = new ImageData[fCapacity * 2];
    ImageData *tempImagesPrevious = new ImageData[fCapacity * 2];

    fCapacity = fCapacity * 2;

    for (unsigned int i = 0; i < fSize; i++)
    {
        tempImages[i] = fImages[i];
        tempImagesPrevious[i] = fImagesPrevious[i];

        fImages[i].delImage();
        fImagesPrevious[i].delImage();
    }

    fImages = tempImages;
    fImagesPrevious = tempImagesPrevious;
}

void Session::addImage(char *name)
{
    if (fImages == nullptr)
        fImages = new ImageData[fCapacity];

    if (fImagesPrevious == nullptr)
        fImagesPrevious = new ImageData[fCapacity];

    if (fSize == fCapacity)
        resizeSession();


    fImages[fSize].loadImage(name);

    fImagesPrevious[fSize] = fImages[fSize];


    std::cout << fImages[fSize].getFileName() << " successfully loaded,  ";
    fSize++;
}


Session::Session()
{
    fSize = 0;
    fSessionID = -1;
    fChangesMade = nullptr;
    fNumberOfChanges = 0;
    fNumberOfChangesCapacity = 0;
    fCapacity = initialCapacity;
    fImages = nullptr;
    fImagesPrevious = nullptr;
    fSessionIsOpen = false;
}


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
    delete[] fImages;
    delete[] fImagesPrevious;
}

void Session::setSessionID(int newID)
{
    fSessionID = newID;
}

void Session::printSessionInfo() const
{
    std::cout << "Session with ID: " << fSessionID << std::endl;
    std::cout << "The session contains the following images: " << std::endl;

    for (int i = 0; i < fSize; i++)
    {
        std::cout << fImages[i].getFileName() << " ," << std::endl;
    }

    std::cout << std::endl;

    std::cout << "The following changes are made so far: " << std::endl;
    for (int i = 0; i < fNumberOfChanges; i++)
    {
        std::cout << fChangesMade[i] << std::endl;
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
    for (int i = 0; i < fSize; i++)
    {
        fImagesPrevious[i] = fImages[i];
        fImages[i].rotateImageLeft();
    }
    addNewChange("Images rotated left");
    std::cout << "All images rotated left. \n";
}

void Session::saveImages()
{


    for (int i = 0; i < fSize; i++)
    {
        fImages[i].saveImageToFile();
    }

}

void Session::rotateSessionRight()
{
    for (int i = 0; i < fSize; i++)
    {
        fImagesPrevious[i] = fImages[i];
        fImages[i].rotateImageRight();
    }
    addNewChange("Images rotated right");
    std::cout << "All images rotated right. \n";
}

void Session::saveImagesAs()
{
    for (int i = 0; i < fSize; i++)
    {
        fImages[i].saveAsImageToFile();
    }
}

void Session::grayscaleSession()
{
    for (int i = 0; i < fSize; i++)
    {
        fImagesPrevious[i] = fImages[i];
        fImages[i].makeImageGrayscale();
    }
    addNewChange("Images converted to grayscale");
    std::cout << "All images converted to grayscale. \n";
}

void Session::monochromeSession()
{
    for (int i = 0; i < fSize; i++)
    {
        fImagesPrevious[i] = fImages[i];
        fImages[i].makeImageMonochrome();
    }
    addNewChange("Images converted to monochrome");
    std::cout << "All images converted to monochrome. \n";
}

void Session::addCollage(const char *image1Name, const char *image2Name, const char *outImageName, bool isVertical)
{
    int image1ID = -1;
    int image2ID = -1;
    for (int i = 0; i < fSize; i++)
    {
        if (strcmp(fImages[i].getFileName(), image1Name) == 0)
            image1ID = i;

        if (strcmp(fImages[i].getFileName(), image2Name) == 0)
            image2ID = i;
    }

    if (image1ID == -1 || image2ID == -1)
        throw std::exception("Images not found! \n");
    ImageData tempImage;
    tempImage.createCollage(fImages[image1ID], fImages[image2ID], outImageName, isVertical);

    fImages[fSize] = tempImage;
    std::cout << "Collage " << tempImage.getFileName() << " successfully created and added to current session.\n";
    fSize++;
}

void Session::setNumberOfChanges(unsigned short newNumber)
{
    fNumberOfChanges = newNumber;
}

unsigned short int Session::getNumberOfChanges()
{
    return fNumberOfChanges;
}

void Session::addNewChange(const char *change)
{
    if (fChangesMade == nullptr)
        fChangesMade = new char *[fNumberOfChangesCapacity];

    if (fNumberOfChanges <= fNumberOfChangesCapacity - 1)
    {
        char **fChangesMadeTemp = new char *[fNumberOfChangesCapacity * 2];
        for (int i = 0; i < fNumberOfChanges; i++)
        {
            fChangesMadeTemp[i] = new char[strlen(fChangesMade[i]) + 1];
            strcpy(fChangesMadeTemp[i], fChangesMade[i]);
            delete[] fChangesMade[i];
        }
        delete[] fChangesMade;

        fChangesMade = fChangesMadeTemp;

        fNumberOfChangesCapacity = fNumberOfChanges * 2;
    }

    fChangesMade[fNumberOfChanges] = new char[strlen(change) + 1];
    strcpy(fChangesMade[fNumberOfChanges], change);

    fNumberOfChanges++;
}

void Session::removeLastChange()
{
    delete[] fChangesMade[fNumberOfChanges - 1];
    fNumberOfChanges--;
}

void Session::undoLastChange()
{
    for (int i = 0; i < fSize; i++)
    {
        fImages[i] = fImagesPrevious[i];
    }

    std::cout << "The last change ( " << fChangesMade[fNumberOfChanges-1] << " ) is reversed! \n";
    removeLastChange();
}

void Session::negativeSession()
{
    for (int i = 0; i < fSize; i++)
    {
        fImagesPrevious[i] = fImages[i];
        fImages[i].makeImageNegative();
    }
    addNewChange("Images converted to negatives");
    std::cout << "All images converted to negatives. \n";

}
