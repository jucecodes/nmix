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

#include "nmix_Node.h"
#include "nmix_Stage.h"
#include "nmix_Colours.h"

nmix::Node::Node(nmix::Stage& s) : stage(s)
{
    status = 0;
    
    setColour(backgroundColourId, nmix::Colours::Blue);
    setColour(highlightColourId,  nmix::Colours::White);
    
    setRepaintsOnMouseActivity(true);
}

nmix::Node::~Node()
{

}

void nmix::Node::mouseEnter(const juce::MouseEvent &e)
{
    status |= hovered;
}

void nmix::Node::mouseExit(const juce::MouseEvent &e)
{
    status &= ~hovered;
}

void nmix::Node::mouseDown(const juce::MouseEvent &e)
{
    toFront(true);
    mouseDownResult = stage.selectedNodes.addToSelectionOnMouseDown(this, e.mods);
}

void nmix::Node::mouseDrag(const juce::MouseEvent &e)
{
    
}

void nmix::Node::mouseUp(const juce::MouseEvent &e)
{
    stage.selectedNodes.addToSelectionOnMouseUp(this, e.mods, e.mouseWasDraggedSinceMouseDown(), mouseDownResult);
}

void nmix::Node::paint(juce::Graphics &g)
{
    int w = getWidth();
    int h = getHeight();
    
    juce::Colour body      = juce::Colour(0x00000000);
    juce::Colour highlight = juce::Colour(0x00000000);
    
    if (status & hovered)
    {
        body      = findColour(backgroundColourId).brighter();
    }
    
    if (status & selected)
    {
        body      = findColour(backgroundColourId).brighter();
        highlight = findColour(highlightColourId);
    }
    if (status == 0)
    {
        body = findColour(backgroundColourId);
    }
    
    g.setColour(body);
    g.fillEllipse(0, 0, w, h);
    
    g.setColour(highlight);
    g.drawEllipse(1, 1, w-2, h-2, 2);
}

void nmix::Node::resized()
{
    
}
