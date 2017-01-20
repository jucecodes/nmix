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

#ifndef NMIX_STAGE_H
#define NMIX_STAGE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "nmix_Anchor.h"

namespace nmix
{
    
struct Node;
struct Output;
struct OperationHandler;
struct Viewport;
struct Anchor;

struct Stage : public juce::Component,
               public juce::LassoSource<nmix::Node*>
{
public:
    Stage(nmix::OperationHandler& o);
    ~Stage();
    
    enum ColourIds
    {
        backgroundColourId,
        foregroundColourId
    };
    
    void mouseDown      (const juce::MouseEvent& e) override;
    void mouseDrag      (const juce::MouseEvent& e) override;
    void mouseUp        (const juce::MouseEvent& e) override;

    void findLassoItemsInArea(juce::Array<nmix::Node*>& results, const juce::Rectangle<int>& area) override;
    juce::SelectedItemSet<Node*>& getLassoSelection() override;
    
    void paint(juce::Graphics& g) override;

    int nodeSize;

    nmix::Output* master;
    nmix::Anchor* anchor;

    juce::LassoComponent<nmix::Node*> lasso;
    
    nmix::OperationHandler& operationHandler;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Stage)
};
    
}

#endif  // NMIX_STAGE_H
