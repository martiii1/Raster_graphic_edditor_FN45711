#include "ImageData.hpp"
#include <fstream>

#define MAX_ROW_SIZE 70


ImageData::ImageData()
{
    fFileName = nullptr;
    fImageFormat = 0;
    fImageWidth = 0;
    fImageHeight = 0;
    fPixelMaxValues = 0;
    //char* fImageComments;
    fImageMatrix = nullptr;
}

ImageData::ImageData(char* FileName)
{
    loadImage(FileName);
}

ImageData::ImageData(const ImageData& other)
{
    copyImage(other);
}

ImageData& ImageData::operator=(const ImageData& other)
{
    if (this != &other)
    {
        delImage();
        copyImage(other);
    }
    return *this;
}

ImageData::~ImageData()
{
    //std::cout << "Image Destruct \n";
    delImage();
}

char* ImageData::getFileName()
{
    char tempName[MAX_FILE_NAME_SIZE];
    strcpy(tempName, fFileName);

    return tempName;
}

void ImageData::copyImage(const ImageData& otherImage)
{
    fImageFormat = otherImage.fImageFormat;
    fImageWidth = otherImage.fImageWidth;
    fImageHeight = otherImage.fImageHeight;
    fPixelMaxValues = otherImage.fPixelMaxValues;

    if (fImageFormat == PPMA)
    {
        fImageMatrix = allocateMatrix(fImageWidth*3, fImageHeight); // width *3 beacuse of RGB

        for (unsigned int i = 0; i < fImageHeight; i++)
            for (unsigned int j = 0; j < fImageWidth*3; j++) // width *3 beacuse of RGB
                fImageMatrix[i][j] = otherImage.fImageMatrix[i][j];
    }
    else
    {

        fImageMatrix = allocateMatrix(fImageWidth, fImageHeight);

        for (unsigned int i = 0; i < fImageHeight; i++)
            for (unsigned int j = 0; j < fImageWidth; j++)
                fImageMatrix[i][j] = otherImage.fImageMatrix[i][j];
    }
}

void ImageData::loadImage(char* FileName)
{
    ImageData();

    fFileName = new(std::nothrow) char[strlen(FileName) + 1];// ///////////////////////////////////// this is nullptr error

    strcpy(fFileName, FileName);

    std::ifstream image(fFileName);

    if (!image.is_open() || !image.good())
    {
        std::cout << "Can't open the file!" << std::endl;
        image.close();
        return;
    }

    switch (getImageFormat(image))
    {
        case PBMA:
            fImageFormat = PBMA;
            readPBMA(image);
            break;
        case PGMA:
            fImageFormat = PGMA;
            readPGMA(image);
            break;
        case PPMA:
            fImageFormat = PPMA;
            readPPMA(image);
            break;
        default:
            std::cout << "Not supported type! " << std::endl;
            fImageFormat = BROKEN;
            image.close();
            delImage();
    }

    image.close();
}

void ImageData::delImage()
{
    if(fImageMatrix != nullptr)
        for (unsigned int i = 0; i < fImageHeight; i++)
        {
            delete[] fImageMatrix[i];
        }

    delete[] fFileName;
    delete[] fImageMatrix;

}

void ImageData::readPBMA(std::ifstream &file)
{
    skipComments(file);

    getDimentions(file);

    fImageMatrix = allocateMatrix(fImageWidth, fImageHeight);

    getPBMApixels(file);

    //std::cout <<fFileName<<"   " << fImageFormat << "   " << fImageWidth << "   " << fImageHeight << "   " << fPixelMaxValues;
}

void ImageData::readPGMA(std::ifstream &file)
{
    skipComments(file);

    getDimentions(file);

    getPixelMaxValues(file);

    fImageMatrix = allocateMatrix(fImageWidth, fImageHeight);

    getPGMApixels(file);
}

void ImageData::readPPMA(std::ifstream& file)
{
    skipComments(file);

    getDimentions(file);

    getPixelMaxValues(file);

    fImageMatrix = allocateMatrix(fImageWidth*3, fImageHeight);

    getPPMApixels(file);
}


void ImageData::getDimentions(std::ifstream &file)
{
    char c;
    file.get(c);
    if (c < '0' || c > '9') // if the new line starts with a number/digit
    {
        std::cout << "Error while reading image dimentions! " << std::endl;
        delImage();
        return;
    }

    file.putback(c);
    file >> fImageWidth >> fImageHeight;
}

