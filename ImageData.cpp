#include "ImageData.hpp"
#include <fstream>

#define MAX_ROW_SIZE 70


ImageData::ImageData()
{
    fFileName = nullptr;
    fImageFormat = 0;
    fImageWidth = 0;
    fImageHeight = 0;
    fPixelMaxValue = 0;
    //char* fImageComments;
    fImageMatrix = nullptr;

}

ImageData::ImageData(char *FileName)
{
    loadImage(FileName);
}

ImageData::ImageData(const ImageData &other)
{
    copyImage(other);
}

ImageData &ImageData::operator=(const ImageData &other)
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

char *ImageData::getFileName()
{
    return fFileName;
}

void ImageData::copyImage(const ImageData &otherImage)
{
    fImageFormat = otherImage.fImageFormat;
    fImageWidth = otherImage.fImageWidth;
    fImageHeight = otherImage.fImageHeight;
    fPixelMaxValue = otherImage.fPixelMaxValue;

    fFileName = new(std::nothrow) char[strlen(otherImage.fFileName) + 1];
    if (fFileName == nullptr)
    {
        std::cout << "Not enough memory! " << std::endl;
        delImage();
        return;
    }
    strcpy(fFileName, otherImage.fFileName);

    if (fImageFormat == PPMA)
    {
        fImageMatrix = allocateMatrix(fImageWidth * 3, fImageHeight); // width *3 because of RGB

        for (unsigned int i = 0; i < fImageHeight; i++)
            for (unsigned int j = 0; j < fImageWidth * 3; j++) // width *3 because of RGB
                fImageMatrix[i][j] = otherImage.fImageMatrix[i][j];
    }
    else
    {

        fImageMatrix = allocateMatrix(fImageWidth, fImageHeight);
        if (fImageMatrix == nullptr)
            return;


        for (unsigned int i = 0; i < fImageHeight; i++)
            for (unsigned int j = 0; j < fImageWidth; j++)
                fImageMatrix[i][j] = otherImage.fImageMatrix[i][j];
    }
}

