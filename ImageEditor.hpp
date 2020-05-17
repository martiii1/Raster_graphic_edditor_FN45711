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

    void saveImagesInCurrentSession(); // TODO add cout-s and checks

    void saveImagesAsInCurrentSession();

    bool add(char *input);

    bool switchSession(int sessionID);

    void rotateLeft();

    void rotateRight();

    void rotateImage();

    void grayscaleCurrentSession();

    void monochromeCurrentSession();
};

