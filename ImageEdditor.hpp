#pragma once
#include "Session.hpp"


class Driver
{
public:
    Driver();

    void StartImageEditor();

private:
    Session* fSessions;
    size_t fOpenSessions;
    unsigned int fNextSession = 0;


    void newSession();
    void CommandCaller();

    bool load(char* input);
};

