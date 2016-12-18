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
    commandManager = new juce::ApplicationCommandManager();
    mainWindow     = new nmix::MainWindow(getApplicationName());
}

void nmix::Application::anotherInstanceStarted(const juce::String &commandLine)
{
    
}

void nmix::Application::shutdown()
{
    mainWindow     = nullptr;
    commandManager = nullptr;
}

void nmix::Application::systemRequestedQuit()
{
    quit();
}

nmix::Application& nmix::Application::getApp()
{
    nmix::Application* const p = dynamic_cast<nmix::Application*>(JUCEApplication::getInstance());
    jassert(p != nullptr);
    return *p;
}

juce::ApplicationCommandManager& nmix::Application::getCommandManager()
{
    juce::ApplicationCommandManager* p = nmix::Application::getApp().commandManager;
    jassert(p != nullptr);
    return *p;
}

START_JUCE_APPLICATION (nmix::Application)