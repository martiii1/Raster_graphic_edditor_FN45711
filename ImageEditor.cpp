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
        if (!fSessions[i].isSessionOpen())
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
    const char *invalidInputErrorMessage = "Invalid input! Try again \n";
    const char *invalidAllocErrorMessage = "Invalid input! Try again \n";


    char *consoleCommands = new char[MAX_CONSOLE_COMMANDS_LENGTH];
    char *consoleCommandsLine = new char[MAX_CONSOLE_COMMANDS_LENGTH];
    char *token;

    bool endOfProgram = false;

    while (!endOfProgram)
    {

        try
        {
            std::cout << std::endl
                      << "Please enter a command (max " << MAX_CONSOLE_COMMANDS_LENGTH << " symbols) "
                      << std::endl;
            std::cin.getline(consoleCommands, MAX_CONSOLE_COMMANDS_LENGTH);

            strcpy(consoleCommandsLine, consoleCommands);

            token = strtok(consoleCommands, " ");
            if (token == nullptr)
                throw std::invalid_argument(invalidInputErrorMessage);


            if (strcmp(token, "load") == 0)        // load is called
                load(consoleCommandsLine);


            if (strcmp(token, "grayscale") == 0)    // grayscale is called
                grayscaleCurrentSession();


            if (strcmp(token, "monochrome") == 0)   // monochrome is called
                monochromeCurrentSession();

            if (strcmp(token, "negative") == 0)     // negative is called
            {


            }

            if (strcmp(token, "rotate") == 0)       // rotate is called
            {
                token = strtok(nullptr, " ");

                if (token == nullptr)
                    throw std::exception(invalidInputErrorMessage);
                if (strcmp(token, "left") == 0)       // rotate left
                {
                    rotateLeft();
                }
                else if (strcmp(token, "right") == 0)       // rotate right
                {
                    rotateRight();
                }
                else
                {
                    throw std::invalid_argument(invalidInputErrorMessage);
                }


            }

            if (strcmp(token, "undo") == 0)         // undo is called
            {


            }

            if (strcmp(token, "add") == 0)          // add is called
            {
                add(consoleCommandsLine);
            }

            if (strcmp(token, "session") == 0)      // session is called
            {


            }

            if (strcmp(token, "switch") == 0)       // switch is called
            {
                token = strtok(nullptr, " ");

                if (token == nullptr)
                    throw std::exception(invalidInputErrorMessage);

                switchSession(atoi(token));
            }

            if (strcmp(token, "collage") == 0)      // collage  is called
            {
                collage(consoleCommandsLine);
            }

            if (strcmp(token, "close") == 0)        // close  is called
            {


            }

            if (strcmp(token, "save") == 0)         // save is called
            {
                token = strtok(nullptr, " ");
                if (token == nullptr)
                {
                    saveImagesInCurrentSession();
                    endOfProgram = true;
                }
                else if (strcmp(token, "as") == 0)
                {
                    saveImagesAsInCurrentSession();
                    endOfProgram = true;
                }
                else
                {
                    throw std::invalid_argument(invalidInputErrorMessage);
                }


            }

            if (strcmp(token, "help") == 0)
            {
                showAllSessions();

            }

            if (strcmp(token, "exit") == 0)
            {
                endOfProgram = true;
            }

        }
        catch (std::exception &)
        {
            handle_exception();
        }


    } // end of while

    delete[] consoleCommandsLine;
    delete[] consoleCommands;

}

void ImageEditor::handle_exception()
{
    const char *invalidFileErrorMessage = "The Image file was not supported or corrupted \n";
    const char *unknownErrorMessage = "Unknown error has occurred. Please try again. \n";

    try
    {
        throw;
    }
    catch (std::invalid_argument &message)
    {
        std::cout << message.what();
    }

    catch (std::bad_exception &)
    {
        std::cout << unknownErrorMessage;
    }
    catch (std::exception &message)
    {
        std::cout << message.what() << "\n";
    }
    catch (...)
    {
        std::cout << unknownErrorMessage;
    }
}

void ImageEditor::load(char *input)
{
    const char *invalidImageName = "You have to specify a valid image name! \n";

    char *token;
    token = strtok(input, " "); // skips the first word ("load")

    token = strtok(nullptr, " "); // if there is nothing after "load"
    if (token == nullptr)
        throw std::invalid_argument(invalidImageName);

    newSession();
    fCurrentSession = fNextSession;

    while (token != nullptr)
    {
        fSessions[fCurrentSession].addImage(token);

        token = strtok(nullptr, " ");
    }
    std::cout << std::endl;

    fNextSession++;
}

void ImageEditor::add(char *input) // Adds an image to the current session.
{
    char *token;
    token = strtok(input, " "); // skips the first word

    token = strtok(nullptr, " "); // if there is nothing after "add"
    if (token == nullptr)
        throw std::exception("Invalid input. Try again.\n");

    fSessions[fCurrentSession].addImage(token);

    std::cout << std::endl;
}

void ImageEditor::showAllSessions()
{
    size_t counter = 0;

    while (counter < fNextSession)
    {
        if (fSessions[counter].isSessionOpen())
        {
            fSessions[counter].printSessionInfo();
        }

        counter++;
    }

}

void ImageEditor::switchSession(int sessionID)
{
    fCurrentSession = sessionID;
    std::cout << "Session with ID: " << sessionID << " is loaded! " << std::endl;
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

void ImageEditor::saveImagesAsInCurrentSession()
{
    fSessions[fCurrentSession].saveImagesAs();
}

void ImageEditor::grayscaleCurrentSession()
{
    fSessions[fCurrentSession].grayscaleSession();
}

void ImageEditor::monochromeCurrentSession()
{
    fSessions[fCurrentSession].monochromeSession();
}

void ImageEditor::rotateImage()
{

}

void ImageEditor::collage(char *input)
{
    const char *invalidImageName = "You have to specify a valid image name! \n";
    const char *invalidInput = "Invalid input! Try again. \n";

    char *token;
    token = strtok(input, " "); // skips the first word ("collage")

    token = strtok(nullptr, " "); // takes the direction
    if (token == nullptr)
        throw std::invalid_argument(invalidImageName);

    char* tempImage1Name;
    char* tempImage2Name;
    char* tempOutImageName;

    bool isVertical;
    if (strcmp(token, "horizontal") == 0)
        isVertical = false;
    else if (strcmp(token, "vertical") == 0)
        isVertical = true;
    else
        throw std::exception(invalidInput);

    token = strtok(nullptr, " "); // takes image1 name
    if (token == nullptr)
        throw std::invalid_argument(invalidImageName);

    tempImage1Name = new char[strlen(token) + 1];
    strcpy(tempImage1Name,token);


    token = strtok(nullptr, " "); // takes image2 name
    if (token == nullptr)
        throw std::invalid_argument(invalidImageName);

    tempImage2Name = new char[strlen(token) + 1];
    strcpy(tempImage2Name,token);


    token = strtok(nullptr, " "); // takes outImage name
    if (token == nullptr)
        throw std::invalid_argument(invalidImageName);

    tempOutImageName = new char[strlen(token) + 1];
    strcpy(tempOutImageName,token);

    try
    {
        fSessions[fCurrentSession].addCollage(tempImage1Name,tempImage2Name,tempOutImageName,isVertical);
    }
    catch (...)
    {
        delete [] tempImage1Name;
        delete [] tempImage2Name;
        delete [] tempOutImageName;
        throw;
    }


}
