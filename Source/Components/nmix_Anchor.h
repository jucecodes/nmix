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

#ifndef NMIX_ANCHOR_H
#define NMIX_ANCHOR_H

#include "../JuceLibraryCode/JuceHeader.h"

namespace nmix
{
 
struct Node;
struct Stage;

struct Anchor : public juce::Component
{
 
    Anchor(nmix::Stage& s);
    ~Anchor();

    void setNode(nmix::Node* n);

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    
    void paint(juce::Graphics& g) override;

    void moved() override;

    bool snapsToOutput;

    juce::ComponentDragger dragger;

    nmix::Node* currentNode;
    nmix::Node* currentSnap;
    
    nmix::Stage& currentStage;
    
};
    
}


#endif  // NMIX_ANCHOR_H
