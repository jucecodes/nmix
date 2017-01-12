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

#ifndef NMIX_OPERATIONHANDLER_H
#define NMIX_OPERATIONHANDLER_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "nmix_Operations.h"

namespace nmix
{
 
struct Viewport;
struct Stage;
struct Node;
    
struct OperationHandler : public juce::ChangeListener
{
 
    OperationHandler();
    ~OperationHandler();

    void selectAll();
    void invertSelection();
    void deselectAll();
    
    void addNode(bool fromKeyPress);
    void deleteSelection();
    void lockSelection();
    
    void nudgeSelection(const juce::KeyPress& k);
    void positionSelection(const juce::MouseEvent& e);

    void modifierKeysChanged(const juce::ModifierKeys& mods);
    void changeListenerCallback(juce::ChangeBroadcaster* source);
    
    nmix::Operation    currentOperation;
    juce::ModifierKeys currentModifiers;

    juce::Point<int> mouseOpOrigin;
    juce::Point<int> mouseModOrigin;

    juce::OwnedArray<nmix::Node>       stagedNodes;
    juce::SelectedItemSet<nmix::Node*> selectedNodes;
    
    juce::ScopedPointer<nmix::Stage>    currentStage;
    juce::ScopedPointer<nmix::Viewport> currentViewport;
};
    
}

#endif  // NMIX_OPERATIONHANDLER_H
