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
        tempSessions[i] = fSessions[i];
        fSessions[i].~Session();
    }


    fSessions = tempSessions;
    fSessions[fNextSession].setSize(0);
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
            negativeCurrentSession();


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
                fSessions[fCurrentSession].undoLastChange();


            if (strcmp(token, "add") == 0)          // add is called
                add(consoleCommandsLine);


            if (strcmp(token, "session") == 0)      // session is called
            {
                token = strtok(nullptr, " ");

                if (token == nullptr)
                    throw std::exception(invalidInputErrorMessage);

                if (strcmp(token, "info") == 0)       // rotate left
                {
                    fSessions[fCurrentSession].printSessionInfo();
                }
                else
                {
                    throw std::exception(invalidInputErrorMessage);
                }
            }


            if (strcmp(token, "switch") == 0)       // switch is called
            {
                token = strtok(nullptr, " ");

                if (token == nullptr)
                    throw std::exception(invalidInputErrorMessage);

                switchSession(atoi(token));
            }


            if (strcmp(token, "collage") == 0)      // collage  is called
                collage(consoleCommandsLine);



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
                help();


            if (strcmp(token, "exit") == 0)
                endOfProgram = true;
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
        fSessions[counter].printSessionInfo();
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

    fSessions[fCurrentSession].rotateSessionLeft();

}

void ImageEditor::saveImagesInCurrentSession()
{
    fSessions[fCurrentSession].saveImages();
}

void ImageEditor::rotateRight()
{
    fSessions[fCurrentSession].rotateSessionRight();
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

void ImageEditor::negativeCurrentSession()
{
    fSessions[fCurrentSession].negativeSession();
}

void ImageEditor::help()
{
    std::cout << "The image editor supports the following commands:\n\n";

    std::cout << "Parameters used in the help instructions.\n\n";
    std::cout << "The parameter <image> is [imageName].[ppm/pgm/pbm]\n\n ";
    std::cout << "The parameter <direction> is \"left\" or \"right\" \n\n ";
    std::cout << "The parameter <session ID> is an integer\n\n";
    std::cout << "The parameter <collage name> is the name of the new photo(the collage)\n\n";

    std::cout << "Commands:\n\n";
    std::cout << "load <image> -> loads a new session with <image>\n\n";
    std::cout << "add <image> -> adds <image> to current session\n\n";
    std::cout << "save -> saves all images in the current session with their default names\n\n";
    std::cout << "save as -> saves all images with custom names\n\n";
    std::cout << "help -> prints these instructions\n\n";
    std::cout << "exit -> exits the program\n\n";
    std::cout << "grayscale -> converts all images in current session to grayscale type\n\n";
    std::cout << "monochrome -> converts all images in current session to monochrome type\n\n";
    std::cout << "negative -> converts all images in current session to negatives\n\n";
    std::cout << "rotate <direction> -> rotates all images in current session in the given direction\n\n";
    std::cout << "undo -> reverses the last change in current session\n\n";
    std::cout << "session info -> prints all images and changes in current session\n\n";
    std::cout << "switch<session ID> -> switches the session with ID <session ID>\n\n";
    std::cout << "collage<vertical/horizontal> <image1> <image2> <collage name> ->"
                 " creates a collage of <image1> and <image2> and adds it to the current session\n\n";
}
