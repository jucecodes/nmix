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
#include "nmix_Operations.h"
#include "nmix_OperationHandler.h"
#include "nmix_Viewport.h"
#include "nmix_Colours.h"

nmix::Node::Node(nmix::Stage& s, nmix::OperationHandler& o) : juce::Component("Unnamed"), stage(s), operationHandler(o)
{
    status = 0;

    setColour(backgroundColourId, nmix::Colours::Blue);
    setColour(highlightColourId,  nmix::Colours::White);
    
    setRepaintsOnMouseActivity(true);
    
    setSize(s.nodeSize, s.nodeSize);
}

nmix::Node::~Node()
{

}

void nmix::Node::mouseEnter(const juce::MouseEvent &e)
{
    status |= Hovered;
}

void nmix::Node::mouseExit(const juce::MouseEvent &e)
{
    status &= ~Hovered;
}

void nmix::Node::mouseDown(const juce::MouseEvent &e)
{

    toFront(true);

    if (e.mods.isLeftButtonDown())
    {
        if (e.mods.isCtrlDown())
        {
            mouseDownResult = operationHandler.selectedNodes.addToSelectionOnMouseDown(this, e.mods);
            operationHandler.currentViewport->invokeContextualMenu(e);
        }
        else
        {
            mouseDownResult = operationHandler.selectedNodes.addToSelectionOnMouseDown(this, e.mods);

            operationHandler.mouseModOrigin = operationHandler.mouseOpOrigin = stage.getLocalPoint(this, e.getMouseDownPosition());

            for (nmix::Node** n = operationHandler.selectedNodes.begin(); n != operationHandler.selectedNodes.end(); ++n)
            {
                (*n)->currentModOrigin = (*n)->currentOpOrigin = (*n)->getPosition();
            }
        }
    }
    else if (e.mods.isRightButtonDown())
    {
        mouseDownResult = operationHandler.selectedNodes.addToSelectionOnMouseDown(this, e.mods);
        operationHandler.currentViewport->invokeContextualMenu(e);
    }

}

void nmix::Node::mouseDrag(const juce::MouseEvent &e)
{
    operationHandler.positionSelection(e);
}

void nmix::Node::mouseUp(const juce::MouseEvent &e)
{
    operationHandler.selectedNodes.addToSelectionOnMouseUp(this, e.mods, e.mouseWasDraggedSinceMouseDown(), mouseDownResult);
    operationHandler.currentOperation = nmix::Operation::None;
}

void nmix::Node::paint(juce::Graphics &g)
{
    int w = getWidth();
    int h = getHeight();
    
    juce::Colour body      = findColour(backgroundColourId);
    juce::Colour highlight = findColour(highlightColourId).withAlpha(0.0f);
    
    if (status & Hovered)
    {
        body = body.brighter();
    }
    
    if (status & Selected)
    {
        body      = body.brighter();
        highlight = highlight.withAlpha(1.0f);
    }
    
    if (status & Locked)
    {
        body = body.darker();
    }
    
    g.setColour(body);
    g.fillEllipse(0, 0, w, h);
    
    g.setColour(highlight);
    g.drawEllipse(1, 1, w-2, h-2, 2);
}