bool ImageData::loadImage(char *FileName)
{

    fFileName = new(std::nothrow) char[strlen(FileName) + 1];
    {
        if (fFileName == nullptr)
        {
            std::cout << "Error while allocating memory! " << std::endl;
            delImage();
            return false;
        }
    }

    strcpy(fFileName, FileName);

    std::ifstream image(fFileName);

    if (!image.is_open() || !image.good())
    {
        std::cout << "Can't open the file!" << std::endl;
        image.close();
        //delImage();
        return false;
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

    if (fImageFormat == BROKEN)
        return false;

    return true;
}

void ImageData::delImage()
{
    if (fImageMatrix != nullptr)
        for (unsigned int i = 0; i < fImageHeight; i++)
        {
            delete[] fImageMatrix[i];
        }

    delete[] fFileName;
    delete[] fImageMatrix;
    fImageFormat = BROKEN;
}

void ImageData::readPBMA(std::ifstream &file)
{
    skipComments(file);

    getDimensions(file);

    fImageMatrix = allocateMatrix(fImageWidth, fImageHeight);

    getPBMApixels(file);

    //std::cout << fFileName << "   " << fImageFormat << "   " << fImageWidth << "   " << fImageHeight << "   "
    //<< fPixelMaxValue; // TODO !
}

void ImageData::readPGMA(std::ifstream &file)
{
    skipComments(file);

    getDimensions(file);

    getPixelMaxValues(file);

    fImageMatrix = allocateMatrix(fImageWidth, fImageHeight);

    getPGMApixels(file);
}

void ImageData::readPPMA(std::ifstream &file)
{
    skipComments(file);

    getDimensions(file);

    getPixelMaxValues(file);

    fImageMatrix = allocateMatrix(fImageWidth * 3, fImageHeight);

    getPPMApixels(file);
}


void ImageData::getDimensions(std::ifstream &file)
{
    char c;
    file.get(c);
    if (c < '0' || c > '9') // if the new line starts with a number/digit
    {
        std::cout << "Error while reading image dimensions! " << std::endl;
        delImage();
        return;
    }

    file.putback(c);
    file >> fImageWidth >> fImageHeight;
}

void ImageData::getPixelMaxValues(std::ifstream &file)
{
    skipComments(file);

    unsigned int maxPixel;

    file >> maxPixel;

    if (fImageFormat == PPMA)
        if (maxPixel > 255)
        {
            std::cout << "Unknown image format! " << std::endl;
            delImage();
            return;
        }

    fPixelMaxValue = maxPixel;
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

            //std::cout << fImageMatrix[row][col] << "  "; // DELETE COUT

            col++;
            if (col == fImageWidth)
            {
                row++;

                if (row == fImageHeight) // if all data is added
                    break;

                col = 0;
                //std::cout << std::endl; // DELETE COUT
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


void ImageData::getPGMApixels(std::ifstream &file)
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

            //std::cout << fImageMatrix[row][col] << "  "; // DELETE COUT

            col++;
            if (col == fImageWidth)
            {
                row++;

                if (row == fImageHeight) // if all data is added
                    break;

                col = 0;
                //std::cout << std::endl; // DELETE COUT
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

void ImageData::getPPMApixels(std::ifstream &file)
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

            //std::cout << fImageMatrix[row][col] << "  "; // DELETE COUT

            col++;
            if (col == fImageWidth * 3)
            {
                row++;

                if (row == fImageHeight) // if all data is added
                    break;

                col = 0;
                //std::cout << std::endl; // DELETE COUT
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

size_t ImageData::getImageFormat(std::ifstream &file)
{
    char temp[3];  // for 'P' + NUMBER + \n
    skipComments(file); // Skips comments if there are any

    file >> temp;

    if (!(temp[0] == 'P' && (temp[1] >= '1' && temp[1] <= '3')))  // Validating image format
    {
        std::cout << "Unsupported file type! " << std::endl;
        file.close();
        delImage();
        return -1;
    }

    return (temp[1] - '0');
}

void ImageData::skipComments(std::ifstream &file)
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

int **ImageData::allocateMatrix(unsigned int width, unsigned int height)
{
    int **Matrix = new(std::nothrow) int *[height];
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

void ImageData::rotateImageLeft()
{
    int **tempNewMatrix;

    unsigned int tempWidth;
    unsigned int tempHeight;


    if (fImageFormat == PPMA)
    {
        tempNewMatrix = allocateMatrix(fImageHeight * 3, fImageWidth);

        tempWidth = fImageWidth;
        tempHeight = fImageHeight;

        unsigned int counterNew = 0;
        unsigned int counterOld = 0;

        for (int i = 0; i < fImageWidth; i++)
        {
            for (int j = 0; j < fImageHeight; j++)
            {
                tempNewMatrix[i][counterNew] = fImageMatrix[j][fImageWidth * 3 - counterOld - 3];
                tempNewMatrix[i][counterNew + 1] = fImageMatrix[j][fImageWidth * 3 - counterOld - 2];
                tempNewMatrix[i][counterNew + 2] = fImageMatrix[j][fImageWidth * 3 - counterOld - 1];
                counterNew += 3;
            }
            counterNew = 0;
            counterOld += 3;
        }
        fImageWidth = tempHeight * 3;
        fImageHeight = tempWidth;
    }
    else
    {
        tempNewMatrix = allocateMatrix(fImageHeight, fImageWidth);

        tempWidth = fImageWidth;
        tempHeight = fImageHeight;

        for (int i = 0; i < fImageWidth; i++)
        {
            for (int j = 0; j < fImageHeight; j++)
            {
                tempNewMatrix[i][j] = fImageMatrix[j][fImageWidth - i - 1]; // -1 because [][] starts at 00
            }
        }

    }

    deleteImageMatrix(fImageWidth, fImageHeight);

    fImageWidth = tempHeight;
    fImageHeight = tempWidth;

    fImageMatrix = tempNewMatrix;

}

void ImageData::deleteImageMatrix(unsigned int width, unsigned int height)
{
    for (int i = 0; i < fImageHeight; i++)
        delete[] fImageMatrix[i];

    delete[] fImageMatrix;
}

unsigned short int ImageData::getImageFormat() const
{
    return fImageFormat;
}

unsigned int ImageData::getImageWidth() const
{
    return fImageWidth;
}

unsigned int ImageData::getImageHeight() const
{
    return fImageHeight;
}

unsigned int ImageData::getPixelMaxValues() const
{
    return fPixelMaxValue;
}

void ImageData::writeMatrixToFile(std::ofstream &file)
{
    unsigned int tempWidth;
    if (fImageFormat == PPMA)
        tempWidth = fImageWidth * 3;
    tempWidth = fImageWidth;

    for (int i = 0; i < fImageHeight; i++)
    {
        for (int j = 0; j < tempWidth; j++)
        {
            if (j >= 35 && j % 35 == 0)
                file << std::endl; // the file must be consisted of lines no longer than 70 characters

            file << fImageMatrix[i][j] << " ";
        }

        file << std::endl;
    }
}

#define TempImageName "tempImageFile.txt"

void ImageData::saveImageToFile()
{
    std::ofstream writefile;
    char tempNewName[MAX_FILE_NAME_SIZE];
    std::strcpy(tempNewName,fFileName);

    writefile.open(TempImageName, std::ofstream::trunc);

    writefile << "P" << fImageFormat << std::endl;
    writefile << fImageWidth << " " << fImageHeight << std::endl;

    writeMatrixToFile(writefile);

    writefile.close();
    remove( tempNewName );
    rename(TempImageName, tempNewName );
}


void ImageData::rotateImageRight()
{
    int **tempNewMatrix;

    unsigned int tempWidth;
    unsigned int tempHeight;


    if (fImageFormat == PPMA)
    {
        tempNewMatrix = allocateMatrix(fImageHeight * 3, fImageWidth);

        tempWidth = fImageWidth;
        tempHeight = fImageHeight;

        unsigned int counterNew = 0;
        unsigned int counterOld = 0;

        for (int i = 0; i < fImageWidth; i++)
        {
            for (int j = 0; j < fImageHeight; j++)
            {
                tempNewMatrix[i][counterNew] = fImageMatrix[fImageHeight - j - 1][counterOld];
                tempNewMatrix[i][counterNew + 1] = fImageMatrix[fImageHeight - j - 1][counterOld + 1];
                tempNewMatrix[i][counterNew + 2] = fImageMatrix[fImageHeight - j - 1][counterOld + 2];

                counterNew += 3;
            }
            counterNew = 0;
            counterOld += 3;
        }

        fImageWidth = tempHeight * 3;
        fImageHeight = tempWidth;
    }
    else
    {
        tempNewMatrix = allocateMatrix(fImageHeight, fImageWidth);

        tempWidth = fImageWidth;
        tempHeight = fImageHeight;

        for (int i = 0; i < fImageWidth; i++)
        {
            for (int j = 0; j < fImageHeight; j++)
            {
                tempNewMatrix[i][j] = fImageMatrix[fImageHeight - j - 1][i]; // -1 because [][] starts at 00
            }
        }

    }

    deleteImageMatrix(fImageWidth, fImageHeight);

    fImageWidth = tempHeight;
    fImageHeight = tempWidth;

    fImageMatrix = tempNewMatrix;

}

void ImageData::saveAsImageToFile() // TODO add a check for forbidden symbols in the filename!
{
    std::ofstream writefile;
    char tempNewName[MAX_FILE_NAME_SIZE];

    std::cout << std::endl <<"Enter a new name for the image file( " << fFileName <<" )  :  ";
    std::cin >> tempNewName;

    if(fImageFormat == PBMA)
        std::strcat(tempNewName, ".pbm");

    if(fImageFormat == PGMA)
        std::strcat(tempNewName, ".pgm");

    if(fImageFormat == PPMA)
        std::strcat(tempNewName, ".ppm");

    std::cout << fFileName << " is going to be saved as: " << tempNewName << std::endl;

    writefile.open(TempImageName, std::ofstream::trunc);

    writefile << "P" << fImageFormat << std::endl;
    writefile << fImageWidth << " " << fImageHeight << std::endl;

    writeMatrixToFile(writefile);

    writefile.close();
    rename(TempImageName, tempNewName );
}
