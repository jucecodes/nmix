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

#ifndef NMIX_NODE_H
#define NMIX_NODE_H

#include "../JuceLibraryCode/JuceHeader.h"

namespace nmix
{

struct Stage;
struct OperationHandler;
    
struct Node : public juce::Component
{
    Node(nmix::Stage& s, nmix::OperationHandler& o);
    ~Node();
    
    enum ColourIds
    {
        backgroundColourId,
        highlightColourId,
    };
    
    enum StatusIds
    {
        None     = 0,
        Hovered  = 1 << 0,
        Selected = 1 << 1,
        Locked   = 1 << 2,
    };
    
    uint8_t status;
    
    void mouseEnter (const juce::MouseEvent& e) override;
    void mouseExit  (const juce::MouseEvent& e) override;
    void mouseDown  (const juce::MouseEvent& e) override;
    void mouseDrag  (const juce::MouseEvent& e) override;
    void mouseUp    (const juce::MouseEvent& e) override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    bool mouseDownResult;
    
    juce::Point<int> currentOpOrigin;
    juce::Point<int> currentModOrigin;
    
    nmix::Stage& stage;
    nmix::OperationHandler& operationHandler;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Node)
};
    
}

#endif  // NMIX_NODE_H
