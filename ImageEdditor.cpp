#include "ImageEdditor.hpp"

#define MAX_CONSOLE_COMMANDS_LENGTH 255

Driver::Driver()
{
    fSessions = nullptr;
    fOpenSessions = 0;
    fNextSession = 0;
}

void Driver::StartImageEditor()
{
    fOpenSessions = 0;

    std::cout << "Raster graphic editor started." << std::endl;

    CommandCaller();



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

void Driver::CommandCaller()
{
    char *consoleCommands = new char[MAX_CONSOLE_COMMANDS_LENGTH];
    char *token;

    bool correctInput = false;

    while (!correctInput)
    {
        correctInput = true;

        std::cout << "Please enter a command (max " << MAX_CONSOLE_COMMANDS_LENGTH << "symbols) " << std::endl;
        std::cin.getline(consoleCommands, MAX_CONSOLE_COMMANDS_LENGTH);

        token = strtok(consoleCommands, " ");
        if (token == nullptr)
        {
            correctInput = false;
            std::cout << "Invalid input! Try again" << std::endl;
            continue;
        }

        if (strcmp(token, "load") == 0)        // load is called
        {
            if(!load(consoleCommands))
            {
                std::cout << "Invalid input! Try again" << std::endl;
                correctInput = false;
                continue;
            }
        }


        if(strcmp(token, "grayscale") == 0)    // grayscale is called
        {


        }

        if(strcmp(token, "monochrome") == 0)   // monochrome is called
        {


        }

        if(strcmp(token, "negative") == 0)     // negative is called
        {


        }

        if(strcmp(token, "rotate") == 0)       // rotate is called
        {


        }

        if(strcmp(token, "undo") == 0)         // undo is called
        {


        }

        if(strcmp(token, "add") == 0)          // add is called
        {


        }

        if(strcmp(token, "session") == 0)      // session is called
        {


        }

        if(strcmp(token, "switch") == 0)       // switch is called
        {


        }

        if(strcmp(token, "collage") == 0)      // collage  is called
        {


        }

        if(strcmp(token, "close") == 0)        // close  is called
        {


        }

        if(strcmp(token, "save") == 0)         // close  is called
        {
            //add save as


        }

        if(strcmp(token, "help") == 0)         // BULLSHIT
        {


        }





    } // end of while

    delete[] consoleCommands;
}

bool Driver::load(char *input)
{
    char* token;
    token = strtok(input, " "); // skips the first word

    token = strtok(nullptr, " ");
    if(token == nullptr)
    {
       return false;
    }
    newSession();
    while (token != nullptr)
    {
        fSessions[fNextSession].addImage(token);
        token = strtok(nullptr, " ");
    }


    fNextSession++;
}
