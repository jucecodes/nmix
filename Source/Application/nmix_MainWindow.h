/*
 *  Copyright 2016 - 2017 Antonio Lassandro
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

#ifndef NMIX_MAINWINDOW_H
#define NMIX_MAINWINDOW_H

#include "../JuceLibraryCode/JuceHeader.h"

namespace nmix
{
    
struct MainWindow : public juce::DocumentWindow,
                    public juce::ApplicationCommandTarget
{
    MainWindow(juce::String name);
    
    juce::ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands(juce::Array<juce::CommandID>& commands) override;
    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
    bool perform(const InvocationInfo& info) override;

    void modifierKeysChanged(const juce::ModifierKeys& mods) override;
    
    void closeButtonPressed() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

}

#endif  // NMIX_MAINWINDOW_H
