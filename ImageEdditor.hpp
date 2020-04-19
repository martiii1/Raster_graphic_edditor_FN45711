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
    unsigned int fnextSession = 0;


    void newSession();
    void CommandCaller(char* command);
};

