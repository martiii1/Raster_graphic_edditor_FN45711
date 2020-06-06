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

void ImageData::loadImage(char *FileName)
{

    try
    {
        fFileName = new char[strlen(FileName) + 1];
    }
    catch (std::bad_alloc &)
    {
        delImage();
        throw;
    }
    catch (std::exception &)
    {
        throw std::bad_exception();
    }

    strcpy(fFileName, FileName);

    std::ifstream image(fFileName);

    if (!image.is_open() || !image.good())
    {
        //std::cout << "Can't open the file!" << std::endl;
        image.close();
        throw std::invalid_argument("Error while opening the image");
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
        throw std::exception("Image file corrupted");
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
    //<< fPixelMaxValue;
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
    int **Matrix = new int *[height];


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
    else
        tempWidth = fImageWidth;

    size_t counter = 0;

    unsigned short int newLineLimit; // maybe change
    if (fImageFormat == PPMA)
        newLineLimit = 12;
    if (fImageFormat == PGMA)
        newLineLimit = 19;
    if (fImageFormat == PBMA)
        newLineLimit = 39;

    for (int i = 0; i < fImageHeight; i++)
    {
        for (int j = 0; j < tempWidth; j++)
        {
            if (counter >= newLineLimit && counter % newLineLimit == 0)
                file << std::endl; // the file must be consisted of lines no longer than 70 characters

            file << fImageMatrix[i][j] << " ";
            counter++;
        }

        //file << std::endl;
    }
}

#define TempImageName "tempImageFile.txt"

void ImageData::saveImageToFile()
{
    std::ofstream writefile;
    char tempNewName[MAX_FILE_NAME_SIZE];
    std::strcpy(tempNewName, fFileName);

    writefile.open(TempImageName, std::ofstream::trunc);

    writefile << "P" << fImageFormat << std::endl;
    writefile << fImageWidth << " " << fImageHeight << std::endl;

    if (fImageFormat != PBMA)
        writefile << fPixelMaxValue << std::endl;

    writeMatrixToFile(writefile);

    writefile.close();
    remove(tempNewName);
    rename(TempImageName, tempNewName);
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

    std::cout << std::endl << "Enter a new name for the image file( " << fFileName << " )  :  ";
    std::cin >> tempNewName;

    if (fImageFormat == PBMA)
        std::strcat(tempNewName, ".pbm");

    if (fImageFormat == PGMA)
        std::strcat(tempNewName, ".pgm");

    if (fImageFormat == PPMA)
        std::strcat(tempNewName, ".ppm");

    std::cout << fFileName << " is going to be saved as: " << tempNewName << std::endl;

    writefile.open(TempImageName, std::ofstream::trunc);

    writefile << "P" << fImageFormat << std::endl;
    writefile << fImageWidth << " " << fImageHeight << std::endl;

    if (fImageFormat != PBMA)
        writefile << fPixelMaxValue << std::endl;

    writeMatrixToFile(writefile);

    writefile.close();
    rename(TempImageName, tempNewName);
}

void ImageData::makeImageGrayscale()
{
    bool imageIsAlreadyGrayscale;

    if (fImageFormat == PPMA)
    {
        if (grayscaleMatrix())
            std::cout << "Image " << fFileName << " converted to grayscale. " << std::endl;
        else
            std::cout << "The image is .ppma format but is already grayscale. No changes will be made." << std::endl;
    }
    else
    {
        std::cout << "Image " << fFileName << " is already grayscale. No changes will be made!" << std::endl;
        return;
    }

}

bool ImageData::grayscaleMatrix()
{
    // Weight values for nicer image taken from https://www.tutorialspoint.com/dip/grayscale_to_rgb_conversion.htm
    const float redWeight = 0.3;
    const float greenWeight = 0.6;
    const float blueWeight = 0.1;

    unsigned short int tempPixelValue;

    bool imageIsAlreadyGrayscale = true;

    if (fImageFormat != PPMA)
    {
        std::cout << "Image " << fFileName << " is already grayscale/monochrome. No changes will be made! "
                  << std::endl;
        return true;
    }

    for (int i = 0; i < fImageHeight; i++)
    {
        for (int j = 0; j < fImageWidth * 3; j += 3)
        {
            if (!((fImageMatrix[i][j] == fImageMatrix[i][j + 1]) &&  // is true the pixel is gray. !true = false
                  (fImageMatrix[i][j + 1] == fImageMatrix[i][j + 2]))) // true when the pixel has color
            {
                tempPixelValue = 0;
                imageIsAlreadyGrayscale = false;

                tempPixelValue += (int) ((float) fImageMatrix[i][j] * redWeight);
                tempPixelValue += (int) ((float) fImageMatrix[i][j + 1] * greenWeight);
                tempPixelValue += (int) ((float) fImageMatrix[i][j + 2] * blueWeight);
                tempPixelValue = tempPixelValue / 3;

                fImageMatrix[i][j] = tempPixelValue;
                fImageMatrix[i][j + 1] = tempPixelValue;
                fImageMatrix[i][j + 2] = tempPixelValue;
            }

        }
    }

    return !imageIsAlreadyGrayscale;
}

void ImageData::makeImageMonochrome()
{
    if (fImageFormat == PBMA)
    {
        std::cout << "Image " << fFileName << " is already monochrome. No changes will be made! " << std::endl;
        return;
    }

    unsigned int tempImageWidth;
    if (fImageFormat == PPMA)
    {
        grayscaleMatrix();
        tempImageWidth = fImageWidth * 3;
    }
    else
    {
        tempImageWidth = fImageWidth;
    }

    unsigned short int maxPixelUsed = 0;
    bool imageIsAlreadyMonochrome = true;

    if (fImageFormat == PPMA)
    {
        for (int i = 0; i < fImageHeight; i++)
        {
            for (int j = 0; j < tempImageWidth; j += 3)
            {
                if (maxPixelUsed < fImageMatrix[i][j])
                    maxPixelUsed = fImageMatrix[i][j];
            }
        }
    }

    if (fImageFormat == PGMA)
    {
        for (int i = 0; i < fImageHeight; i++)
        {
            for (int j = 0; j < fImageWidth; j += 3)
            {
                if (maxPixelUsed < fImageMatrix[i][j])
                    maxPixelUsed = fImageMatrix[i][j];
            }
        }
    }


    for (int i = 0; i < fImageHeight; i++)
    {
        for (int j = 0; j < fImageWidth * 3; j += 3)
        {


            if ((fImageMatrix[i][j] != 0) && (fImageMatrix[i][j] != fPixelMaxValue))
            {
                imageIsAlreadyMonochrome = false;
            }


            if (fImageMatrix[i][j] <= maxPixelUsed / 2)
            {
                fImageMatrix[i][j] = 0;
                fImageMatrix[i][j + 1] = 0;
                fImageMatrix[i][j + 2] = 0;
            }
            else
            {
                fImageMatrix[i][j] = fPixelMaxValue;
                fImageMatrix[i][j + 1] = fPixelMaxValue;
                fImageMatrix[i][j + 2] = fPixelMaxValue;
            }

        }

    }

    if (imageIsAlreadyMonochrome)
        std::cout << "The image is .ppma format but is already monochrome. No changes will be made." << std::endl;
    else
        std::cout << "Image " << fFileName << " converted to monochrome. " << std::endl;

}

void ImageData::createCollage(ImageData &image1, ImageData &image2, const char *outImageName, bool isVertical)
{
    if (isVertical)
        collageVerticalMatrix(image1, image2);
    else
        collageHorizontalMatrix(image1, image2);

    fFileName = new char[strlen(outImageName) + 5]; // + 1 for \0 and +4 for the ".ppm /.pgm /.pbm"

    strcpy(fFileName, outImageName);

    if (fImageFormat == PPMA)
        strcat(fFileName, ".ppm");

    if (fImageFormat == PGMA)
        strcat(fFileName, ".pgm");

    if (fImageFormat == PBMA)
        strcat(fFileName, ".pbm");
}

void ImageData::collageVerticalMatrix(ImageData &image1, ImageData &image2)
{
    if (image1.fImageFormat != image2.fImageFormat)
        throw std::exception("These images aren't compatible for a vertical collage! \n");

    if (image1.fImageWidth != image2.fImageWidth)
        throw std::exception("These images aren't compatible for a vertical collage! \n");


    if (image1.fPixelMaxValue > image2.fPixelMaxValue)
        fPixelMaxValue = image1.fPixelMaxValue;
    else
        fPixelMaxValue = image2.fPixelMaxValue;

    fImageFormat = image1.fImageFormat;
    fImageWidth = image1.fImageWidth;
    fImageHeight = image1.fImageHeight + image2.fImageHeight;

    if(fImageFormat == PPMA)
        fImageWidth = fImageWidth * 3;

    fImageMatrix = allocateMatrix(fImageWidth, fImageHeight);

    for (int i = 0; i < image1.fImageHeight; i++)
    {
        for (int j = 0; j < fImageWidth; j++)
        {
            fImageMatrix[i][j] = image1.fImageMatrix[i][j];
        }
    }

    for (unsigned i = image1.fImageHeight; i < fImageHeight; i++)
    {
        for (int j = 0; j < fImageWidth; j++)
        {
            fImageMatrix[i][j] = image2.fImageMatrix[i - image1.fImageHeight][j];
        }
    }

    if(fImageFormat == PPMA)
        fImageWidth = fImageWidth / 3;

}

void ImageData::collageHorizontalMatrix(ImageData &image1, ImageData &image2)
{
    if (image1.fImageFormat != image2.fImageFormat)
        throw std::exception("These images aren't compatible for a vertical collage! \n");

    if (image1.fImageHeight != image2.fImageHeight)
        throw std::exception("These images aren't compatible for a vertical collage! \n");


    if (image1.fPixelMaxValue > image2.fPixelMaxValue)
        fPixelMaxValue = image1.fPixelMaxValue;
    else
        fPixelMaxValue = image2.fPixelMaxValue;

    fImageFormat = image1.fImageFormat;
    fImageHeight = image1.fImageHeight;
    fImageWidth = image1.fImageWidth + image2.fImageWidth;

    unsigned int tempImage1MatrixWidth = image1.fImageWidth;
    unsigned int tempImage2MatrixWidth = image2.fImageWidth;

    if(fImageFormat == PPMA)
    {
        fImageWidth = fImageWidth * 3;
        tempImage1MatrixWidth = tempImage1MatrixWidth * 3;
        tempImage2MatrixWidth = tempImage2MatrixWidth * 3;
    }

    fImageMatrix = allocateMatrix(fImageWidth, fImageHeight);

    for (int i = 0; i < image1.fImageHeight; i++)
    {
        for (int j = 0; j < tempImage1MatrixWidth; j++)
        {
            fImageMatrix[i][j] = image1.fImageMatrix[i][j];
        }

        for (unsigned j = tempImage1MatrixWidth; j < fImageWidth; j++)
        {
            fImageMatrix[i][j] = image2.fImageMatrix[i][j - tempImage1MatrixWidth];
        }
    }

    if(fImageFormat == PPMA)
        fImageWidth = fImageWidth / 3;
}

