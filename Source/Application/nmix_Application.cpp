/*
 *  Copyright 2016 Antonio Lassandro 
 *
 *  nmix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nmix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../JuceLibraryCode/JuceHeader.h"

#include "nmix_Application.h"
#include "nmix_MainWindow.h"
#include "nmix_OperationHandler.h"

nmix::Application::Application()
{
    
}

const juce::String nmix::Application::getApplicationName()
{
    return ProjectInfo::projectName;
}

const juce::String nmix::Application::getApplicationVersion()
{
    return ProjectInfo::versionString;
}

bool nmix::Application::moreThanOneInstanceAllowed()
{
    return false;
}

void nmix::Application::initialise(const juce::String &commandLine)
{
    commandManager   = new juce::ApplicationCommandManager();
    operationHandler = new nmix::OperationHandler();
    mainWindow       = new nmix::MainWindow(getApplicationName());
}

void nmix::Application::anotherInstanceStarted(const juce::String &commandLine)
{
    
}

void nmix::Application::shutdown()
{
    mainWindow       = nullptr;
    operationHandler = nullptr;
    commandManager   = nullptr;
}

void nmix::Application::systemRequestedQuit()
{
    quit();
}

juce::ApplicationCommandManager& nmix::Application::getCommandManager()
{
    nmix::Application* const app = dynamic_cast<nmix::Application*>(JUCEApplication::getInstance());    
    juce::ApplicationCommandManager* cmd = app->commandManager;
    jassert(cmd != nullptr);
    return *cmd;
}

nmix::OperationHandler& nmix::Application::getOperationHandler()
{
    nmix::Application* const app = dynamic_cast<nmix::Application*>(JUCEApplication::getInstance());
    nmix::OperationHandler* op = app->operationHandler;
    jassert(op != nullptr);
    return *op;
}

START_JUCE_APPLICATION (nmix::Application)