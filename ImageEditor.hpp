#pragma once

#include "Session.hpp"


class ImageEditor
{
public:
    ImageEditor();

    void StartImageEditor();

private:
    Session *fSessions;
    size_t fOpenSessions;
    unsigned int fNextSession;
    unsigned int fCurrentSession;


    void newSession();

    void CommandCaller();

    bool load(char *input);

    bool add(char *input);
};

