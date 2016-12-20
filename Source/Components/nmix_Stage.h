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

#ifndef NMIX_STAGE_H
#define NMIX_STAGE_H

#include "../JuceLibraryCode/JuceHeader.h"

namespace nmix
{
    
struct Node;
    
struct Stage : public juce::Component,
               public juce::LassoSource<nmix::Node*>,
               public juce::ChangeListener,
               public juce::ApplicationCommandTarget
{
public:
    Stage();
    ~Stage();
    
    enum ColourIds
    {
        backgroundColourId,
        foregroundColourId
    };
    
    enum OperationStates
    {
        None = 0,
        NudgeSelection,
        AdjustVolume,
        AdjustBalance,
    };
    
    uint8_t status;
    
    juce::ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands(juce::Array<juce::CommandID>& commands) override;
    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
    bool perform(const InvocationInfo& info) override;
    
    void mouseDown      (const juce::MouseEvent& e) override;
    void mouseDrag      (const juce::MouseEvent& e) override;
    void mouseUp        (const juce::MouseEvent& e) override;

    void findLassoItemsInArea(juce::Array<nmix::Node*>& results, const juce::Rectangle<int>& area) override;
    juce::SelectedItemSet<Node*>& getLassoSelection() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    juce::SelectedItemSet<nmix::Node*> selectedNodes;
    juce::OwnedArray<nmix::Node> stagedNodes;
    
    juce::LassoComponent<nmix::Node*> lasso;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Stage)
};
    
}

#endif  // NMIX_STAGE_H
