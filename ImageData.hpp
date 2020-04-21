#pragma once
#pragma warning(disable:4996)

#include <iostream>
#include <fstream>

#define MAX_FILE_NAME_SIZE 255


#define PBMA 1
#define PGMA 2
#define PPMA 3
#define BROKEN 4


class ImageData
{
public:
    ImageData();
    ImageData(char *FileName);
    ImageData(const ImageData &other);
    ImageData &operator=(const ImageData &other);
    ~ImageData();

    char *getFileName();
    bool loadImage(char *FileName);
    void delImage();


    unsigned short int getImageFormat() const;
    unsigned int getImageWidth() const;
    unsigned int getImageHeight() const;
    unsigned int getPixelMaxValues() const;
    void writeMatrixToFile(std::ofstream &file);


    //char* fImageComments;
    int **fImageMatrix; // unsigned TODO
    void rotateImageLeft();

    void rotateImageRight();

private:
    char *fFileName;
    unsigned short int fImageFormat;
    unsigned int fImageWidth;
    unsigned int fImageHeight;
    unsigned int fPixelMaxValues;

    void copyImage(const ImageData &otherImage);

    void readPBMA(std::ifstream &file);

    void readPGMA(std::ifstream &file);

    void readPPMA(std::ifstream &file);

    void getDimensions(std::ifstream &file);

    void getPixelMaxValues(std::ifstream &file);

    void getPBMApixels(std::ifstream &file);

    void getPGMApixels(std::ifstream &file);

    void getPPMApixels(std::ifstream &file);

    size_t getImageFormat(std::ifstream &file);

    void skipComments(std::ifstream &file);

    int **allocateMatrix(unsigned int width, unsigned int height);

    void deleteImageMatrix(unsigned int width, unsigned int height);

};
