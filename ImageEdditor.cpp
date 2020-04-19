#include "ImageEdditor.hpp"

#define MAX_CONSOLE_COMMANDS_LENGTH 255

Driver:: Driver()
{
    fSessions = nullptr;
    fOpenSessions = 0;
    fnextSession = 0;
}

void Driver::StartImageEditor()
{
    fOpenSessions = 0;

    char *consoleCommands = new char[MAX_CONSOLE_COMMANDS_LENGTH];

    std::cin.getline(consoleCommands, MAX_CONSOLE_COMMANDS_LENGTH);

    CommandCaller(consoleCommands);


    delete[] consoleCommands;
}

void Driver::newSession()
{
    Session *tempSessions = new(std::nothrow) Session[fOpenSessions + 1];
    if (tempSessions == nullptr)
    {
        std::cout << "Error while adding a session! " << std::endl;
        return;
    }

    for (size_t i = 0; i < fOpenSessions; i++)
    {
        tempSessions[i] = fSessions[i];
        fSessions[i].~Session();
    }

    fSessions = tempSessions;
    fOpenSessions++;
}

void Driver::CommandCaller(char *command)
{
    char *token = strtok(command, " ");
    while (token != nullptr)
    {
        if (strcmp(token, "load") == 0)
        {

            token = strtok(nullptr, " ");

            newSession();
            while (token != nullptr)
            {
                fSessions[sessionToOpen].addImage(token);
                token = strtok(nullptr, " ");
            }


            sessionToOpen++;
        }


        break;
        token = strtok(NULL, " ");
    }


}