void ImageData::getPixelMaxValues(std::ifstream& file)
{
    skipComments(file);

    unsigned int maxPixel;

    file >> maxPixel;

    if(fImageFormat == PPMA)
        if (maxPixel > 255)
        {
            std::cout << "Unknown image format! " << std::endl;
            delImage();
            return;
        }
}

void ImageData::getPBMApixels(std::ifstream &file)
{
    char c;
    unsigned int row = 0, col = 0;
    while (file.good())
    {
        skipComments(file);
        file.get(c);

        if ((c - '0') == 0 || (c - '0') == 1)
        {

            fImageMatrix[row][col] = c - '0';

            std::cout << fImageMatrix[row][col] << "  "; // DELETE COUT

            col++;
            if (col == fImageWidth)
            {
                row++;

                if (row == fImageHeight) // if all data is added
                    break;

                col = 0;
                std::cout << std::endl; // DELETE COUT
            }
        }
        else if (c == ' ' || c == '\n') // Ignores whitespaces
        {
            continue;
        }
        else
        {
            std::cout << "Error while reading image data! " << std::endl;
            delImage();
            return;
        }

    }
}


void ImageData::getPGMApixels(std::ifstream& file)
{

    char c;
    unsigned int pixel;
    unsigned int row = 0, col = 0;
    while (file.good())
    {
        skipComments(file);
        file.get(c);

        if ((c - '0') >= 0 && (c - '0') <= 9)
        {
            file.putback(c);
            file >> pixel;

            fImageMatrix[row][col] = pixel;

            std::cout << fImageMatrix[row][col] << "  "; // DELETE COUT

            col++;
            if (col == fImageWidth)
            {
                row++;

                if (row == fImageHeight) // if all data is added
                    break;

                col = 0;
                std::cout << std::endl; // DELETE COUT
            }
        }
        else if (c == ' ' || c == '\n') // Ignores whitespaces
        {
            continue;
        }
        else
        {
            std::cout << "Error while reading image data! " << std::endl;
            delImage();
            return;
        }

    }
}

void ImageData::getPPMApixels(std::ifstream& file)
{
    char c;
    unsigned int pixel;
    unsigned int row = 0, col = 0;
    while (file.good())
    {
        skipComments(file);
        file.get(c);

        if ((c - '0') >= 0 && (c - '0') <= 9)
        {
            file.putback(c);
            file >> pixel;

            fImageMatrix[row][col] = pixel;

            std::cout << fImageMatrix[row][col] << "  "; // DELETE COUT

            col++;
            if (col == fImageWidth*3)
            {
                row++;

                if (row == fImageHeight) // if all data is added
                    break;

                col = 0;
                std::cout << std::endl; // DELETE COUT
            }
        }
        else if (c == ' ' || c == '\n') // Ignores whitespaces
        {
            continue;
        }
        else
        {
            std::cout << "Error while reading image data! " << std::endl;
            delImage();
            return;
        }

    }
}

size_t ImageData::getImageFormat(std::ifstream& file)
{
    char temp[3];  // for 'P' + NUMBER + \n
    skipComments(file);
    file >> temp;
    size_t tempFormat;

    if (!(temp[0] == 'P' && temp[1] >= '1' && temp[1] <= '3'))  // Validating image format
    {
        std::cout << "Unsupported file type! " << std::endl;
        file.close();
        delImage();
        return -1;
    }

    return(temp[1] - '0');
}

void ImageData::skipComments(std::ifstream& file)
{
    char c;

    do
    {
        file.get(c);

        if (c == '\n')
        {
            continue;
        }
        else if (c == '#')
        {
            file.ignore(MAX_ROW_SIZE, '\n');
        }
        else
        {
            break;
        }

    } while (file.good());

    file.putback(c);
}

int** ImageData::allocateMatrix(unsigned int width, unsigned int height)
{
    int** Matrix = new(std::nothrow) int* [height];
    if (Matrix == nullptr)
    {
        std::cout << "Error while allocating memory!" << std::endl;
        return nullptr;
    }


    for (unsigned int i = 0; i < height; i++)
    {
        Matrix[i] = new(std::nothrow) int[width];

        if (Matrix[i] == nullptr)
        {
            std::cout << "Error while allocating memory!" << std::endl;
            for (int j = i - 1; j >= 0; j--)
            {
                delete[] Matrix[j];
            }
            delete[] Matrix;
            return nullptr;
        }
    }

    return Matrix;
}
