#pragma once

#include "Session.hpp"


class ImageEditor
{
public:
    ImageEditor();

    void StartImageEditor();

    void showAllSessions();

private:
    Session *fSessions;
    size_t fOpenSessions;
    unsigned int fNextSession;
    unsigned int fCurrentSession;

    void handle_exception();

    void newSession();

    void CommandCaller();

    void load(char *input);

    void saveImagesInCurrentSession();

    void saveImagesAsInCurrentSession();

    void add(char *input);

    void switchSession(int sessionID);

    void collage(char* input);

    void rotateLeft();

    void rotateRight();

    void grayscaleCurrentSession();

    void monochromeCurrentSession();

    void negativeCurrentSession();

    void help();
};

