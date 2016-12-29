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

#ifndef NMIX_APPLICATION
#define NMIX_APPLICATION

#include "../JuceLibraryCode/JuceHeader.h"

namespace nmix
{
    
struct MainWindow;
struct OperationHandler;
    
struct Application : public juce::JUCEApplication
{
    Application();
    
    const juce::String getApplicationName()    override;
    const juce::String getApplicationVersion() override;
    bool moreThanOneInstanceAllowed()          override;
    
    void initialise(const juce::String& commandLine) override;
    void anotherInstanceStarted(const juce::String& commandLine) override;
    
    void shutdown() override;
    void systemRequestedQuit() override;
    
    static nmix::Application& getApp();
    static juce::ApplicationCommandManager& getCommandManager();
    static nmix::OperationHandler&          getOperationHandler();
    
    juce::ScopedPointer<nmix::MainWindow> mainWindow;
    juce::ScopedPointer<juce::ApplicationCommandManager> commandManager;
    
    juce::ScopedPointer<nmix::OperationHandler> operationHandler;
};
    
}

#endif  // NMIX_APPLICATION_H
