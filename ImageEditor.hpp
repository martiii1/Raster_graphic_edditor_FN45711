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


    void newSession();

    void CommandCaller();

    bool load(char *input);

    void saveImagesInCurrentSession(); // TODO add cout-s and checks

    void saveImagesAsInCurrentSession();

    bool add(char *input);

    bool switchSession(int sessionID);

    void rotateLeft();

    void rotateRight();

    void grayscaleCurrentSession();

    void monochromeCurrentSession();
};

