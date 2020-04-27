#include "ImageEditor.hpp"

#define MAX_CONSOLE_COMMANDS_LENGTH 255

ImageEditor::ImageEditor()
{
    fSessions = nullptr;
    fOpenSessions = 0;
    fNextSession = 0;
    fCurrentSession = 0;
}

void ImageEditor::StartImageEditor()
{
    fOpenSessions = 0;
    fNextSession = 0;

    std::cout << "Raster graphic editor started." << std::endl;

    CommandCaller();


}

void ImageEditor::newSession()
{
    Session *tempSessions = new(std::nothrow) Session[fNextSession + 1];
    if (tempSessions == nullptr)
    {
        std::cout << "Error while adding a session! " << std::endl;
        return;
    }

    for (size_t i = 0; i < fNextSession; i++)
    {
        if(!fSessions[i].isSessionOpen())
            continue;

        tempSessions[i] = fSessions[i];
        fSessions[i].~Session();
    }


    fSessions = tempSessions;
    fSessions[fNextSession].setSize(0);
    fSessions[fNextSession].setOpen();
    fSessions[fNextSession].setSessionID(fNextSession);

    std::cout << std::endl << std::endl << "Session with ID:" << fSessions[fNextSession].getSessionID()
    << " started." << std::endl;
}

void ImageEditor::CommandCaller()
{
    char *consoleCommands = new char[MAX_CONSOLE_COMMANDS_LENGTH];
    char *consoleCommandsLine = new char[MAX_CONSOLE_COMMANDS_LENGTH];
    char *token;

    bool correctInput = false;
    bool endOfProgram = false;

    while (!correctInput || !endOfProgram)
    {
        correctInput = true;

        std::cout << "Please enter a command (max " << MAX_CONSOLE_COMMANDS_LENGTH << " symbols) " << std::endl;
        std::cin.getline(consoleCommands, MAX_CONSOLE_COMMANDS_LENGTH);
        strcpy(consoleCommandsLine, consoleCommands);

        token = strtok(consoleCommands, " ");
        if (token == nullptr)
        {
            correctInput = false;
            std::cout << "Invalid input! Try again" << std::endl;
            continue;
        }

        if (strcmp(token, "load") == 0)        // load is called
        {
            if (!load(consoleCommandsLine))
            {
                std::cout << "Invalid input! Try again" << std::endl;
                correctInput = false;
                continue;
            }
        }


        if (strcmp(token, "grayscale") == 0)    // grayscale is called
        {


        }

        if (strcmp(token, "monochrome") == 0)   // monochrome is called
        {


        }

        if (strcmp(token, "negative") == 0)     // negative is called
        {


        }

        if (strcmp(token, "rotate") == 0)       // rotate is called
        {

            //rotateRight();
            rotateLeft();

        }

        if (strcmp(token, "undo") == 0)         // undo is called
        {


        }

        if (strcmp(token, "add") == 0)          // add is called
        {


        }

        if (strcmp(token, "session") == 0)      // session is called
        {


        }

        if (strcmp(token, "switch") == 0)       // switch is called
        {


        }

        if (strcmp(token, "collage") == 0)      // collage  is called
        {


        }

        if (strcmp(token, "close") == 0)        // close  is called
        {


        }

        if (strcmp(token, "save") == 0)         // close  is called
        {
            //add save as
            saveImagesInCurrentSession();
            endOfProgram = true;

        }

        if (strcmp(token, "help") == 0)         //
        {
            showAllSessions();

        }


    } // end of while

    delete[] consoleCommands;
}

bool ImageEditor::load(char *input)
{
    char *token;
    token = strtok(input, " "); // skips the first word

    token = strtok(nullptr, " "); // if there is nothing after "load"
    if (token == nullptr)
    {
        return false;
    }

    newSession();
    fCurrentSession = fNextSession;

    while (token != nullptr)
    {
        if (!fSessions[fCurrentSession].addImage(token))
            return false;

        token = strtok(nullptr, " ");
    }
    std::cout << std::endl;

    fNextSession++;
}

bool ImageEditor::add(char *input) // Adds an image to the current session.
{
    char *token;
    token = strtok(input, " "); // skips the first word

    token = strtok(nullptr, " "); // if there is nothing after "add"
    if (token == nullptr)
    {
        return false;
    }

    if (!fSessions[fCurrentSession].addImage(token))
        return false;

    std::cout << std::endl;
    return true;
}

void ImageEditor::showAllSessions()
{
    size_t counter=0;

    while (counter < fNextSession)
    {
        if(fSessions[counter].isSessionOpen())
        {
            fSessions[counter].printSessionInfo();
        }

        counter++;
    }

}

bool ImageEditor::switchSession(int sessionID)
{
    if(fSessions[sessionID].isSessionOpen())
    {
        fCurrentSession = sessionID;
        std::cout << "Session with ID: " << sessionID <<" is loaded! " << std::endl;
        return true;
    }

    return false;

}

void ImageEditor::rotateLeft()
{
    fSessions[fCurrentSession].rotateSessionLeft(); // TODO add changes made!

}

void ImageEditor::saveImagesInCurrentSession()
{
    fSessions[fCurrentSession].saveImages();

}

void ImageEditor::rotateRight()
{
    fSessions[fCurrentSession].rotateSessionRight(); // TODO add changes made!

}
